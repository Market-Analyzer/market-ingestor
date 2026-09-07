#ifndef MARKET_INGESTOR_DATA_ORDER_HPP_
#define MARKET_INGESTOR_DATA_ORDER_HPP_

#include <cassert>
#include <type_traits>

#include <market_ingestor/data/types.hpp>
#include <market_ingestor/memory/types.hpp>

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
    OrderID   id        = OrderID::sentinel();
    Volume    volume    = Volume{};
    Price     price     = Price{};
    SymbolID  symbol_id = SymbolID::sentinel();
    OrderSide side      = OrderSide::NONE;
    OrderType type      = OrderType::UNKNOWN;
  };


  class Order 
  {
  public:
    using pool_category = memory::poolable_tag;
    using params_type   = OrderParams;

    constexpr Order()
    : order_id_(OrderID::sentinel()), volume_{}, price_{},
      symbol_id_(SymbolID::sentinel()), side_(OrderSide::NONE),
      type_(OrderType::UNKNOWN) {}

    constexpr Order(OrderID order_id, OrderParams p)
    : order_id_(order_id), volume_(p.volume), price_(p.price),
      symbol_id_(p.symbol_id), side_(p.side), type_(p.type) {}
    
    constexpr void reinitialize(const params_type& p) noexcept
    {
      order_id_  = p.id;
      volume_    = p.volume;
      price_     = p.price;
      symbol_id_ = p.symbol_id;
      side_      = p.side; 
      type_      = p.type;
    }

    constexpr void reset()
    {
      reinitialize(OrderParams{});
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
    OrderID   order_id_;  // 8 bytes
    Volume    volume_;    // 4 bytes
    Price     price_;     // 4 bytes
    SymbolID  symbol_id_; // 4 bytes
    OrderSide side_;      // 1 byte
    OrderType type_;      // 1 byte
  };
  
  static_assert(std::is_trivially_copyable_v<Order>, "Order must remain trivially copyable for pooling.");
  static_assert(sizeof(Order)  == 24, "Order size must stay 24 bytes for pool packing");
  static_assert(alignof(Order) == 8,  "Order alignment assumption changed");
}

#endif
