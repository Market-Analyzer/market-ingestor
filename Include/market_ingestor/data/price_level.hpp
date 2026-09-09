#ifndef MARKET_INGESTOR_DATA_ORDER_HPP_
#define MARKET_INGESTOR_DATA_ORDER_HPP_

#include <market_ingestor/data/types.hpp>
#include <market_ingestor/memory/handle.hpp>

namespace market_ingestor::data
{

  struct OrderLocation
  {
    memory::Handle<> h; // fill later
    Price  p;
    Volume v;
  };

  struct PLParams
  {;
    Price  price {0};
    Volume volume{0};
  };

  class PriceLevel
  {
  public:
    using pool_category = memory::poolable_tag;
    using params_type   = PLParams;

    constexpr PriceLevel() noexcept = default;
    constexpr PriceLevel(const params_type& p)
      : price_(p.price), volume_(p.volume)
    {}

    constexpr PriceLevel(PriceLevel&) noexcept = delete;

    

  private:
    Price   price_{0};
    Volume volume_{0};
    memory::FifoCircular<OrderLocation> fifo_;
  };
}

#endif