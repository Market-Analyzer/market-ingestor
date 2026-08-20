#ifndef MARKET_INGESTOR_DATA_TYPES_HPP_
#define MARKET_INGESTOR_DATA_TYPES_HPP_

#include <cstdint>
#include <compare>
#include <concepts>

namespace market_ingestor::data 
{
  namespace types_details
  {
    template<typename Tag, typename T = uint64_t>
    class ArithmeticType
    {
    private:
      T value;

    public:
      explicit constexpr ArithmeticType(T v) : value(v) {}
      explicit constexpr operator T() const { return value; }

      constexpr auto operator<=>(const ArithmeticType&) const = default;

      constexpr ArithmeticType& operator+=(const ArithmeticType& rhs) { value += rhs.value; return *this; }
      constexpr ArithmeticType& operator-=(const ArithmeticType& rhs) { value -= rhs.value; return *this; }

      [[nodiscard]] friend constexpr ArithmeticType operator+(ArithmeticType lhs, const ArithmeticType& rhs)
      {
        lhs+=rhs;
        return lhs;
      }

      [[nodiscard]] friend constexpr ArithmeticType operator-(ArithmeticType lhs, const ArithmeticType& rhs)
      {
        lhs-=rhs;
        return lhs;
      }
    };

    
    template<typename Tag, std::integral T = uint64_t, T SentinelValue = T{0}>
    class DiscreteType
    {
    public:
      explicit constexpr DiscreteType(T v) : value_(v) {}
      explicit constexpr operator T() const { return value_; }

      [[nodiscard]] static constexpr DiscreteType sentinel() { return DiscreteType{SentinelValue}; }
      [[nodiscard]] constexpr bool has_value() const { return value_ != SentinelValue; }

      constexpr auto operator<=>(const DiscreteType&) const = default;
      
    private:
      T value_;
    };

    
    struct VolumeTag     {};
    struct PriceTag      {};
    struct SymbolTag     {};
    struct OrderTag      {};
  }

  using Volume = types_details::ArithmeticType<types_details::VolumeTag, uint64_t>;
  using Price  = types_details::ArithmeticType<types_details::PriceTag,  int64_t>;

  using SymbolID = types_details::DiscreteType<types_details::SymbolTag, uint32_t>;
  using OrderID  = types_details::DiscreteType<types_details::OrderTag,  uint64_t>;
}

#endif