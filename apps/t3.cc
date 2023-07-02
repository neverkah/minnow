//
// Created by ASUS on 2023/6/23.
//
#include <iostream>
using namespace std;

int main()
{
  string buffer_ = "abcdef";
  buffer_.resize( 100 );

  buffer_[10] = 'z';

  cout << buffer_ << endl;

  //  string_view sv = { &buffer_[3], 3 };

  //  buffer_.erase( 0, 2 );
}
