#ifndef CONNECTION_HPP
#define CONNECTION_HPP


#include <sys/socket.h>
#include <sys/types.h>

#include <array>
#include <cstddef>
#include <system_error>


// An accept()ed connection.
class connection {

  static constexpr size_t buffer_size = 4096;


  int _socket_fd; // Fd for this end
  std::array<std::byte, buffer_size> _recv_buffer; // The buffer filled by recv()


public:

  // The ctor param should be the file descriptor returned by a successful
  // accept().
  // FIXME: might want to clear buffer for debug purposes
  connection(int fd) : _socket_fd{fd} {}

  
  // Cleanup: close the connection.
  ~connection() {
    close(_socket_fd);
  }


  // XXX: Connections imply system resources, so to keep it simple, disallow
  // value semantics.
  connection(connection const&) = delete;
  connection(connection&&) = delete; 
  connection& operator=(connection const&) = delete;
  connection& operator=(connection&&) = delete;


  // Receive some stuff.
  // XXX: Blocking receive.
  std::vector<std::byte> receive() {
    // TODO: error checking
    recv(_socket_fd,_recv_buffer.data(), buffer_size, 0);
  }


  // Send some stuff.
  void send(std::vector<std::byte> const& msg) {
    // TODO: error checking
    send(_socket_fd, msg.data(), msg.size(), 0);
  }

};


#endif // CONNECTION_HPP
