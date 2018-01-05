/*!
 * @creator eli 
 * @date 1/4/18
 */

#ifndef ALIGNED_CIRCULAR_BUFFER_ALIGNED_CIRCULAR_BUFFER_HPP
#define ALIGNED_CIRCULAR_BUFFER_ALIGNED_CIRCULAR_BUFFER_HPP

#include <type_traits>
#include <cassert>
#include <cstdint>
#include <new>


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
    : member_size_{0}, read_{0}, write_{0}, capacity_{N}
  {
    assert((!(N & (N-1))) && N);
    while (member_size_ < N)
      emplace_back(T());
  }

  ~aligned_circular_buffer()
  /**
   * Frees all objects of type T from contiguous memory locations.
   */
  {
    for (std::size_t position = 0; position < capacity_; ++position)
      reinterpret_cast<T*>(data_ + position)->~T();
  }


private:
  void emplace_back(T element)
  /**
   * Create an object within aligned storage.
   * @param element Element to store of type T.
   */
  {
    if (member_size_ > N)
      throw std::bad_alloc{};

    new(data_ + member_size_) T(element);
    ++member_size_;
  }

  T& operator[](aligned_size_t position)
  /**
   * Random access for use within public interface functions.
   * @param position Position at which to randomly access object of type T.
   * @return Reference to object of type T.
   */
  {
    return *reinterpret_cast<T*>(data_ + position);
  }

  aligned_storage_t data_[N]; // contiguous storage for N objects of type T.
  aligned_size_t member_size_;
  aligned_size_t read_;
  aligned_size_t write_;
  aligned_size_t capacity_;
};


#endif //ALIGNED_CIRCULAR_BUFFER_ALIGNED_CIRCULAR_BUFFER_HPP
