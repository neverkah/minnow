//
// Created by ASUS on 2023/8/7.
//

#include <utility>

#include "cat.hh"
Cat::Cat(  int age, string  name ) : age_( age ), name_(std::move( name )) {}

//int main(){
//  Cat k {3,"ddfd"};
//}
