//
// Created by ASUS on 2023/8/27.
//
#include <utility>

#include "iostream"

class Family
{
private:
public:
  void setFamilyNo( const std::string& familyNo_ ) { Family::familyNo = familyNo_; }

public:
  std::string familyNo;

  explicit Family( std::string familyNo_ ) : familyNo( std::move( familyNo_ ) ) {}
};

class Cat
{
private:
  std::string name {};

  //    int age;
public:
  const Family& family;

  const std::string& getName() const { return name; }

  explicit Cat( const Family& family_ ) : family( family_ ) {}

  Cat( std::string name_, const Family& family_ ) : name( std::move( name_ ) ), family( family_ ) {}
};

int main()
{

  Family fff { "abc" };
  Cat cat { "a", fff };
  std::cout << sizeof( Cat ) << std::endl;
  std::cout << sizeof( Cat* ) << std::endl;
  std::cout << cat.getName() << std::endl;
  cat.getName();
  std::string familyNo = "fabc";
  Family f { familyNo };
  Cat catf = Cat { f };
  std::string newNo = "newf";
  f = Family { newNo };
  std::cout << catf.family.familyNo << std::endl;

  std::cout << cat.getName() << std::endl;
}
