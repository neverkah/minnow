#include "reassembler.hh"
#include <set>

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
  // merge
  BlockNode node = BlockNode( first_index, data );
  BlockNode const base_node = { node.last_index_ + 1, {} };
  auto lower_it = blocks_.lower_bound( base_node );
  while ( lower_it != blocks_.end() && lower_it->last_index_ + 1 >= node.first_index_ ) {
    merge_data( node, *lower_it );
    bytes_pending_count_ -= lower_it->data_.length();
    blocks_.erase( lower_it );
    lower_it = blocks_.lower_bound( base_node );
  }
  if ( output.available_capacity() < node.data_.length() + bytes_pending() ) {
    return;
  }
  // 与byteBuffer连上了
  if ( node.first_index_ <= output.bytes_pushed() ) {
    // 先处理自个儿
    output.push( node.data_ );
    if ( output.bytes_pushed() == total_len_ ) {
      output.close();
      return;
    }
  } else {
    blocks_.insert( node );
    bytes_pending_count_ += node.data_.length();
  }
}

void Reassembler::merge_data( Reassembler::BlockNode& node, const Reassembler::BlockNode& killed_node )
{
  string left_str = {};
  string right_str = {};
  if ( killed_node.first_index_ < node.first_index_ ) {
    left_str = { killed_node.data_.data(), node.first_index_ - killed_node.first_index_ };
    node.first_index_ = killed_node.first_index_;
  }
  if ( killed_node.last_index_ > node.last_index_ ) {
    right_str = { &killed_node.data_[node.last_index_ + 1 - killed_node.first_index_],
                  killed_node.last_index_ - node.last_index_ };
    node.last_index_ = killed_node.last_index_;
  }
  node.data_ = left_str.append( node.data_ ).append( right_str );
}

uint64_t Reassembler::bytes_pending() const
{
  // Your code here.
  return bytes_pending_count_;
}
