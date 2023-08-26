//
// Created by ASUS on 2023/8/5.
//
#include "ethernet_header.hh"
#include "iostream"
#include "xaddress.hh"
#include <map>
#include <optional>
#include <queue>
#include <set>

using namespace std;

int main()
{
  std::map<XAddress, EthernetAddress> df {};
  XAddress address = XAddress( {}, 0 );
  df.insert( { address, ETHERNET_BROADCAST } );
  bool not_found = df.find( address ) == df.end();
  XAddress::Raw raw {};
  sockaddr* xx = raw;
  sockaddr* yy = raw.operator sockaddr*();
  cout << xx << endl;
  cout << yy << endl;
  cout << not_found << endl;
}