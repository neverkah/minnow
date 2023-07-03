#pragma once

#include "byte_stream.hh"

#include <set>
#include <string>

class Reassembler {
protected:
    struct block_node {
        size_t first_index_ = 0;
        size_t last_index_ = 0;
        std::string data_ = {};

        block_node() = default;

        block_node(size_t firstIndex, const std::string &data)
                : first_index_(firstIndex), last_index_(firstIndex + data.length() - 1), data_(data) {}


        bool operator<(const block_node t) const { return this->first_index_ > t.first_index_; }
    };

    std::set<block_node> blocks_ = {};

    uint64_t total_len_ = 0;
    bool last_piece_appeared_ = false;
    uint64_t bytes_pending_count_ = 0;

public:
  /*
   * Insert a new substring to be reassembled into a ByteStream.
   *   `first_index`: the index of the first byte of the substring
   *   `data`: the substring itself
   *   `is_last_substring`: this substring represents the end of the stream
   *   `output`: a mutable reference to the Writer
   *
   * The Reassembler's job is to reassemble the indexed substrings (possibly out-of-order
   * and possibly overlapping) back into the original ByteStream. As soon as the Reassembler
   * learns the next byte in the stream, it should write it to the output.
   *
   * If the Reassembler learns about bytes that fit within the stream's available capacity
   * but can't yet be written (because earlier bytes remain unknown), it should store them
   * internally until the gaps are filled in.
   *
   * The Reassembler should discard any bytes that lie beyond the stream's available capacity
   * (i.e., bytes that couldn't be written even if earlier gaps get filled in).
   *
   * The Reassembler should close the stream after writing the last byte.
   */
  void insert( uint64_t first_index, std::string data, bool is_last_substring, Writer& output );

    // How many bytes are stored in the Reassembler itself?
    uint64_t bytes_pending() const;


    void
    merge_data(Reassembler::block_node left_node, Reassembler::block_node &node, Reassembler::block_node right_node);
};
