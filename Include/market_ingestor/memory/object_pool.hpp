#ifndef MARKET_INGESTOR_MEMORY_OBJECTPOOL_HPP_
#define MARKET_INGESTOR_MEMORY_OBJECTPOOL_HPP_


#include <cstdint>
#include <cassert>

#include <market_ingestor/memory/concepts.hpp>
#include <market_ingestor/memory/types.hpp>
#include <market_ingestor/memory/free_list.hpp>
#include <market_ingestor/memory/handle.hpp>
#include <market_ingestor/memory/slot_array.hpp>

namespace market_ingestor::memory
{
  template<PoolStorable T, std::uint32_t N>
  class ObjectPool
  {
    using handle_type = Handle<N>;
    using params_type = typename T::params_type;

  public:
    constexpr ObjectPool() noexcept          = default;
    ObjectPool(const ObjectPool&)            = delete;
    ObjectPool& operator=(const ObjectPool&) = delete;

    [[nodiscard]] constexpr handle_type acquire(const params_type& p) noexcept
    {
      const SlotIndex i = free_.next();
      if(i.is_null()) [[unlikely]] return {};

      Slot& s = pool_[i];
      s.value.reinitialize(p);
      return handle_type(i, s.generation);
    }

    [[nodiscard]] constexpr bool release(handle_type h) noexcept
    { 
      if (h.is_null()) [[unlikely]] { return false; }
      
      Slot& s = pool_[h.index()];
      if (s.generation != h.generation()) [[unlikely]] { return false; }
      
      s.generation = s.generation.next();
      return free_.release(h.index());
    }

    [[nodiscard]] constexpr T* get(handle_type h) noexcept
    {
      if (h.is_null()) [[unlikely]] { return nullptr; }
      Slot& s = pool_[h.index()];
      return (s.generation == h.generation()) ? &s.value : nullptr;
    }

    [[nodiscard]] constexpr std::uint32_t available() const noexcept { return free_.available(); }
    [[nodiscard]] constexpr bool          exhausted() const noexcept { return free_.exhausted(); }
    [[nodiscard]] static constexpr std::uint32_t capacity() noexcept { return FreeList<N>::capacity(); }

  private:
    using generation_type = typename handle_type::generation_type;

    struct Slot { T value; generation_type generation; };

    static_assert(
      64 % sizeof(Slot) == 0 || sizeof(Slot) % 64 == 0,
      "sizeof(T) plus the generation stamp must divide or be a multiple of 64"
    );

    alignas(64) details::SlotArray<Slot, N> pool_;
                FreeList<N>                 free_;
  };
}

#endif