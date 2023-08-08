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
  return re_send_count_;
}

optional<TCPSenderMessage> TCPSender::maybe_send()
{
  TCPSenderMessage sender_message {};
  for ( auto& item : un_ack_deque_ ) {
    if ( timeout == 0 || !item.sended ) {
      sender_message = item.message;
      item.sended = true;
      if ( timeout == 0 ) {
        if ( receiver_message_.ackno.has_value() && receiver_message_.window_size == 0 ) {
          timeout = RTO_ms_;
        } else {
          RTO_ms_ *= 2;
          timeout = RTO_ms_;
        }
        re_send_count_++;
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
  const string_view peek_str = outbound_stream.peek();

  size_t const stream_len
    = peek_str.size() + ( bytes_send_count_ == 0 ? 1 : 0 ) + outbound_stream.writer().is_closed();
  size_t available_size = 0;
  if ( receiver_message_.window_size == 0 && un_ack_byte_count_ == 0 ) {
    available_size = 1;
  } else {
    available_size = receiver_message_.window_size >= un_ack_byte_count_
                       ? receiver_message_.window_size - un_ack_byte_count_
                       : 0;
  }

  size_t const len = min( stream_len, available_size );
  if ( len == 0 ) {
    return;
  }
  unsigned int i = 0;
  unsigned int offset = 0;
  while ( offset < len ) {
    Wrap32 const next_no = Wrap32::wrap( bytes_send_count_, isn_ );
    bool const SYN = bytes_send_count_ == 0 && offset == 0;
    size_t pop_len = min( min( available_size - SYN, peek_str.length() ), TCPConfig::MAX_PAYLOAD_SIZE );
    outbound_stream.pop( pop_len );
    send_seg const seg = { { next_no,
                             SYN,
                             { { &peek_str[i * TCPConfig::MAX_PAYLOAD_SIZE], pop_len } },
                             ( offset + SYN + pop_len ) == ( len - 1 ) } };
    un_ack_deque_.push_back( seg );
    offset += seg.message.sequence_length();
    i++;
    bytes_send_count_ += seg.message.sequence_length();
    un_ack_byte_count_ += seg.message.sequence_length();
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
  receiver_message_ = msg;
  if ( msg.ackno.has_value() ) {
    u_int64_t const abs_ackno = msg.ackno.value().unwrap( isn_, bytes_send_count_ );
    u_int64_t const abs_next_send_no = bytes_send_count_;
    if ( abs_ackno > abs_next_send_no ) {
      return;
    }
    if ( un_ack_deque_.empty() ) {
      return;
    }

    auto it = un_ack_deque_.begin();
    while ( it < un_ack_deque_.end() ) {
      Wrap32 const seg_tail = it->message.seqno + it->message.sequence_length();
      u_int64_t const abs_seg_tail = seg_tail.unwrap( isn_, bytes_send_count_ );
      if ( abs_seg_tail <= abs_ackno ) {
        un_ack_byte_count_ -= it->message.sequence_length();
      } else {
        break;
      }
      it++;
    }
    if ( it > un_ack_deque_.begin() ) {
      RTO_ms_ = initial_RTO_ms_;
      timeout = RTO_ms_;
      re_send_count_ = 0;
    }
    un_ack_deque_.erase( un_ack_deque_.begin(), it );
  }
}

void TCPSender::tick( const size_t ms_since_last_tick )
{
  timeout = timeout > ms_since_last_tick ? timeout - ms_since_last_tick : 0;
  if ( timeout == 0 && re_send_count_ == TCPConfig::MAX_RETX_ATTEMPTS ) {
    re_send_count_++;
  }
}
