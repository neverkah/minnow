//
// Created by ASUS on 2023/6/18.
//
#include "byte_stream.hh"
#include <deque>
#include <iostream>
using namespace std;
int main()
{
  //  std::cout << "hello fuck  world\n";
  //  cout << "hello, fuck all\n";
  //  char c = 'x';
  //  char* cs = new char[1];
  //  cs[0] = c;
  //  string_view sv = cs;
  //  cout << cs[0] << " fffffuck";
  //  cout << sv << endl;
  //  return 0;
  //  string s = {};
  //  s.append( 5, 'x' );
  //  s.append( "fuck this world off" );
  //  cout << s << endl;
  //  int subscript = -1;
  //  std::deque<char> buffer_ = {};
  //  buffer_.push_back('x');
  //  buffer_.pop_front();
  //  char const c = buffer_.front();
  //  cout << c << endl;
  //  char x = 'x';
  //  string s = string().assign( 10, x );
  //  const char* cp = s.c_str();
  //  string_view stringView = string_view( cp );
  //
  //  cout << stringView << endl;
  //  std::shared_ptr<std::string> buffer_;
  //  char c= buffer_->front();

  string got;
  ByteStream bs = ByteStream( 2 );
  bs.writer().push( "cat" );
  auto peeked = bs.reader().peek();
  got = got.append( peeked );
  cout << got << endl;


  return 0;
}