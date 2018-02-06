#ifndef CONNECTION_MANAGER_HPP
#define CONNECTION_MANAGER_HPP


#include <sys/socket.h>
#include <sys/types.h>

#include <system_error>


// A connection manager; sets up the listening socket, listens, and closes
// as necessary.
// XXX: IPv4 only?
// FIXME: more docs..
template <typename Service, size_t MaxConnections = 10> // FIXME sanity check
class connection_manager {
  int _listening_socket_fd;
  std::vector<connection> _connections; // Managed set; initially 0
  Service& _service; // Non-owning reference to a service

public:

  // Setup a socket and bind to the given address.
  connection_manager(Service& service) : _service{service} {
    // The number of connections is set statically, so might as well reserve
    _connections.reserve(MaxConnections) // FIXME: probably unnecessary


    // Setup a socket
    if (_listening_socket_fd = socket() < 0) {
      throw std::system_error(errno, std::generic_category);
    }

    // Bind an address to it
    // TODO



    // Mark the socket as listening
    listen(_listening_socket_fd, MaxConnections);
  }


  // Start accepting connections and handling them one at a time
  void [[noreturn]] start() {
    int accepted; // An accepted connection
    while (true) {
      if ((accepted = accept(_listening_socket_fd, NULL, NULL)) < 0) {
        // FIXME: this may be an overreaction, but...
        throw std::system_error(errno, std::generic_category);
      }
      

      // Add a new connection
      _connections.emplace_back(accepted); 


      // Do what needs to be done
      _service.serve(_connections);
    }    
  }  
};


#endif // CONNECTION_MANAGER_HPP
