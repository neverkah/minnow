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
  char pc = peeked.front();
  char* pcp = &pc;
  cout << &pcp << endl;
  got += pc;
  cout << got << endl;
  return 0;
}
string_view return_string_view()
{
  string const s = string().assign( 2, 'x' );
  auto sv = string_view( s.c_str() );
  char c = sv.front();
  char* cp = &c;
  cout << &cp << endl;
  return sv;
}