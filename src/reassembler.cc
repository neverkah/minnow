#include "reassembler.hh"

using namespace std;

void Reassembler::insert( uint64_t first_index, string data, bool is_last_substring, Writer& output )
{

  if ( is_last_substring ) {
    total_len_ = first_index + data.size();
    last_piece_appeared_ = true;
  }

  if ( data.empty() ) {
    if ( last_piece_appeared_ && output.bytes_pushed() == total_len_ ) {
      output.close();
    }
    return;
  }

  if ( first_index + data.length() <= output.bytes_pushed() ) {
    return;
  }

  if ( first_index < output.bytes_pushed() ) {
    data.erase( 0, output.bytes_pushed() - first_index );
    first_index = output.bytes_pushed();
  }
  if ( first_index + data.length() - output.bytes_pushed() > output.available_capacity() ) {

    if ( first_index < output.bytes_pushed() + output.available_capacity() ) {
      data.erase( output.bytes_pushed() + output.available_capacity() - first_index );
    } else {
      return;
    }
  }

  for ( uint64_t i = first_index; i < first_index + data.length(); i++ ) {
    if ( filled_.size() > i && filled_[i] ) {
      filled_[i] = false;
      bytes_pending_count_--;
    }
  }

  if ( output.available_capacity() < data.length() + bytes_pending() ) {
    return;
  }

  // 与byteBuffer连上了
  if ( first_index <= output.bytes_pushed() ) {
    // 先处理自个儿

    output.push( data );
    if ( output.bytes_pushed() == total_len_ ) {
      output.close();
      return;
    }

    // 再看有没有连锁反应，导致相邻的数据片也连上的
    uint64_t end_idx = output.bytes_pushed();
    bool successor_found = false;
    for ( ; end_idx < un_buffered_.length() && filled_[end_idx]; end_idx++ ) {}
    successor_found = end_idx > output.bytes_pushed();

    if ( successor_found ) {
      string const successor_buffered_bytes
        = { &un_buffered_[output.bytes_pushed()], end_idx - output.bytes_pushed() };

      output.push( successor_buffered_bytes );
      bytes_pending_count_ -= successor_buffered_bytes.length();
      if ( output.bytes_pushed() == total_len_ ) {
        output.close();
        return;
      }
    }

  } else {
    if ( first_index + data.length() <= un_buffered_.length() ) {

    } else {
      un_buffered_.resize( first_index + data.length() );
      filled_.resize( first_index + data.length(), false );
    }
    for ( u_int64_t i = first_index; i < first_index + data.length(); i++ ) {
      un_buffered_[i] = data[i - first_index];
      bytes_pending_count_++;
      filled_[i] = true;
    }
  }
}

uint64_t Reassembler::bytes_pending() const
{
  // Your code here.
  return bytes_pending_count_;
}
