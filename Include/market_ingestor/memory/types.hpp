#ifndef MARKET_INGESTOR_MEMORY_TYPES_HPP_
#define MARKET_INGESTOR_MEMORY_TYPES_HPP_

#include <cstdint>

namespace market_ingestor::memory
{
  class SlotIndex
  {
  public:
    constexpr SlotIndex() noexcept = default;
    explicit constexpr SlotIndex(std::uint32_t i) noexcept : v_(i) {}

    [[nodiscard]] static constexpr SlotIndex    null() noexcept { return SlotIndex{}; }
    [[nodiscard]] constexpr bool             is_null() const noexcept { return value() == SENTINEL_VALUE_; } 
    [[nodiscard]] constexpr std::uint32_t      value() const noexcept { return v_; }

    friend constexpr bool operator==(SlotIndex, SlotIndex) noexcept = default;

  private:
    static constexpr std::uint32_t SENTINEL_VALUE_ = 0;
    std::uint32_t v_ = 0;
  };


  template<std::uint32_t Bits>
  class Generation
  {
    static_assert(0 < Bits && Bits < 32, "Bits must leave room for an index");

  public:
    constexpr Generation() noexcept = default;
    explicit constexpr Generation(std::uint32_t i) noexcept : v_(i & MASK) {}
    
    [[nodiscard]] constexpr Generation     next() const noexcept { return Generation{v_ + 1u}; }
    [[nodiscard]] constexpr std::uint32_t value() const noexcept { return v_; }

    friend constexpr bool operator==(Generation, Generation) noexcept = default;

  private:
    static constexpr std::uint32_t MASK = (std::uint32_t{1} << Bits) - 1;
    std::uint32_t v_ = 0;
  };
}

#endif