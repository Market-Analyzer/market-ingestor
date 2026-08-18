#ifndef MARKET_INGESTOR_DATA_ORDER_HPP_
#define MARKET_INGESTOR_DATA_ORDER_HPP_

#include <cassert>
#include <type_traits>

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
    constexpr Order()
    : order_id_(InvalidOrderID), volume_(Volume{0}), price_(Price{0}),
      symbol_id_(SymbolID{0}), side_(OrderSide::NONE), 
      type_(OrderType::UNKNOWN) {}

    constexpr Order(OrderID order_id, OrderParams p)
    : order_id_(order_id), volume_(p.volume), price_(p.price),
      symbol_id_(p.symbol_id), side_(p.side), type_(p.type) {}

    constexpr Order& reset()
    {
      order_id_  = InvalidOrderID;
      volume_    = Volume{0};
      price_     = Price{0};
      symbol_id_ = SymbolID{0};
      side_      = OrderSide::NONE; 
      type_      = OrderType::UNKNOWN;

      return *this;
    }
    
    constexpr Order& reinitialize(OrderID order_id, OrderParams p)
    {
      order_id_  = order_id;
      volume_    = p.volume;
      price_     = p.price;
      symbol_id_ = p.symbol_id;
      side_      = p.side; 
      type_      = p.type;

      return *this;
    }

    constexpr Order& fill(const Volume v)
    { 
      assert(v <= volume_ && "fill quantity exceeds remaining volume");
      volume_ -= v;
      return *this;
    }
    
    constexpr OrderID   id()     const { return order_id_;  }
    constexpr Volume    volume() const { return volume_;    }
    constexpr Price     price()  const { return price_;     }
    constexpr SymbolID  symbol() const { return symbol_id_; }
    constexpr OrderSide side()   const { return side_;      }
    constexpr OrderType type()   const { return type_;      }

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
  static_assert(std::is_trivially_copyable_v<Order>, "Order must remain trivially copyable for pooling.");
}

#endif
