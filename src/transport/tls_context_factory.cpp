#include <market_ingestor/transport/tls_context_factory.hpp>

ssl::context TlsContextFactory::create_client_context(const TlsConfig &config)
{
  if(config.ca_bundle_path.empty())
    throw std::runtime_error("CA bundle path must be specified.");

  ssl::context ctx{ssl::context::tls_client};

  ctx.set_options(
    ssl::context::default_workarounds
    | ssl::context::no_sslv2
    | ssl::context::no_sslv3
    | ssl::context::no_tlsv1
    | ssl::context::no_tlsv1_1
    | SSL_OP_NO_COMPRESSION
  );

  ctx.set_verify_mode(ssl::verify_peer);
  ctx.load_verify_file(config.ca_bundle_path);

  if(config.certificate_chain_path)
    ctx.use_certificate_chain_file(*config.certificate_chain_path);

  if(!config.alpn_protocols.empty())
  {
    const auto alpn = build_alpn_wire_format_(config.alpn_protocols);
    if(SSL_CTX_set_alpn_protos(ctx.native_handle(), alpn.data(), static_cast<unsigned int>(alpn.size())) != 0)
      throw std::runtime_error("Failed to configure ALPN protocols.");
  }

  return ctx;
}

std::vector<unsigned char> TlsContextFactory::build_alpn_wire_format_(const std::vector<std::string> &protocols)
{
  std::vector<unsigned char> wire;
  wire.reserve(32);
  
  for(auto &proto : protocols)
  {
    if(proto.size() > 255)
      throw std::runtime_error("ALPN Protocol should be less than 255 bytes.");
    if(proto.empty())
      throw std::runtime_error("ALPN Protocol cannot be an empty string.");

    wire.push_back(static_cast<unsigned char>(proto.size()));
    wire.insert(wire.end(), proto.begin(), proto.end());
  }

  return wire;
}
