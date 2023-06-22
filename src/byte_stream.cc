#include "byte_stream.hh"
#include <iostream>
#include <stdexcept>

using namespace std;

ByteStream::ByteStream( uint64_t capacity ) : capacity_( capacity ) {}

void Writer::push( string data )
{
  // Your code here.
  //


  size_t len = min( available_capacity(), data.length() );
  for ( size_t i = 0; i < len; i++ ) {
    this->buffer_.push_back( data[i] );
  }
  bytes_pushed_count_ += len;
}

void Writer::close()
{
  // Your code here.
  input_ended_flag_ = true;
}

void Writer::set_error()
{
  // Your code here.
  error_flag_ = true;
}

bool Writer::is_closed() const
{
  // Your code here.
  return input_ended_flag_;
}

uint64_t Writer::available_capacity() const
{
  // Your code here.
  return this->capacity_ - buffer_.size();
}

uint64_t Writer::bytes_pushed() const
{
  // Your code here.
  return bytes_pushed_count_;
}

string_view Reader::peek() const
{
  // Your code here.
  if ( !buffer_.empty() ) {
    //    char c = buffer_.front();
    //
    //    char* xx = { &c };
    //    //    string const s = string().append( 1, c );
    //    string_view peekResult = xx;
    //    return peekResult;
    string const s = {buffer_.front()};
    return  {s};
  }
  return {};
}

bool Reader::is_finished() const
{
  // Your code here.
  return buffer_.empty() && input_ended_flag_;
}

bool Reader::has_error() const
{
  // Your code here.
  return error_flag_;
}

void Reader::pop( uint64_t len )
{
  // Your code here.
  for ( uint64_t i = 0; i < len; ++i ) {
    buffer_.pop_front();
  }
  bytes_popped_count_ += len;
}

uint64_t Reader::bytes_buffered() const
{
  // Your code here.
  return buffer_.size();
}

uint64_t Reader::bytes_popped() const
{
  // Your code here.
  return bytes_popped_count_;
}
