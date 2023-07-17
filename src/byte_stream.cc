#include "byte_stream.hh"
#include <iostream>
#include <stdexcept>

using namespace std;

ByteStream::ByteStream( uint64_t capacity ) : capacity_( capacity ) {}

void Writer::push( string data )
{
  // Your code here.
  //

  size_t const len = min( available_capacity(), data.length() );
  if ( len == 0 ) {
    return;
  }
  buffer_.append( data, 0, len );
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
  return this->capacity_ - ( buffer_.length() - begin_idx_ );
}

uint64_t Writer::bytes_pushed() const
{
  // Your code here.
  return bytes_pushed_count_;
}

string_view Reader::peek() const
{
  // Your code here.

  return { &buffer_[begin_idx_], bytes_buffered() };
}

bool Reader::is_finished() const
{
  // Your code here.
  return bytes_buffered() == 0 && input_ended_flag_;
}

bool Reader::has_error() const
{
  // Your code here.
  return error_flag_;
}

void Reader::pop( uint64_t len )
{
  // Your code here.
  uint64_t const actual_len = min( len, bytes_buffered() );
  begin_idx_ += actual_len;
  bytes_popped_count_ += actual_len;
  if ( begin_idx_ > capacity_ / 2 ) {
    buffer_.erase( 0, begin_idx_ );
    begin_idx_ = 0;
  }
}

uint64_t Reader::bytes_buffered() const
{
  // Your code here.
  return buffer_.size() - begin_idx_;
}

uint64_t Reader::bytes_popped() const
{
  // Your code here.
  return bytes_popped_count_;
}
