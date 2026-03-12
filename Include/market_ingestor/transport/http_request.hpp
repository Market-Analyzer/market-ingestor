#pragma once

#include <boost/beast.hpp>
#include <string>
#include <vector>
#include <future>

namespace http  = boost::beast::http;

namespace market_ingestor::transport
{
  struct HttpRequestSpecification {
    http::verb method;
    std::string target;
    std::vector<std::pair<std::string, std::string>> headers;
    std::string body = "";
  };

  struct PendingHttpRequest {
    http::request<http::string_body> req;
    std::promise<http::response<http::string_body>> promise;
  };
}