#ifndef MARKET_INGESTOR_MEMORY_SLOTARRAY_HPP_
#define MARKET_INGESTOR_MEMORY_SLOTARRAY_HPP_

#include <array>

#include <market_ingestor/memory/types.hpp>

namespace market_ingestor::memory::details
{
  template<typename T, std::uint32_t N>
  class SlotArray
  {
  public:
    [[nodiscard]] constexpr T&       operator[](SlotIndex i)       noexcept { return a_[i.value()]; }
    [[nodiscard]] constexpr const T& operator[](SlotIndex i) const noexcept { return a_[i.value()]; }
  
  private:
    std::array<T, N> a_;
  };
}

#endif