//
// Created by huangzhongjie1 on 2023/7/5.
//
#include "iostream"
#include "reassembler.hh"
#include "set"
using namespace std;

struct BlockNode
{
  size_t first_index_ = 0;
  size_t last_index_ = 0;
  std::string data_ = {};

  BlockNode() = default;

  BlockNode( size_t firstIndex, const std::string& data )
    : first_index_( firstIndex ), last_index_( firstIndex + data.length() - 1 ), data_( data )
  {}

  bool operator<( const BlockNode t ) const { return this->first_index_ > t.first_index_; }
};
int main()
{
  set<BlockNode> blocks = {};
  blocks.insert( BlockNode( 8, "abc" ) );
  blocks.insert( BlockNode( 5, "def" ) );
  auto it = blocks.lower_bound( { 6, "xxx" } );
  const BlockNode node = *it;
  cout << &( *it ) << endl;
  cout << &node << endl;
}