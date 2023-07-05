#include "wrapping_integers.hh"
#include "stdlib.h"
#include <algorithm>
#include <cstdlib>
#include <limits>

using namespace std;

Wrap32 Wrap32::wrap( uint64_t n, Wrap32 zero_point )
{
  // Your code here.
  return zero_point + ( n & UINT32_MAX );
}

uint64_t Wrap32::unwrap( Wrap32 zero_point, uint64_t checkpoint ) const
{

  u_int64_t const orig_index = raw_value_ < zero_point.raw_value_
                                 ? ( 1UL << 32 ) - zero_point.raw_value_ + raw_value_
                                 : raw_value_ - zero_point.raw_value_;
  u_int64_t ring_v = orig_index;
  if ( orig_index >= checkpoint ) {
    return orig_index;
  }
  for ( u_int64_t ring = 1; ring_v < checkpoint; ring++ ) {
    ring_v = ( ring << 32 ) + orig_index;
  }

  if ( ring_v - checkpoint < checkpoint - ( ring_v - ( 1UL << 32 ) ) ) {
    return ring_v;
  }
  return ring_v - ( 1UL << 32 );
}
