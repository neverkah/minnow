#include "tcp_receiver.hh"

using namespace std;

void TCPReceiver::receive( TCPSenderMessage message, Reassembler& reassembler, Writer& inbound_stream )
{
  if ( message.SYN ) {
    this->zero_point = { message.seqno };
    this->syn_appeard = true;
  }
  if ( syn_appeard ) {
    if ( !message.SYN && message.seqno == zero_point ) {
      return;
    }
    uint64_t const absolute_seq_no = message.seqno.unwrap( zero_point, inbound_stream.bytes_pushed() );
    reassembler.insert(
      absolute_seq_no == 0 ? 0 : absolute_seq_no - 1, message.payload, message.FIN, inbound_stream );
  }
}

TCPReceiverMessage TCPReceiver::send( const Writer& inbound_stream ) const
{
  TCPReceiverMessage receiverMessage = {};
  // Your code here.
  if ( syn_appeard ) {
    receiverMessage.ackno
      = Wrap32::wrap( inbound_stream.bytes_pushed() + 1 + inbound_stream.is_closed(), zero_point );
  }
  receiverMessage.window_size
    = inbound_stream.available_capacity() > UINT16_MAX ? UINT16_MAX : inbound_stream.available_capacity();
  return receiverMessage;
}
