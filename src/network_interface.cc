#include "network_interface.hh"

#include "arp_message.hh"
#include "ethernet_frame.hh"

using namespace std;
// ARP广播时，如果非目标机器已知目标机器的的mac地址，是否直接返回，不再取访问目标机器？

// ethernet_address: Ethernet (what ARP calls "hardware") address of the interface
// ip_address: IP (what ARP calls "protocol") address of the interface
NetworkInterface::NetworkInterface( const EthernetAddress& ethernet_address, const Address& ip_address )
  : ethernet_address_( ethernet_address ), ip_address_( ip_address )
{
  cerr << "DEBUG: Network interface has Ethernet address " << to_string( ethernet_address_ ) << " and IP address "
       << ip_address.ip() << "\n";
}

// dgram: the IPv4 datagram to be sent
// next_hop: the IP address of the interface to send it to (typically a router or default gateway, but
// may also be another host if directly connected to the same network as the destination)

// Note: the Address type can be converted to a uint32_t (raw 32-bit IP address) by using the
// Address::ipv4_numeric() method.
void NetworkInterface::send_datagram( const InternetDatagram& dgram, const Address& next_hop )
{

  if ( eth_addr_known( next_hop ) ) {
    EthernetFrame ef {};
    ef.header.src = ethernet_address_;
    ef.header.dst = address_map_[next_hop.ipv4_numeric()];
    ef.header.type = EthernetHeader::TYPE_IPv4;
    ef.payload = serialize( dgram );
    ef_queue_.push( ef );
    return;
  }
  // 加入待发送队列
  dgram_queue_.push( { dgram, next_hop } );

  if ( arp_map_.find( next_hop.ipv4_numeric() ) != arp_map_.end()
       && arp_map_[next_hop.ipv4_numeric()] <= ARP_REPEATE_GAP ) {
    return;
  }
  // 发送ARP报文
  ARPMessage message {};
  message.opcode = ARPMessage::OPCODE_REQUEST;
  message.sender_ip_address = ip_address_.ipv4_numeric();
  message.target_ip_address = next_hop.ipv4_numeric();
  message.sender_ethernet_address = ethernet_address_;

  EthernetFrame ef {};
  ef.header.src = ethernet_address_;
  ef.header.dst = ETHERNET_BROADCAST;
  ef.header.type = EthernetHeader::TYPE_ARP;
  ef.payload = serialize( message );
  ef_queue_.push( ef );

  // 时间置为0
  arp_map_[next_hop.ipv4_numeric()] = 0;
}
bool NetworkInterface::eth_addr_known( Address next_hop )
{
  return address_map_.find( next_hop.ipv4_numeric() ) != this->address_map_.end();
}

// frame: the incoming Ethernet frame
optional<InternetDatagram> NetworkInterface::recv_frame( const EthernetFrame& frame )
{

  if ( EthernetHeader::TYPE_IPv4 == frame.header.type ) {
    if ( frame.header.dst != ethernet_address_ ) {
      return {};
    }
    InternetDatagram datagram {};
    if ( parse( datagram, frame.payload ) ) {
      return datagram;
    }
    return {};
  }
  if ( EthernetHeader::TYPE_ARP == frame.header.type ) {
    ARPMessage arp_message {};
    if ( !parse( arp_message, frame.payload ) ) {
      return {};
    }
    // 请求或响应都需要记录发送方信息
    address_map_[arp_message.sender_ip_address] = arp_message.sender_ethernet_address;
    address_ttl_map_[arp_message.sender_ip_address] = IP_ETH_TTL;
    // eth地址已知，删除未响应的arp信息
    arp_map_.erase( arp_message.sender_ip_address );
    if ( arp_message.opcode == ARPMessage::OPCODE_REQUEST ) {
      if ( arp_message.target_ip_address == ip_address_.ipv4_numeric() ) {
        // 发送ARP报文
        ARPMessage message {};
        message.opcode = ARPMessage::OPCODE_REPLY;
        message.sender_ip_address = ip_address_.ipv4_numeric();
        message.target_ip_address = arp_message.sender_ip_address;
        message.sender_ethernet_address = ethernet_address_;
        message.target_ethernet_address = arp_message.sender_ethernet_address;

        EthernetFrame ef {};
        ef.header.src = ethernet_address_;
        ef.header.dst = arp_message.sender_ethernet_address;
        ef.header.type = EthernetHeader::TYPE_ARP;
        ef.payload = serialize( message );
        ef_queue_.push( ef );
      }
    }
  }
  return {};
}

// ms_since_last_tick: the number of milliseconds since the last call to this method
void NetworkInterface::tick( const size_t ms_since_last_tick )
{
  for ( auto& item : arp_map_ ) {
    item.second += ms_since_last_tick;
  }
  set<u_int32_t> dated_address {};
  for ( auto& item : address_ttl_map_ ) {
    // 过期
    if ( item.second < ms_since_last_tick ) {
      dated_address.insert( item.first );
    } else {
      item.second -= ms_since_last_tick;
    }
  }
  for ( const auto& item : dated_address ) {
    address_ttl_map_.erase( item );
    address_map_.erase( item );
  }
}

optional<EthernetFrame> NetworkInterface::maybe_send()
{
  if ( !ef_queue_.empty() ) {
    EthernetFrame ef = ef_queue_.front();
    ef_queue_.pop();
    return ef;
  }

  if ( !dgram_queue_.empty() ) {
    DgramHop const distDgram = dgram_queue_.front();

    if ( eth_addr_known( distDgram.next_hop ) ) {
      dgram_queue_.pop();
      EthernetFrame ef {};
      ef.header.src = ethernet_address_;
      ef.header.dst = address_map_[distDgram.next_hop.ipv4_numeric()];
      ef.header.type = EthernetHeader::TYPE_IPv4;
      ef.payload = serialize( distDgram.dgram );
      return ef;
    }
  }
  return {};
}
