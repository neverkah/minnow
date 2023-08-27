#pragma once

#include <cstddef>
#include <cstdint>
#include <netdb.h>
#include <netinet/in.h>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <utility>

//! Wrapper around [IPv4 addresses](@ref man7::ip) and DNS operations.
class XAddress
{
public:
  //! \brief Wrapper around [sockaddr_storage](@ref man7::socket).
  //! \details A `sockaddr_storage` is enough space to store any socket address (IPv4 or IPv6).
  class Raw
  {
  public:
    sockaddr_storage storage {}; //!< The wrapped struct itself.
    // NOLINTBEGIN (*-explicit-*)
    operator sockaddr*();
    operator const sockaddr*() const;
    // NOLINTEND (*-explicit-*)
  };

private:
  socklen_t _size {}; //!< Size of the wrapped address.
  Raw _address {};    //!< A wrapped [sockaddr_storage](@ref man7::socket) containing the address.

public:
  //! Construct from a [sockaddr *](@ref man7::socket).
  XAddress( const sockaddr* addr, std::size_t size );

  //! Equality comparison.
  bool operator==( const XAddress& other ) const;
  bool operator!=( const XAddress& other ) const { return not operator==( other ); }

  operator const sockaddr*() const { return ( _address ); } // NOLINT(*-explicit-*)
};
