#pragma once

#include <boost/asio/ssl.hpp>
#include <string>
#include <vector>

#include "tls_config.hpp"

class TlsContextFactory
{
public:
  static ssl::context create_client_context(const TlsConfig& config);

private:
  static std::vector<unsigned char> build_alpn_wire_format_(const std::vector<std::string>& protocols);
};