/*!
 * @creator eli 
 * @date 1/4/18
 */

#include <iostream>
#include "catch.hpp"
#include "../aligned_circular_buffer.hpp"


/*
 * A stand in basic message with 8 fields, aligned to 64 bytes.
 */
using align_t = std::uint64_t;
struct alignas(64) test_message
{
  align_t field_0{0};
  align_t field_1{0};
  align_t field_2{0};
  align_t field_3{0};
  align_t field_4{0};
  align_t field_5{0};
  align_t field_6{0};
  align_t field_7{0};
};

/*
 * using a small power of two (as required) for testing purposes.
 */
const align_t test_buffer_size = 16;


TEST_CASE("verify initial values are correct after construction",
          "[initial_values]")
{
  aligned_circular_buffer<test_message, test_buffer_size> test_buffer;

  REQUIRE(test_buffer.capacity() == test_buffer_size);
  REQUIRE(test_buffer.size() == 0);
  REQUIRE(test_buffer.empty());
  REQUIRE_FALSE(test_buffer.full());
}
