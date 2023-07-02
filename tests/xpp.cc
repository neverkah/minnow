//
// Created by ASUS on 2023/7/2.
//

#include "byte_stream.hh"
#include "iostream"
#include "reassembler.hh"
using namespace std;
int main()
{

  ByteStream byteStream = ByteStream( 12 );
  Reassembler reassembler = {};

  string s1 = {};
  s1.resize( 3 );
  s1[0] = -40;
  s1[1] = -33;
  s1[2] = -15;
  reassembler.insert( 0, s1, false, byteStream.writer() );

  string s2 = {};
  s2.resize( 5 );
  s2[0] = -40;
  s2[1] = -33;
  s2[2] = -15;
  s2[3] = -22;
  s2[4] = -23;
  reassembler.insert( 0, s2, false, byteStream.writer() );

  string s3 = {};
  s3.resize( 6 );
  s3[0] = -40;
  s3[1] = -33;
  s3[2] = -15;
  s3[3] = -22;
  s3[4] = -23;
  s3[5] = 0;
  reassembler.insert( 0, s3, true, byteStream.writer() );
}