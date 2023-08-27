//
// Created by ASUS on 2023/6/23.
//
#include <iostream>
std::string_view return_string_view();
using namespace std;
int main()
{
  string got;
  auto peeked = return_string_view();
  cout << &peeked << endl;
  got += peeked;
  cout << got << endl;

  return 0;
}
string_view return_string_view()
{
  string const s = string().assign( 2, 'x' );
  string_view const sv = s;
  const char* c = s.data();
  cout << &s << endl;
  cout << static_cast<const void*>( c ) << endl;
  return sv;
}