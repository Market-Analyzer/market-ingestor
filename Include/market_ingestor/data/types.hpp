#ifndef MARKET_INGESTOR_DATA_TYPES_HPP_
#define MARKET_INGESTOR_DATA_TYPES_HPP_

#include <cstdint>
#include <compare>

namespace market_ingestor::data 
{
  namespace detail 
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

      [[nodiscard]] friend constexpr ArithmeticType operator+(const ArithmeticType& lhs, const ArithmeticType& rhs)
      {
        lhs+=rhs;
        return lhs;
      }

      [[nodiscard]] friend constexpr ArithmeticType operator-(const ArithmeticType& lhs, const ArithmeticType& rhs)
      {
        lhs-=rhs;
        return lhs;
      }
    };

    template<typename Tag, typename T>
    class DiscreteType
    {
      private:
        T value;

      public:
        explicit constexpr DiscreteType(T v) : value(v) {}
        explicit constexpr operator T() const { return value; }

        constexpr auto operator<=>(const DiscreteType&) const = default;
    };

    
    struct VolumeTag     {};
    struct PriceTag      {};
    struct SymbolTag     {};
    struct OrderTag      {};
  }

  using Volume = detail::ArithmeticType<detail::VolumeTag, uint64_t>;
  using Price  = detail::ArithmeticType<detail::PriceTag,  int64_t>;

  using SymbolID = detail::DiscreteType<detail::SymbolTag, uint32_t>;
  using OrderID  = detail::DiscreteType<detail::OrderTag,  uint64_t>;
}

#endif
