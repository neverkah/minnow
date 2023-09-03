//
// Created by ASUS on 2023/9/2.
//

#include "iostream"
#include "router.hh"
#include <bitset>
int main()
{
  uint32_t x = ( ( 1 << 16 ) - 1 );
  std::cout << UINT16_MAX << std::endl;
  std::cout << x << std::endl;

  Router router {};

  uint32_t m = ( 172 << 8 ) + 16;
  uint32_t n = ( 192 << 8 ) + 168;
  std::cout << m << std::endl;
  std::cout << n << std::endl;
  std::cout << ( m & n ) << std::endl;
  bitset<32> bsm { m };
  bitset<32> bsn { n };
  bitset<32> bsmn { m & n };

  std::cout << bsm << std::endl;
  std::cout << bsn << std::endl;
  std::cout << bsmn << std::endl;

  bitset<4> bbb { 32 };
  std::cout << bbb << std::endl;
  uint8_t p = 16;
  std::cout << ( ~p + 1 ) << std::endl;
  uint32_t v = 0xffffffff << 31;
  std::cout << ( v << 1 ) << std::endl;
}