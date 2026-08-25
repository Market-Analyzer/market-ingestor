#include <gtest/gtest.h>

#include "market_ingestor/data/order.hpp"
#include "market_ingestor/data/types.hpp"

using namespace market_ingestor::data;

TEST(OrderTest, DefaultConstructedOrderIsSentinel) {
  Order order;

  EXPECT_EQ(order.id(), OrderID::sentinel());
  EXPECT_FALSE(order.id().has_value());
}

TEST(OrderTest, ReinitializeSetsFields) {
  Order order;
  order.reinitialize(OrderID{1}, OrderParams{Volume{100}, Price{5000}, SymbolID{7}, OrderSide::BID, OrderType::LIMIT});

  EXPECT_EQ(order.id(), OrderID{1});
  EXPECT_EQ(order.volume(), Volume{100});
  EXPECT_EQ(order.price(), Price{5000});
  EXPECT_EQ(order.symbol(), SymbolID{7});
  EXPECT_EQ(order.side(), OrderSide::BID);
  EXPECT_EQ(order.type(), OrderType::LIMIT);
}

TEST(OrderTest, FillReducesVolume) {
  Order order;
  order.reinitialize(OrderID{1}, OrderParams{Volume{100}, Price{5000}, SymbolID{7}, OrderSide::BID, OrderType::LIMIT});
  order.fill(Volume{40});

  EXPECT_EQ(order.volume(), Volume{60});
}

TEST(OrderTest, ResetReturnsToSentinelState) {
  Order order;
  order.reinitialize(OrderID{1}, OrderParams{Volume{100}, Price{5000}, SymbolID{7}, OrderSide::BID, OrderType::LIMIT});
  order.reset();

  EXPECT_EQ(order.id(), OrderID::sentinel());
  EXPECT_EQ(order.side(), OrderSide::NONE);
  EXPECT_EQ(order.type(), OrderType::UNKNOWN);
}
