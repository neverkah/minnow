#include "tcp_sender.hh"
#include "tcp_config.hh"

#include <random>

using namespace std;

/* TCPSender constructor (uses a random ISN if none given) */
TCPSender::TCPSender( uint64_t initial_RTO_ms, optional<Wrap32> fixed_isn )
  : isn_( fixed_isn.value_or( Wrap32 { random_device()() } ) ), initial_RTO_ms_( initial_RTO_ms )
{}

uint64_t TCPSender::sequence_numbers_in_flight() const
{
  // Your code here.
  return initial_RTO_ms_ == 0 ? un_ack_buffer_.size() : 0;
}

uint64_t TCPSender::consecutive_retransmissions() const
{
  // Your code here.
  return {};
}

optional<TCPSenderMessage> TCPSender::maybe_send()
{
  TCPSenderMessage sender_message {};
  if ( initial_RTO_ms_ == 0 ) {
    next_send_index_ = 0;
  }
  if ( next_send_index_ == 0 && is_first_syn_ ) {
    sender_message.SYN = true;
    sender_message.seqno = isn_;
    next_send_index_ += 1;
    if ( un_ack_buffer_.size() > next_send_index_ ) {
      sender_message.payload = { &un_ack_buffer_[next_send_index_] };
      next_send_index_ = un_ack_buffer_.length();
    }
    return sender_message;
  }
  if ( un_ack_buffer_.size() > next_send_index_ ) {
    sender_message.seqno = un_ack_no_ + next_send_index_;
    sender_message.payload = { &un_ack_buffer_[next_send_index_] };
    next_send_index_ = un_ack_buffer_.length();
    return sender_message;
  }
  return {};
}

void TCPSender::push( Reader& outbound_stream )
{
  // Your code here.
  const string_view peek_str = outbound_stream.peek();
  u_int32_t available_size = receiver_message_.window_size - un_ack_buffer_.length() + next_send_index_;
  uint32_t len = outbound_stream.peek().size() > available_size ? available_size : outbound_stream.peek().size();
  if ( len > 0 ) {
    bytes_pushed_count_ += len;

    outbound_stream.pop( len );
    un_ack_buffer_.append( peek_str.substr( 0, len ) );
  }
}

TCPSenderMessage TCPSender::send_empty_message() const
{
  // Your code here.
  TCPSenderMessage senderMessage {};
  senderMessage.seqno = un_ack_no_ + next_send_index_;
  return senderMessage;
}

void TCPSender::receive( const TCPReceiverMessage& msg )
{
  // Your code here.
  Wrap32 next_send_no = un_ack_no_ + next_send_index_;

  if ( msg.ackno.has_value() ) {
    u_int64_t abs_ackno = msg.ackno.value().unwrap( isn_, bytes_pushed_count_ );
    u_int64_t abs_next_send_no = next_send_no.unwrap( isn_, bytes_pushed_count_ );
    u_int64_t abs_un_ack_no = un_ack_no_.unwrap( isn_, bytes_pushed_count_ );
    if ( abs_ackno <= abs_next_send_no && abs_ackno >= abs_un_ack_no ) {
      receiver_message_ = msg;

      if ( abs_ackno > abs_un_ack_no ) {
        un_ack_buffer_.erase( 0, abs_ackno - abs_un_ack_no );
        next_send_index_ -= abs_ackno - abs_un_ack_no;
        un_ack_no_ = un_ack_no_ + ( abs_ackno - abs_un_ack_no );
        is_first_syn_ = false;
      }
    }
  }
}

void TCPSender::tick( const size_t ms_since_last_tick )
{
  initial_RTO_ms_ = initial_RTO_ms_ > ms_since_last_tick ? initial_RTO_ms_ - ms_since_last_tick : 0;
}
