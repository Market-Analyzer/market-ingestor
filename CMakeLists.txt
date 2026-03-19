cmake_minimum_required(VERSION 3.24)

project(MarketIngestor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(boost_system CONFIG REQUIRED)
find_package(boost_asio CONFIG REQUIRED)
find_package(boost_beast CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)


find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_program(PROTOC protoc REQUIRED)
find_program(GRPC_CPP_PLUGIN grpc_cpp_plugin REQUIRED)
 

set(PROTO_SRC_DIR "${CMAKE_SOURCE_DIR}/proto")
set(PROTO_GEN_DIR "${CMAKE_BINARY_DIR}/generated")
file(MAKE_DIRECTORY ${PROTO_GEN_DIR})

set(PROTO_FILE "${PROTO_SRC_DIR}/market_ingestor.proto")

add_custom_command(
  OUTPUT
    ${PROTO_GEN_DIR}/market_ingestor.pb.cc
    ${PROTO_GEN_DIR}/market_ingestor.pb.h
    ${PROTO_GEN_DIR}/market_ingestor.grpc.pb.cc
    ${PROTO_GEN_DIR}/market_ingestor.grpc.pb.h
  COMMAND ${PROTOC}
    --proto_path=${PROTO_SRC_DIR}
    --cpp_out=${PROTO_GEN_DIR}
    --grpc_out=${PROTO_GEN_DIR}
    --plugin=protoc-gen-grpc=${GRPC_CPP_PLUGIN}
    ${PROTO_FILE}
  DEPENDS ${PROTO_FILE}
  COMMENT "Generating C++ from market_ingestor.proto"
)
 

add_library(market_ingestor_proto
    ${PROTO_GEN_DIR}/market_ingestor.pb.cc
    ${PROTO_GEN_DIR}/market_ingestor.grpc.pb.cc
)

target_include_directories(market_ingestor_proto PUBLIC ${PROTO_GEN_DIR})
target_link_libraries(market_ingestor_proto PUBLIC
    protobuf::libprotobuf
    gRPC::grpc++
)


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