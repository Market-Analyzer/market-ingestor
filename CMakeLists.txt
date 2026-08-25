cmake_minimum_required(VERSION 3.24)

project(MarketIngestor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


add_executable(MarketIngestor
  src/main.cpp
)

target_include_directories(MarketIngestor PRIVATE
  ${PROJECT_SOURCE_DIR}/Include
)

target_link_libraries(MarketIngestor PRIVATE
)