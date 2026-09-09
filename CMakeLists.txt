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


# Expose the compile database at the source root so tooling (clangd, clang-tidy,
# IWYU) finds it with no per-editor config.
#
# Symlink preferred. Attempt then fallback on copy.
if(PROJECT_IS_TOP_LEVEL AND CMAKE_EXPORT_COMPILE_COMMANDS)
  file(CREATE_LINK
    "${CMAKE_BINARY_DIR}/compile_commands.json"
    "${CMAKE_SOURCE_DIR}/compile_commands.json"
    SYMBOLIC
    RESULT compile_db_link
  )

  if(NOT compile_db_link EQUAL 0)
    # Symlink refused.
    add_custom_command(
      OUTPUT  "${CMAKE_SOURCE_DIR}/compile_commands.json"
      COMMAND "${CMAKE_COMMAND}" -E copy_if_different
              "${CMAKE_BINARY_DIR}/compile_commands.json"
              "${CMAKE_SOURCE_DIR}/compile_commands.json"
      DEPENDS "${CMAKE_BINARY_DIR}/compile_commands.json"
      COMMENT "Copying compile_commands.json to source root (symlink unavailable)"
      VERBATIM
    )
    add_custom_target(compile_db ALL
      DEPENDS "${CMAKE_SOURCE_DIR}/compile_commands.json"
    )
  endif()
endif()
