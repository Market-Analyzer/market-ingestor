cmake_minimum_required(VERSION 3.24)

project(MarketIngestor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(boost_system CONFIG REQUIRED)
find_package(boost_asio CONFIG REQUIRED)
find_package(boost_beast CONFIG REQUIRED)


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
)

target_include_directories(MarketIngestor PRIVATE
  ${PROJECT_SOURCE_DIR}/Include
)

target_link_libraries(MarketIngestor PRIVATE
  Boost::system
  Boost::asio
  Boost::beast
  market_ingestor_proto
)