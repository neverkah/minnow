//
// Created by ASUS on 2023/7/15.
//
#include "iostream"
#include "queue"

using namespace std;
int main()
{

  deque<string> deque1 = {};
//  deque1.push_back( "abc" );
  bool b = deque1.begin() == deque1.end();
  cout << b << endl;
}