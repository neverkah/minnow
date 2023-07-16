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

  deque1.erase( deque1.begin(), deque1.begin() + 2 );

  for ( const auto& item : deque1 ) {
    cout << item << endl;
  }
}