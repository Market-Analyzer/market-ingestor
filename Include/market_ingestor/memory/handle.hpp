#ifndef MARKET_INGESTOR_MEMORY_HANDLE_HPP_
#define MARKET_INGESTOR_MEMORY_HANDLE_HPP_

#include <cstdint>
#include <bit>

namespace market_ingestor::memory
{
  inline constexpr std::uint32_t NULL_INDEX = 0;

  template<std::uint32_t N>
  struct Handle
  {
    static_assert(std::has_single_bit(N), "N must be a power of two");
    static_assert(N > 1, "slot 0 is reserved as the null handle");

    static constexpr std::uint32_t INDEX_BITS = std::countr_zero(N);
    static constexpr std::uint32_t INDEX_MASK = N - 1;
    static constexpr std::uint32_t GEN_BITS   = 32 - INDEX_BITS;
    static constexpr std::uint32_t GEN_MASK   = (std::uint32_t{1} << GEN_BITS) - 1;

    static_assert(INDEX_BITS < 32 , "N leaves no room for a generation stamp");

    std::uint32_t bits = 0;

    constexpr Handle() noexcept = default;
    constexpr Handle(std::uint32_t i, std::uint32_t g) noexcept
      : bits(((g & GEN_MASK) << INDEX_BITS) | (i & INDEX_MASK)) {}

    [[nodiscard]] constexpr std::uint32_t index()      const noexcept { return bits & INDEX_MASK; }
    [[nodiscard]] constexpr std::uint32_t generation() const noexcept { return bits >> INDEX_BITS; }
    [[nodiscard]] constexpr bool          is_null()    const noexcept { return index() == NULL_INDEX; }

    friend constexpr bool operator==(Handle, Handle) noexcept = default;
  };
}

#endif