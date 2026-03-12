#include <market_ingestor/transport/rest_session.hpp>

namespace market_ingestor::transport
{
  RestSession::RestSession(net::io_context &ioc, net::ssl::context &tls_ctx, std::string host, std::string port)
    : resolver_(net::make_strand(ioc))
    , stream_(net::make_strand(ioc), tls_ctx)
    , host_(std::move(host))
    , port_(std::move(port))
  {}

  void RestSession::start()
  {
    if(!SSL_set_tlsext_host_name(stream_.native_handle(), host_.c_str()))
    {
      beast::error_code ec {
        static_cast<int>(::ERR_get_error()),
        net::error::get_ssl_category()
      };
      return fail_(ec, "sni");
    }

    stream_.set_verify_mode(ssl::verify_peer);
    stream_.set_verify_callback(ssl::host_name_verification(host_));

    state_ = State::Connecting;
    resolver_.async_resolve(
      host_, port_,
      beast::bind_front_handler(&RestSession::on_resolve_, shared_from_this())
    );
  }

  RestSession::ResponseFuture RestSession::enqueue(HttpRequestSpecification spec)
  {
    PendingHttpRequest pending;
    pending.req = build_request_(spec);
    auto future = pending.promise.get_future();

    // unsure
    net::dispatch(
      stream_.get_executor(),
      [this, self = shared_from_this(), p = std::move(pending)]() mutable
      {
        queue_.push_back(std::move(p));
        if (state_ == State::Ready)
          dispatch_next_();
      }
    );

    return future;
  }

  http::request<http::string_body> RestSession::build_request_(const HttpRequestSpecification &spec) const
  {
    http::request<http::string_body> req;

    req.version(11); // HTTP 1.1
    req.method(spec.method);
    req.target(spec.target);
    req.set(http::field::host, host_);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    req.keep_alive(true);

    for (const auto& [key, value] : spec.headers)
      req.set(key, value);

    req.body() = spec.body;
    req.prepare_payload();

    return req;
  }

  void RestSession::dispatch_next_()
  {
    if(queue_.empty())
    {
      state_ = State::Ready;
      return;
    }

    state_ = State::Writing;

    http::async_write(
      stream_,
      queue_.front().req,
      beast::bind_front_handler(&RestSession::on_write_, shared_from_this())
    );
  }

  void RestSession::on_resolve_(beast::error_code ec, const tcp::resolver::results_type &results)
  {
    if(ec) return fail_(ec, "resolve");

    beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(10));
    beast::get_lowest_layer(stream_).async_connect(
      results,
      beast::bind_front_handler(&RestSession::on_connect_, shared_from_this())
    );
  }

  void RestSession::on_connect_(beast::error_code ec, tcp::resolver::results_type::endpoint_type)
  {
    if(ec) return fail_(ec, "connect");
    
    stream_.async_handshake(
      ssl::stream_base::client,
      beast::bind_front_handler(&RestSession::on_handshake_, shared_from_this())
    );
  }

  void RestSession::on_handshake_(beast::error_code ec)
  {
    if(ec) return fail_(ec, "handshake");
    
    state_ = State::Ready;
    dispatch_next_();
  }

  void RestSession::on_write_(beast::error_code ec, std::size_t bytes_transferred)
  {
    if(ec) return fail_(ec, "write");

    state_ = State::Reading;
    active_response_.emplace();
    http::async_read(
      stream_,
      buffer_,
      *active_response_,
      beast::bind_front_handler(&RestSession::on_read_, shared_from_this())
    );
  }

  void RestSession::on_read_(beast::error_code ec, std::size_t bytes_transferred)
  {
    if(ec) return fail_(ec, "read");

    queue_.front().promise.set_value(std::move(*active_response_));
    active_response_.reset();
    buffer_.consume(buffer_.size());
    queue_.pop_front();

    state_ = State::Ready;
    dispatch_next_();
  }

  void RestSession::fail_(beast::error_code ec, const char *where)
  {
    std::cerr << "[RestSession] " << where << ": " << ec.message() << "\n";

    for(auto& pending : queue_)
      pending.promise.set_exception(std::make_exception_ptr(std::system_error(ec)));

    queue_.clear();
    active_response_.reset();
    state_ = State::Idle;
  }
}
