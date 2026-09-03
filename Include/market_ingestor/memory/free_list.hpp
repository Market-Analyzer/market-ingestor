#ifndef MARKET_INGESTOR_MEMORY_FREELIST_HPP_
#define MARKET_INGESTOR_MEMORY_FREELIST_HPP_

#include <cstdint>
#include <array>

namespace market_ingestor::memory
{
  inline constexpr std::uint32_t NULL_INDEX = 0;

  // very simple implementation of a FreeList
  // NOT THREAD SAFE
  template<std::uint32_t N>
  class FreeList
  {
    static_assert(N > 1, "N counts slot 0, which is reserved as null");

    public:
      constexpr FreeList() noexcept : top_(CAPACITY_) {
        available_indices_[0] = NULL_INDEX;
        for (std::uint32_t k = 1; k <= CAPACITY_; ++k) { available_indices_[k] = CAPACITY_ + 1 - k; }
      }

      FreeList(const FreeList&) = delete;
      FreeList& operator=(const FreeList&) = delete;

      [[nodiscard]] constexpr std::uint32_t next() noexcept {
        const std::uint32_t i = available_indices_[top_];
        top_ -= (top_ != 0);
        return i;
      }

      constexpr bool release(std::uint32_t i) noexcept {
        if (i == NULL_INDEX || i >= N || top_ >= CAPACITY_) [[unlikely]] return false;
        available_indices_[++top_] = i;
        return true;
      }

      [[nodiscard]] static constexpr std::uint32_t capacity() noexcept { return CAPACITY_; }
      [[nodiscard]] constexpr std::uint32_t available() const noexcept { return top_; }
      [[nodiscard]] constexpr bool exhausted() const noexcept { return top_ == 0; }

    private:
      static constexpr std::uint32_t CAPACITY_ = N - 1;

      std::array<std::uint32_t, N> available_indices_;
      std::uint32_t            top_;
  };
}

#endif