//
// Created by ASUS on 2023/7/15.
//
#include "iostream"
#include "queue"
#include <array>
#include <map>

using namespace std;
int main()
{

  map<string, int> strMap; // 以string为键值，以int为实值
  strMap[string( "jjhou" )] = 1;
  strMap[string( "jerry" )] = 2;
  strMap[string( "jason" )] = 3;
  strMap[string( "jimmy" )] = 4;
  cout << strMap[string( "xxxx" )] << endl;
  bool xxd = strMap.find( "xxxx" ) == strMap.end();
  cout << "xxd " << xxd << endl;
  cout << strMap.find( "jerry" )->second << endl;
  bool xx = strMap.find( "jerry" ) == strMap.end();
  cout << xx << endl;

  pair<string, int> value( string( "david" ), 5 );
  strMap.insert( value ); // 插入新元素

  map<string, int>::iterator strmap_iter = strMap.begin();
  for ( ; strmap_iter != strMap.end(); strmap_iter++ ) {
    cout << strmap_iter->first << ' ' << strmap_iter->second << endl;
  }
  cout << endl;

  int number = strMap[string( "jjhou" )];
  cout << number << endl;
  cout << endl;

  // 查找元素
  map<string, int>::iterator iter1;
  // 面对关联式容器，应该使用其所提供的find函数来搜索元素，会比使用STL算法find()更有效率。因为STL算法find()只是循环搜索。
  iter1 = strMap.find( string( "mchen" ) );
  if ( iter1 == strMap.end() )
    cout << "mchen no fount" << endl;
  cout << endl;

  iter1 = strMap.find( string( "jerry" ) );
  if ( iter1 != strMap.end() )
    cout << "jerry fount" << endl;
  cout << endl;

  // 修改实值，键值不可修改
  iter1->second = 9; // 可以通过map迭代器修改“value”(not key)
  int number1 = strMap[string( "jerry" )];
  cout << number1 << endl;

  // 删除元素
  map<string, int>::iterator strmap_iter1 = strMap.begin();
  for ( ; strmap_iter1 != strMap.end(); strmap_iter1++ ) {
    cout << strmap_iter1->first << ' ' << strmap_iter1->second << endl;
  }
  cout << endl;

  strMap.erase( iter1 );             // 删除一个条目
  strMap.erase( string( "jason" ) ); // 根据键值删除

  map<string, int>::iterator strmap_iter2 = strMap.begin();
  for ( ; strmap_iter2 != strMap.end(); strmap_iter2++ ) {
    cout << strmap_iter2->first << ' ' << strmap_iter2->second << endl;
  }
}