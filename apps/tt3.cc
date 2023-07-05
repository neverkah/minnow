//
// Created by huangzhongjie1 on 2023/7/5.
//
#include "iostream"
#include "reassembler.hh"
#include "set"
using namespace std;

struct block_node
{
  size_t first_index_ = 0;
  size_t last_index_ = 0;
  std::string data_ = {};

  block_node() = default;

  block_node( size_t firstIndex, const std::string& data )
    : first_index_( firstIndex ), last_index_( firstIndex + data.length() - 1 ), data_( data )
  {}

  bool operator<( const block_node t ) const { return this->first_index_ > t.first_index_; }
};
int main()
{
  set<block_node> blocks = {};
  blocks.insert( block_node( 8, "abc" ) );
  blocks.insert( block_node( 5, "def" ) );
  auto it = blocks.lower_bound( { 6, "xxx" } );
  const block_node node = *it;
  cout << &( *it ) << endl;
  cout << &node << endl;
}