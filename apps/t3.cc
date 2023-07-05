//
// Created by ASUS on 2023/6/23.
//
#include <iostream>
using namespace std;

int main()
{
  string ss = "bdfsdsdffd";
  ss.resize( 100 );
  cout << &ss << endl;
  const char* xxp = ss.c_str();
  cout << xxp << endl;
  char c_arr[6];
  cout << sizeof( c_arr ) << endl;
  cout << c_arr << endl;
  printf( "%p\n", static_cast<void*>( c_arr ) );
  printf( "%p\n", static_cast<void*>( &c_arr ) );
  printf( "%p", static_cast<void*>( &c_arr[0] ) );
}
