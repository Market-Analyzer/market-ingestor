#ifndef MARKET_INGESTOR_CORE_STRONGTYPES_HPP_
#define MARKET_INGESTOR_CORE_STRONGTYPES_HPP_

#include <cstdint>
#include <compare>  // IWYU pragma: keep
#include <concepts>

namespace market_ingestor::core
{
  template<typename Tag, std::integral T = std::uint64_t>
  class ArithmeticType
  {
  public:
    constexpr ArithmeticType() noexcept : value_{} {}
    explicit constexpr ArithmeticType(T v) noexcept : value_(v) {}

    explicit constexpr operator T() const noexcept { return value_; }
    constexpr auto operator<=>(const ArithmeticType&) const = default;
    constexpr ArithmeticType& operator+=(const ArithmeticType& rhs) noexcept { value_ += rhs.value_; return *this; }
    constexpr ArithmeticType& operator-=(const ArithmeticType& rhs) noexcept { value_ -= rhs.value_; return *this; }

    [[nodiscard]] friend constexpr ArithmeticType operator+(ArithmeticType lhs, const ArithmeticType& rhs) noexcept
    {
      lhs+=rhs;
      return lhs;
    }

    [[nodiscard]] friend constexpr ArithmeticType operator-(ArithmeticType lhs, const ArithmeticType& rhs) noexcept
    {
      lhs-=rhs;
      return lhs;
    }

  private:
    T value_;
  };


  template<typename Tag, std::integral T = std::uint64_t, T SentinelValue = T{0}>
  class DiscreteType
  {
  public:
    explicit constexpr DiscreteType(T v) noexcept : value_(v) {}
    explicit constexpr operator T() const noexcept { return value_; }

    [[nodiscard]] static constexpr DiscreteType sentinel() noexcept { return DiscreteType{SentinelValue}; }
    [[nodiscard]] constexpr bool has_value() const noexcept { return value_ != SentinelValue; }

    constexpr auto operator<=>(const DiscreteType&) const = default;

  private:
    T value_;
  };
}

#endif