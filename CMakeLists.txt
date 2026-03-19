cmake_minimum_required(VERSION 3.24)

project(MarketIngestor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(boost_system CONFIG REQUIRED)
find_package(boost_asio CONFIG REQUIRED)
find_package(boost_beast CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)

add_executable(MarketIngestor
  src/main.cpp
  src/transport/tls_context_factory.cpp
  src/transport/rest_session.cpp
)

target_include_directories(MarketIngestor PRIVATE
  ${PROJECT_SOURCE_DIR}/Include
)

target_link_libraries(MarketIngestor PRIVATE
  Boost::system
  Boost::asio
  Boost::beast
  OpenSSL::SSL
  OpenSSL::Crypto
  market_ingestor_proto
)

if (MSVC)
  target_compile_options(MarketIngestor PRIVATE /bigobj)
  target_compile_definitions(MarketIngestor PRIVATE _WIN32_WINNT=0x0A00)
endif()

add_custom_command(TARGET MarketIngestor POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/resources/certs
    $<TARGET_FILE_DIR:MarketIngestor>/resources/certs
)

add_custom_target(run
  COMMAND MarketIngestor
  DEPENDS MarketIngestor
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  COMMENT "Running market-ingestor..."
)