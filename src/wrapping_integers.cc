#include "wrapping_integers.hh"
#include <cstdlib>

using namespace std;

Wrap32 Wrap32::wrap( uint64_t n, Wrap32 zero_point )
{
  // Your code here.
  return zero_point + ( n & UINT32_MAX );
}

uint64_t Wrap32::unwrap( Wrap32 zero_point, uint64_t checkpoint ) const
{
  u_int64_t const offset = raw_value_ < zero_point.raw_value_ ? ( 1UL << 32 ) - zero_point.raw_value_ + raw_value_
                                                              : raw_value_ - zero_point.raw_value_;
  uint64_t const checkpoint_relative = checkpoint & UINT32_MAX;
  u_int64_t left_index = 0;
  u_int64_t right_index = 0;
  if ( checkpoint_relative > offset ) {
    left_index = ( checkpoint & 0xFFFFFFFF00000000 ) + offset;
    right_index = left_index + ( 1UL << 32 );
  } else {
    right_index = ( checkpoint & 0xFFFFFFFF00000000 ) + offset;
    left_index = right_index >= ( 1UL << 32 ) ? right_index - ( 1UL << 32 ) : right_index;
  }
  if ( checkpoint - left_index < right_index - checkpoint ) {
    return left_index;
  }
  return right_index;
}
