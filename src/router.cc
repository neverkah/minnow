#include "router.hh"

#include <iostream>
#include <limits>
using namespace std;

// route_prefix: The "up-to-32-bit" IPv4 address prefix to match the datagram's destination address against
// prefix_length: For this route to be applicable, how many high-order (most-significant) bits of
//    the route_prefix will need to match the corresponding bits of the datagram's destination address?
// next_hop: The IP address of the next hop. Will be empty if the network is directly attached to the router (in
//    which case, the next hop address should be the datagram's final destination).
// interface_num: The index of the interface to send the datagram out on.
void Router::add_route( const uint32_t route_prefix,
                        const uint8_t prefix_length,
                        const optional<Address> next_hop,
                        const size_t interface_num )
{
  cerr << "DEBUG: adding route " << Address::from_ipv4_numeric( route_prefix ).ip() << "/"
       << static_cast<int>( prefix_length ) << " => " << ( next_hop.has_value() ? next_hop->ip() : "(direct)" )
       << " on interface " << interface_num << "\n";
  Route route = { route_prefix, prefix_length, next_hop, interface_num };
  routes_.push_back( route );
}

void Router::route()
{
  for ( auto& net_interface : interfaces_ ) {
    optional<InternetDatagram> op_internet_datagram = net_interface.maybe_receive();
    if ( !op_internet_datagram.has_value() ) {
      continue;
    }

    InternetDatagram internet_datagram = op_internet_datagram.value();

    uint32_t const dst = internet_datagram.header.dst;
    optional<Route> op_match_route = matchRoute( dst );
    if ( !op_match_route.has_value() ) {
      continue;
    }
    if ( internet_datagram.header.ttl > 0 ) {
      internet_datagram.header.ttl--;
      internet_datagram.header.compute_checksum();
    }
    if ( internet_datagram.header.ttl == 0 ) {
      continue;
    }
    Route const match_route = op_match_route.value();
    Address next_hop = Address::from_ipv4_numeric( dst );
    if ( match_route.next_hop_.has_value() ) {
      next_hop = match_route.next_hop_.value();
    }

    this->interface( match_route.interface_num_ ).send_datagram( internet_datagram, next_hop );
  }
}
optional<Router::Route> Router::matchRoute( const uint32_t dst )
{
  optional<Route> op_match_route {};
  uint8_t match_length = 0;
  for ( const auto& route : routes_ ) {
    uint32_t const mask = route.prefix_length_ > 0 ? UINT32_MAX << ( 32 - route.prefix_length_ ) : 0;
    if ( ( dst & mask ) == route.route_prefix_ && route.prefix_length_ >= match_length ) {
      op_match_route = route;
      match_length = route.prefix_length_;
    }
  }
  return op_match_route;
}
