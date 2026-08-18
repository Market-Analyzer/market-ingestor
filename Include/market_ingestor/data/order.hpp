#ifndef MARKET_INGESTOR_DATA_ORDER_HPP_
#define MARKET_INGESTOR_DATA_ORDER_HPP_

#include "types.hpp"

namespace market_ingestor::data
{
  enum class OrderSide : uint8_t {
    NONE = 0,
    ASK  = '1',
    BID  = '2',
  };

  enum class OrderType : uint8_t {
    UNKNOWN    = 0,
    MARKET     = '1',
    LIMIT      = '2',
    STOP       = '3',
    STOP_LIMIT = '4',
    PEGGED     = 'P',
  };

  struct alignas(32) Order {
    OrderID order_id;
    Volume  volume;
    Price   price;

    SymbolID symbol_id;

    OrderSide side;
    OrderType type;

    uint8_t pad[2];
  };


  static_assert(sizeof(Order)  == 32, "Order struct must be exactly 32 bytes.");
  static_assert(alignof(Order) == 32, "Order struct must be aligned to 32 bytes.");
}

#endif
