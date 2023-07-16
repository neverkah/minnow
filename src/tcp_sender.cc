#include "tcp_sender.hh"
#include "iostream"
#include "random.hh"
#include "tcp_config.hh"
#include <cstdlib>
#include <random>
#include <utility>

using namespace std;

/* TCPSender constructor (uses a random ISN if none given) */
TCPSender::TCPSender( uint64_t initial_RTO_ms, optional<Wrap32> fixed_isn )
  : isn_( fixed_isn.value_or( Wrap32 { random_device()() } ) ), initial_RTO_ms_( initial_RTO_ms )
{}

uint64_t TCPSender::sequence_numbers_in_flight() const
{
  // Your code here.
  return un_ack_byte_count_;
}

uint64_t TCPSender::consecutive_retransmissions() const
{
  // Your code here.
  return {};
}

optional<TCPSenderMessage> TCPSender::maybe_send()
{
  TCPSenderMessage sender_message {};
  for ( auto& item : un_ack_deque_ ) {
    if ( timeout == 0 || !item.sended ) {
      sender_message.SYN = item.SYN;
      sender_message.FIN = item.FIN;
      sender_message.seqno = item.seqno;
      sender_message.payload = item.buffer;
      item.sended = true;

      if ( timeout == 0 ) {
        if ( receiver_message_.window_size > 0 ) {
          re_send_count_++;
          RTO_ms_ *= 2;
          timeout = RTO_ms_;
        } else {
          timeout = RTO_ms_;
        }
      }
      return sender_message;
    }
  }

  return {};
}

void TCPSender::push( Reader& outbound_stream )
{

  if ( bytes_send_count_ >= 1 + outbound_stream.writer().bytes_pushed() + outbound_stream.writer().is_closed() ) {
    return;
  }
  // Your code here.
  const string_view peek_str = outbound_stream.peek();
  //  if ( outbound_stream.is_finished() && peek_str.empty() ) {
  //    if ( bytes_send_count_ == 0 ) {
  //      un_ack_deque_.push_back( { isn_, {}, true, true } );
  //      bytes_send_count_ += 2;
  //      un_ack_byte_count_ += 2;
  //    } else {
  //      un_ack_deque_.push_back( { Wrap32::wrap( bytes_send_count_, isn_ ), {}, false, true } );
  //      bytes_send_count_++;
  //      un_ack_byte_count_++;
  //    }
  //    return;
  //  }
  u_int32_t available_size
    = receiver_message_.window_size >= un_ack_byte_count_ ? receiver_message_.window_size - un_ack_byte_count_ : 0;
  u_int32_t stream_len
    = peek_str.size() + ( bytes_send_count_ == 0 ? 1 : 0 ) + outbound_stream.writer().is_closed();
  if ( receiver_message_.window_size == un_ack_byte_count_ ) {
    if ( stream_len > 0 ) {
      if ( bytes_send_count_ == 0 ) {
        un_ack_deque_.push_back( { isn_, {}, true, outbound_stream.is_finished() } );
      } else {
        size_t pop_len = !peek_str.empty() ? 1 : 0;
        outbound_stream.pop( pop_len );
        un_ack_deque_.push_back( { Wrap32::wrap( bytes_send_count_, isn_ ),
                                   { peek_str.data(), pop_len },
                                   false,
                                   pop_len == 0 && outbound_stream.is_finished() } );
      }
      bytes_send_count_++;
      un_ack_byte_count_++;
    }
    return;
  }

  size_t len = stream_len > available_size ? available_size : stream_len;
  if ( len > 0 ) {
    size_t const seg_count = len % TCPConfig::MAX_PAYLOAD_SIZE == 0 ? len / TCPConfig::MAX_PAYLOAD_SIZE
                                                                    : len / TCPConfig::MAX_PAYLOAD_SIZE + 1;
    if ( bytes_send_count_ == 0 ) {

      for ( unsigned int i = 0; i < seg_count; i++ ) {
        size_t const offset = i * TCPConfig::MAX_PAYLOAD_SIZE;
        const size_t expected_size = min( TCPConfig::MAX_PAYLOAD_SIZE, len - offset );
        if ( i == 0 ) {
          size_t pop_len = min( peek_str.length(), expected_size - 1 );
          outbound_stream.pop( pop_len );
          un_ack_deque_.push_back( { isn_,
                                     { peek_str.data(), pop_len },
                                     true,
                                     pop_len < expected_size - 1 && outbound_stream.is_finished() } );
        } else {
          size_t pop_len = min( peek_str.length(), expected_size );
          outbound_stream.pop( expected_size );
          un_ack_deque_.push_back( { isn_ + offset,
                                     { &peek_str[offset - 1], pop_len },
                                     false,
                                     pop_len < expected_size && outbound_stream.is_finished() } );
        }
      }

    } else {
      Wrap32 next_no = Wrap32::wrap( bytes_send_count_, isn_ );
      for ( unsigned int i = 0; i < seg_count; i++ ) {
        size_t const offset = i * TCPConfig::MAX_PAYLOAD_SIZE;
        const size_t expected_size = min( TCPConfig::MAX_PAYLOAD_SIZE, len - offset );
        size_t pop_len = min( peek_str.length(), expected_size );
        outbound_stream.pop( pop_len );
        un_ack_deque_.push_back( { next_no + offset,
                                   { &peek_str[offset], pop_len },
                                   false,
                                   pop_len < expected_size && outbound_stream.is_finished() } );
      }
    }

    bytes_send_count_ += len;
    un_ack_byte_count_ += len;
  }
}

TCPSenderMessage TCPSender::send_empty_message() const
{
  // Your code here.
  TCPSenderMessage senderMessage {};
  senderMessage.seqno = Wrap32::wrap( bytes_send_count_, isn_ );
  return senderMessage;
}

void TCPSender::receive( const TCPReceiverMessage& msg )
{
  // Your code here.

  if ( msg.ackno.has_value() ) {

    u_int64_t abs_ackno = msg.ackno.value().unwrap( isn_, bytes_send_count_ );
    u_int64_t abs_next_send_no = bytes_send_count_;
    if ( abs_ackno > abs_next_send_no ) {
      return;
    }
    if ( un_ack_deque_.size() > 0 ) {
      send_seg un_ack_first = un_ack_deque_.front();
      uint64_t abs_un_ack_first_tail = ( un_ack_first.seqno + un_ack_first.buffer.length() + un_ack_first.SYN )
                                         .unwrap( isn_, bytes_send_count_ );
      if ( abs_ackno >= abs_un_ack_first_tail ) {
        RTO_ms_ = initial_RTO_ms_;
        timeout = RTO_ms_;

        re_send_count_ = 0;
      }
      auto it = un_ack_deque_.begin();
      for ( ; it <= un_ack_deque_.end(); it++ ) {
        Wrap32 seg_tail = it->seqno + ( it->SYN + it->buffer.length() + it->FIN );
        u_int64_t const abs_seg_tail = seg_tail.unwrap( isn_, bytes_send_count_ );
        if ( abs_seg_tail <= abs_ackno ) {
          un_ack_byte_count_ -= ( it->buffer.length() + it->SYN + it->FIN );
        } else {
          break;
        }
      }
      un_ack_deque_.erase( un_ack_deque_.begin(), it );
    }

    receiver_message_ = msg;
  }
}

void TCPSender::tick( const size_t ms_since_last_tick )
{
  timeout = timeout > ms_since_last_tick ? timeout - ms_since_last_tick : 0;
}