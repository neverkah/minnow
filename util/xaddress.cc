#include "xaddress.hh"

#include "iostream"
#include <arpa/inet.h>
#include <cstring>
#include <stdexcept>


//! Converts Raw to `sockaddr *`.
XAddress::Raw::operator sockaddr*()
{
  return reinterpret_cast<sockaddr*>( &storage ); // NOLINT(*-reinterpret-cast)
}

//! Converts Raw to `const sockaddr *`.
XAddress::Raw::operator const sockaddr*() const
{
  std::cout << "Raw::operator const called" << std::endl;
  return reinterpret_cast<const sockaddr*>( &storage ); // NOLINT(*-reinterpret-cast)
}

//! \param[in] addr points to a raw socket address
//! \param[in] size is `addr`'s length
XAddress::XAddress( const sockaddr* addr, const size_t size ) : _size( size )
{
  // make sure proposed sockaddr can fit
  if ( size > sizeof( _address.storage ) ) {
    throw std::runtime_error( "invalid sockaddr size" );
  }

  memcpy( &_address.storage, addr, size );
}

// equality
bool XAddress::operator==( const XAddress& other ) const
{
  if ( _size != other._size ) {
    return false;
  }

  return 0 == memcmp( &_address, &other._address, _size );
}
