cmake_minimum_required(VERSION 3.24)

option(BUILD_TESTING "Build tests" OFF)
if(BUILD_TESTING)
  list(APPEND VCPKG_MANIFEST_FEATURES "tests")
endif()

project(MarketIngestor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(market_ingestor_core INTERFACE)
add_library(MarketIngestor::Core ALIAS market_ingestor_core)

target_include_directories(market_ingestor_core INTERFACE
  ${PROJECT_SOURCE_DIR}/Include
)

add_executable(MarketIngestor
  src/main.cpp
)

target_link_libraries(MarketIngestor PRIVATE
  MarketIngestor::Core
)

if(BUILD_TESTING)
  enable_testing()
  add_subdirectory(tests)
endif()
