#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <iostream>

int main() {
  boost::asio::io_context ioc;
  boost::beast::flat_buffer buffer;

  std::cout << "Boost.Asio & Boost.Beast are working.\n";
  std::cout << "io_context ready.\n";
  std::cout << "buffer size: " << buffer.size() << '\n';

  return 0;
}