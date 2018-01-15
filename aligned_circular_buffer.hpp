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
#include <atomic>


template <class T, std::size_t N>
class aligned_circular_buffer
/**
 * For use with single producer, single consumer communication. Parameter of
 * type T must have default constructor.
 */
{
  using aligned_size_t = std::uint64_t;
  using aligned_storage_t =
    typename std::aligned_storage<sizeof(T), alignof(T)>::type;

private:
  aligned_storage_t data_[N]; // contiguous storage for N objects of type T.
  std::atomic<aligned_size_t> member_size_;
  std::atomic<aligned_size_t> read_;
  std::atomic<aligned_size_t> write_;
  std::atomic<aligned_size_t> capacity_;


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

  const aligned_size_t mask(aligned_size_t value) const
  /**
   * Masks the value to index within the capacity of the aligned contiguous data.
   * @param value Value to mask.
   * @return Masked value.
   */
  {
    return (value & (capacity_ - 1));
  }

public:

  aligned_circular_buffer()
  /**
   * Default constructor ~ asserts that buffer size is equal to a power of two
   * at compile time. Initializes contiguous memory with default constructed
   * members of type T.
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

  /*
   * Prevent buffer from being copied or moved after being constructed.
   */
  aligned_circular_buffer(aligned_circular_buffer&) = delete;
  aligned_circular_buffer& operator=(aligned_circular_buffer&) = delete;
  aligned_circular_buffer(aligned_circular_buffer&&) = delete;
  aligned_circular_buffer& operator=(aligned_circular_buffer&&) = delete;

  aligned_size_t capacity() const { return capacity_; }
  void clear() { read_ = write_ = 0; }
  aligned_size_t size() const { return write_ - read_; }
  bool empty() const { return read_ == write_; }
  bool full() const { return size() == capacity_; }

  T pop_front()
  /**
   * Advance read iterator and return object at masked position.
   * @return Object of type T.
   */
  {
    return operator[](mask(++read_));
  }

  void push_back(const T& element)
  /**
   * Takes element, checks if the circular array buffer is full and then
   * adjusts read iterator if necessary before placing object in buffer.
   * @param element Object of type T.
   */
  {
    if (full())
      ++read_;
    operator[](mask(++write_)) = element;
  }
};


#endif //ALIGNED_CIRCULAR_BUFFER_ALIGNED_CIRCULAR_BUFFER_HPP
