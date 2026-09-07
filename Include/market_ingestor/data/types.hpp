#ifndef MARKET_INGESTOR_DATA_TYPES_HPP_
#define MARKET_INGESTOR_DATA_TYPES_HPP_

#include <cstdint>

#include <market_ingestor/core/strong_types.hpp>

namespace market_ingestor::data 
{
  namespace details
  {
    struct VolumeTag {};
    struct PriceTag  {};
    struct SymbolTag {};
    struct OrderTag  {};
  }

  using Volume = core::ArithmeticType<details::VolumeTag, std::uint32_t>;
  using Price  = core::ArithmeticType<details::PriceTag,  std::int32_t>;

  using SymbolID = core::DiscreteType<details::SymbolTag, std::uint32_t>;
  using OrderID  = core::DiscreteType<details::OrderTag,  std::uint64_t>;
}

#endif