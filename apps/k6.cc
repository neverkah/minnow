//
// Created by ASUS on 2023/8/5.
//
#include "address.hh"
#include "ethernet_header.hh"
#include "iostream"
#include <map>
#include <optional>
#include <queue>
#include <set>

using namespace std;

int main()
{
  //  Address address = Address( "192.168.0.1", 0 );
  //  Address address2 = Address::from_ipv4_numeric( address.ipv4_numeric() );
  //  bool xx = address == address2;
  //  cout << xx << endl;
  std::map<uint32_t, EthernetAddress> df {};
  Address address = Address( "192.168.0.1", 0 );
  Address address2 = Address( "192.168.0.1", 0 );
  bool xxx = address == address2;
  cout << xxx << endl;
  df.insert( { 123, ETHERNET_BROADCAST } );
  //  Address address2 = Address( "192.168.0.1", 0 );
  //  cout << df.find( address )->first.ipv4_numeric() << endl;
  auto x = df[123][0];
  bool yy = x == 0xff;
  cout << yy << endl;
}