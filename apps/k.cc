//
// Created by ASUS on 2023/7/15.
//
#include "ethernet_frame.hh"
#include "iostream"

using namespace std;

int main()
{
  EthernetFrame ef = {};
  //  ef.header.src = { 2, 3, 4, 5, 6, 7 };
  //  ef.header.dst = { 8, 9, 10, 11, 12, 13 };
  vector<Buffer> buffers = {};
  buffers.emplace_back( "abcdef" );
  buffers.emplace_back( "ghijkl" );

  string type = {};
  type.push_back( 8 );
  type.push_back( 0 );
  buffers.emplace_back( type );
  buffers.emplace_back( "haliluya" );
  buffers.emplace_back( "hello world" );
  buffers.emplace_back( "fuck world" );
  Parser p { buffers };
  ef.parse( p );
  cout << "parse succ" << endl;
  Serializer s;
  ef.serialize( s );
  cout << "serialize succ" << endl;
}
