#ifndef MARKET_INGESTOR_MEMORY_CONCEPTS_HPP_
#define MARKET_INGESTOR_MEMORY_CONCEPTS_HPP_

#include <concepts>
#include <type_traits>

#include <market_ingestor/memory/types.hpp>

namespace market_ingestor::memory
{
  template<typename T>
  concept TriviallyPoolable = std::is_trivially_copyable_v<T>
                           && std::is_default_constructible_v<T>;


  template<typename T>
  concept HasParamsType = requires { typename T::params_type; };

  
  template<typename T>
  concept InPlaceReinitializable = 
    HasParamsType<T>
    && requires(T& t, const typename T::params_type& p) {
         { t.reinitialize(p) } noexcept;
       };

       
  template<typename T>
  concept PoolStorable = std::same_as<typename T::pool_category, poolable_tag>
                      && TriviallyPoolable<T> 
                      && InPlaceReinitializable<T>;
}

#endif