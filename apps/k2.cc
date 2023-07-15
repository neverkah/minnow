//
// Created by ASUS on 2023/7/15.
//
#include "iostream"
#include "queue"

using namespace std;
int main()
{

  deque<string> deque1 = {};
  deque1.push_back( "abc" );
  deque1.push_back( "def" );
  deque1.push_back( "kkk" );
  for ( const auto& item : deque1 ) {
    cout << item << endl;
  }

  for ( const auto& item : deque1 ) {
    cout << item << endl;
  }
  cout << "-----" << endl;
  string string1 = *( deque1.begin() + 1 );

  cout << string1 << endl;
  cout << "-----" << endl;
  for ( const auto& item : deque1 ) {
    cout << item << endl;
  }
}