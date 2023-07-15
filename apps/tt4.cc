//
// Created by ASUS on 2023/7/11.
//
#include "buffer.hh"
#include "iostream"
#include "tcp_receiver_message.hh"
using namespace std;

int main()
{
  TCPReceiverMessage tcpReceiverMessage = {};
  bool r = tcpReceiverMessage.ackno.has_value();
  cout << r << endl;

  Buffer buffer {"xxx"};
  string s = buffer;
  s.append( "abc" );
  cout << buffer.operator std::string&() << endl;
  cout << s << endl;
}
