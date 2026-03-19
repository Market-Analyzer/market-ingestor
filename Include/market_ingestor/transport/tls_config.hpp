#pragma once

#include <boost/asio/ssl.hpp>
#include <optional>
#include <string>
#include <vector>

namespace net = boost::asio;
namespace ssl = net::ssl;

struct TlsConfig
{
  std::string ca_bundle_path;
  std::optional<std::string> certificate_chain_path;
  std::vector<std::string> alpn_protocols;
};