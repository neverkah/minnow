#pragma once

#include "buffer.hh"
#include "byte_stream.hh"
#include "queue"
#include "tcp_receiver_message.hh"
#include "tcp_sender_message.hh"

class TCPSender
{
  Wrap32 isn_;

  uint64_t initial_RTO_ms_;
  uint64_t RTO_ms_ { initial_RTO_ms_ };
  uint64_t timeout { initial_RTO_ms_ };

  uint32_t un_ack_byte_count_ { 0 };
  uint64_t bytes_send_count_ { 0 };
  uint64_t re_send_count_ { 0 };

  TCPReceiverMessage receiver_message_ {};

public:
  struct send_seg
  {
    Wrap32 seqno;
    std::string buffer;
    bool SYN { false };
    bool FIN { false };
    bool sended { false };
  };
  std::deque<send_seg> un_ack_deque_ {};
  /* Construct TCP sender with given default Retransmission Timeout and possible ISN */
  TCPSender( uint64_t initial_RTO_ms, std::optional<Wrap32> fixed_isn );

  /* Push bytes from the outbound stream */
  void push( Reader& outbound_stream );

  /* Send a TCPSenderMessage if needed (or empty optional otherwise) */
  std::optional<TCPSenderMessage> maybe_send();

  /* Generate an empty TCPSenderMessage */
  TCPSenderMessage send_empty_message() const;

  /* Receive an act on a TCPReceiverMessage from the peer's receiver */
  void receive( const TCPReceiverMessage& msg );

  /* Time has passed by the given # of milliseconds since the last time the tick() method was called. */
  void tick( uint64_t ms_since_last_tick );

  /* Accessors for use in testing */
  uint64_t sequence_numbers_in_flight() const;  // How many sequence numbers are outstanding?
  uint64_t consecutive_retransmissions() const; // How many consecutive *re*transmissions have happened?
};
