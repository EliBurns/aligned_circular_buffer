/*!
 * @creator eli 
 * @date 1/4/18
 */

#ifndef ALIGNED_CIRCULAR_BUFFER_ALIGNED_CIRCULAR_BUFFER_HPP
#define ALIGNED_CIRCULAR_BUFFER_ALIGNED_CIRCULAR_BUFFER_HPP

#include <type_traits>
#include <cassert>
#include <cstdint>


template <class T, std::size_t N>
class aligned_circular_buffer
/**
 * For use with single producer, single consumer communication. Parameter of
 * type T must have default constructor.
 */
{
  using aligned_size_t = std::uint64_t;
  using aligned_storage_t = std::aligned_storage<sizeof(T), alignof(T)>::type;

public:

  aligned_circular_buffer()
  /**
   * Default constructor ~ asserts that buffer size is equal to a power of two
   * at compile time. Fills contiguous memory with default constructed members
   * of type T.
   */
  {
    assert((!(N & (N-1))) && N);
    while (member_size_ < N)
      emplace_back(T());
  }

private:




  aligned_storage_t data_[N];
  aligned_size_t member_size_{0};
  aligned_size_t read_{0};
  aligned_size_t write_{0};
  aligned_size_t capacity_{N};
};


#endif //ALIGNED_CIRCULAR_BUFFER_ALIGNED_CIRCULAR_BUFFER_HPP
