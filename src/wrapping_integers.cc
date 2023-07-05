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

  u_int64_t const offset = raw_value_ < zero_point.raw_value_ ? ( 1UL << 32 ) - zero_point.raw_value_ + raw_value_
                                                              : raw_value_ - zero_point.raw_value_;

  u_int64_t const index = ( checkpoint & 0xFFFFFFFF00000000 ) + offset;
  u_int64_t const left_index = index >= 1UL << 32 ? index - ( 1UL << 32 ) : index;
  u_int64_t const right_index = index + ( 1UL << 32 );
  u_int64_t const index_arr[] = { left_index, index, right_index };
  u_int64_t dis_arr[3] = {};
  for ( int i = 0; i < 3; ++i ) {
    dis_arr[i] = cal_abs( index_arr[i], checkpoint );
  }
  u_int64_t min_dis = min( dis_arr[0], min( dis_arr[1], dis_arr[2] ) );
  if ( min_dis == dis_arr[0] ) {
    return left_index;
  }
  if ( min_dis == dis_arr[1] ) {
    return index;
  }
  return right_index;
}
u_int64_t Wrap32::cal_abs( u_int64_t x1, u_int64_t x2 ) const
{
  if ( x1 > x2 ) {
    return x1 - x2;
  }
  return x2 - x1;
}
