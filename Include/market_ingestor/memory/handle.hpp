#ifndef MARKET_INGESTOR_MEMORY_HANDLE_HPP_
#define MARKET_INGESTOR_MEMORY_HANDLE_HPP_

#include <cstdint>
#include <bit>

#include <market_ingestor/memory/types.hpp>

namespace market_ingestor::memory
{
  template<std::uint32_t N>
  struct Handle
  {
    static_assert(std::has_single_bit(N), "N must be a power of two");
    static_assert(N > 1, "slot 0 is reserved as the null handle");

    static constexpr std::uint32_t INDEX_BITS = std::countr_zero(N);
    static constexpr std::uint32_t INDEX_MASK = N - 1;
    static constexpr std::uint32_t GEN_BITS   = 32 - INDEX_BITS;

    static_assert(GEN_BITS >= 8, "N leaves fewer than 8 generation bits");

    using generation_type = Generation<GEN_BITS>;

    std::uint32_t bits = 0;

    constexpr Handle() noexcept = default;
    constexpr Handle(SlotIndex i, generation_type g) noexcept
      : bits((g.value() << INDEX_BITS) | (i.value() & INDEX_MASK)) {}

    [[nodiscard]] constexpr SlotIndex index() const noexcept
    {
      return SlotIndex{bits & INDEX_MASK}; 
    }

    [[nodiscard]] constexpr generation_type generation() const noexcept
    {
      return generation_type{bits >> INDEX_BITS};
    }

    [[nodiscard]] constexpr bool is_null() const noexcept
    {
      return index().is_null();
    }

    friend constexpr bool operator==(Handle, Handle) noexcept = default;
  };
}

#endif