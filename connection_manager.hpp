#ifndef CONNECTION_MANAGER_HPP
#define CONNECTION_MANAGER_HPP


#ifndef DEBUG
#define DEBUG 1
#endif


#include <netinet/ip.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <cerrno>
#include <forward_list>
#include <future>
#include <iostream>
#include <memory>
#include <system_error>
#include <utility>

#include "connection.hpp"
#include "util.hpp"


// A connection manager; sets up the listening socket, listens, and closes
// as necessary.
// XXX: IPv4 only?
// FIXME: more docs..
template <typename Service, size_t MaxConnections = 10> // FIXME sanity check
class connection_manager {
  int _listening_socket_fd;
  Service& _service; // Non-owning reference to a service
  // Queue for checking exception status of (possibly) threads used to do work
  std::forward_list<std::future<std::exception_ptr>> _exceptions;

public:

  // Setup a socket and bind to the given address.
  connection_manager(Service& service, uint16_t port_no) : _service{service} {
    // Setup a socket
    if ((_listening_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      std::cerr << "connection_manager: failed to setup socket!\n"; 
      util::throw_errno();
    }


    // Bind an address to it (INADDR_ANY by default) FIXME
    sockaddr_in addr = { 
      .sin_family = AF_INET,
      .sin_port = htons(port_no),
      .sin_addr = INADDR_ANY
    }; 
      
    if (bind(_listening_socket_fd, reinterpret_cast<sockaddr*>(&addr),
      sizeof(sockaddr_in)) < 0) {
      std::cerr << "connection_manager: failed to bind addr to socket!\n"; 
      util::throw_errno();
    }


    // Mark the socket as listening
    if (listen(_listening_socket_fd, MaxConnections) < 0) {
      std::cerr << "connection_manager: failed to listen on socket!\n"; 
      util::throw_errno();
    }
  }


  // Start accepting connections and handling them one at a time
  void start() {
    if constexpr (DEBUG) {
      std::cerr << "connection_manager::start()\n";
    }


    int accepted; // An accepted connection


    while (true) {
      if ((accepted = accept(_listening_socket_fd, NULL, NULL)) < 0) {
        // FIXME: this may be an overreaction, but...
        std::cerr << "connection_manager: failed to accept connection!\n"; 
        util::throw_errno();
      }
      

      // Create a new connection and hand it off
      _exceptions.push_front(
        _service.serve(std::make_unique<connection>(accepted)));
          
      
      // Check for exceptions and report
      _exceptions.remove_if(
        [](auto&& eptr_future) {
          std::exception_ptr eptr;

          // FIXME: use literal seconds
          switch (eptr_future.wait_for(std::chrono::seconds(0))) {
            case std::future_status::ready:
              eptr = eptr_future.get();
              try {
                if (eptr) {
                  std::rethrow_exception(eptr);
                }
              } catch (std::exception const& e) {
                std::cerr << "A thread failed with: " <<
                  e.what() << '\n';
              }
              return true;

            case std::future_status::timeout:
              std::cerr << "A thread timed out\n";
              return true;

            // A no-op
            case std::future_status::deferred:
              return false;
          }
        }
      );
    }    
  }  
};


#endif // CONNECTION_MANAGER_HPP
