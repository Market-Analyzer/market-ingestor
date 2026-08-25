#include <gtest/gtest.h>

#include "market_ingestor/data/types.hpp"

using namespace market_ingestor::data;

TEST(VolumeTest, DefaultConstructsToZero) {
  Volume v;

  EXPECT_EQ(static_cast<uint64_t>(v), 0u);
}

TEST(VolumeTest, ConstructsFromValue) {
  Volume v{42};

  EXPECT_EQ(static_cast<uint64_t>(v), 42u);
}

TEST(VolumeTest, AdditionSumsValues) {
  EXPECT_EQ(Volume{10} + Volume{5}, Volume{15});
}

TEST(VolumeTest, SubtractionDiffsValues) {
  EXPECT_EQ(Volume{10} - Volume{5}, Volume{5});
}

TEST(VolumeTest, CompoundAddAssignAccumulates) {
  Volume v{10};
  v += Volume{5};

  EXPECT_EQ(v, Volume{15});
}

TEST(VolumeTest, CompoundSubtractAssignAccumulates) {
  Volume v{10};
  v -= Volume{5};

  EXPECT_EQ(v, Volume{5});
}

TEST(VolumeTest, OrderingComparesUnderlyingValue) {
  EXPECT_LT(Volume{5}, Volume{10});
  EXPECT_GT(Volume{10}, Volume{5});
  EXPECT_LE(Volume{5}, Volume{5});
  EXPECT_GE(Volume{5}, Volume{5});
}

TEST(PriceTest, SupportsNegativeValues) {
  Price p{-100};

  EXPECT_EQ(static_cast<int64_t>(p), -100);
}

TEST(PriceTest, SubtractionCanGoNegative) {
  EXPECT_EQ(Price{5} - Price{10}, Price{-5});
}

TEST(SymbolIDTest, SentinelHasNoValue) {
  EXPECT_FALSE(SymbolID::sentinel().has_value());
}

TEST(SymbolIDTest, NonSentinelHasValue) {
  EXPECT_TRUE(SymbolID{1}.has_value());
}

TEST(SymbolIDTest, EqualityComparesUnderlyingValue) {
  EXPECT_EQ(SymbolID{7}, SymbolID{7});
  EXPECT_NE(SymbolID{7}, SymbolID{8});
}

TEST(SymbolIDTest, ConvertsBackToUnderlyingValue) {
  EXPECT_EQ(static_cast<uint32_t>(SymbolID{7}), 7u);
}

TEST(OrderIDTest, SentinelHasNoValue) {
  EXPECT_FALSE(OrderID::sentinel().has_value());
}

TEST(OrderIDTest, NonSentinelHasValue) {
  EXPECT_TRUE(OrderID{1}.has_value());
}

TEST(OrderIDTest, EqualityComparesUnderlyingValue) {
  EXPECT_EQ(OrderID{42}, OrderID{42});
  EXPECT_NE(OrderID{42}, OrderID{43});
}
