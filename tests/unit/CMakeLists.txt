find_package(GTest CONFIG REQUIRED)

add_executable(MarketIngestorUnitTests
  data/types_test.cpp
)

target_link_libraries(MarketIngestorUnitTests PRIVATE
  MarketIngestor::Core
  GTest::gtest
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(MarketIngestorUnitTests
  PROPERTIES LABELS "unit"
)
