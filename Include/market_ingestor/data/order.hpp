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


  struct OrderParams {
    Volume    volume;
    Price     price;
    SymbolID  symbol_id;
    OrderSide side;
    OrderType type;
  };


  class alignas(32) Order 
  {
  public:
    constexpr Order(OrderID order_id, OrderParams p)
    : order_id_(order_id), volume_(p.volume), price_(p.price),
      symbol_id_(p.symbol_id), side_(p.side), type_(p.type) {}

    
    OrderID   id()     const { return order_id_;  }
    Volume    volume() const { return volume_;    }
    Price     price()  const { return price_;     }
    SymbolID  symbol() const { return symbol_id_; }
    OrderSide side()   const { return side_;      }
    OrderType type()   const { return type_;      }


  private:
    OrderID   order_id_;
    Volume    volume_;
    Price     price_;
    SymbolID  symbol_id_;
    OrderSide side_;
    OrderType type_;

    uint8_t   pad[2]{};
  };

  static_assert(sizeof(Order)  == 32, "Order struct must be exactly 32 bytes.");
  static_assert(alignof(Order) == 32, "Order struct must be aligned to 32 bytes.");
}

#endif
