#pragma once

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/beast.hpp>
#include <boost/beast/ssl.hpp>
#include <iostream>
#include <memory>
#include <thread>
#include <deque>
#include <future>
#include <vector>
#include <optional>

#include "http_request.hpp"

namespace beast = boost::beast;
namespace http  = beast::http;
namespace net   = boost::asio;
namespace ssl   = boost::asio::ssl;
using tcp       = net::ip::tcp;

namespace market_ingestor::transport
{
  class RestSession : public std::enable_shared_from_this<RestSession>
  {
  public:
    using Response = http::response<http::string_body>;
    using ResponseFuture = std::future<Response>;

    RestSession(
      net::io_context& ioc,
      net::ssl::context& tls_ctx,
      std::string host,
      std::string port
    );

    void start();
    ResponseFuture enqueue(HttpRequestSpecification spec);
  
  private:
    enum class State { Idle, Connecting, Ready, Writing, Reading };
    State state_ = State::Idle;
    tcp::resolver resolver_;
    beast::ssl_stream<beast::tcp_stream> stream_;
    beast::flat_buffer buffer_;
    std::optional<Response> active_response_;
    std::string host_;
    std::string port_;
    std::deque<PendingHttpRequest> queue_;

    http::request<http::string_body> build_request_(const HttpRequestSpecification& spec) const;
    void dispatch_next_();
    void on_resolve_(beast::error_code ec, const tcp::resolver::results_type &results);
    void on_connect_(beast::error_code ec, tcp::resolver::results_type::endpoint_type);
    void on_handshake_(beast::error_code ec);
    void on_write_(beast::error_code ec, std::size_t bytes_transferred);
    void on_read_(beast::error_code ec, std::size_t bytes_transferred);
    void fail_(beast::error_code ec, const char* where);
  };
}