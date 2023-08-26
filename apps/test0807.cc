//
// Created by ASUS on 2023/8/7.
//
#include "iostream"

void zzz( std::string* pString );
int main()
{
  std::string s = "abc";
  for ( const auto& item : s ) {
    std::cout << &item << std::endl;
  }
  std::cout << s[1] << std::endl;
  std::cout << &s[1] << std::endl;

  zzz( &s );
  for ( const auto& item : s ) {
    std::cout << &item << std::endl;
  }
}
void zzz( std::string* pString )
{
  pString->append( "xdf" );
}
