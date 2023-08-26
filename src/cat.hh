//
// Created by ASUS on 2023/8/7.
//
#ifndef MINNOW_KKK_HH
#define MINNOW_KKK_HH
#include <string>
using namespace std;

class Cat
{
private:
  int age_;
  string name_;

public:
  Cat( int age, string name );

  bool operator==( const Cat& t ) const { return age_ == t.age_ && name_ == t.name_; }
  bool operator!=( const Cat& other ) const { return not operator==( other ); }
};

#endif // MINNOW_KKK_HH
