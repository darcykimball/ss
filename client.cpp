#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include <cstdint>
#include <cstdlib>
#include <iostream>

#include "shell.hpp"


using namespace std::literals;


int main(int argc, char** argv) {
  auto print_usage = [prog_name = argv[0]]() {
    std::cerr << "Usage: " << prog_name << " server_ip server_port\n";
  };


  // Parse/validate server args
  if (argc != 3) {
    print_usage();
    exit(EXIT_FAILURE);
  }


  // Get the server's address info
  // TODO
  addrinfo* server_addrinfo_ptr;
  addrinfo hints = {
    .ai_flags = 0,
    .ai_family = AF_INET,
    .ai_socktype = SOCK_STREAM,
    .ai_protocol = 0,
    .ai_addrlen = sizeof(sockaddr_in),
    .ai_addr = nullptr,
    .ai_canonname = nullptr,
    .ai_next = nullptr
  };

  auto gai_retval =
    getaddrinfo(argv[1], argv[2], &hints, &server_addrinfo_ptr);
  
  if (gai_retval) {
    std::cerr << "Error identifying server/port:\n";
    std::cerr << gai_strerror(gai_retval) << '\n';
    exit(EXIT_FAILURE);
  }


  // Store the server address
  sockaddr_in const server_addr =
    *(reinterpret_cast<sockaddr_in*>(server_addrinfo_ptr->ai_addr));

  freeaddrinfo(server_addrinfo_ptr);


  // Setup CLI
  cli::shell cli{
    { "echo", [&server_addr](auto&& args) {
        if (args.size() != 2) {
          std::cerr << "usage: echo string\n";
          return;
        }

       
        // Setup a socket
        int socket_fd;
        if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
          std::cerr << "Error creating socket!\n"; 
          perror("cli:echo:");
          return;
        }


        // Attempt to connect  
        if (connect(
              socket_fd,
              reinterpret_cast<sockaddr const*>(&server_addr),
              sizeof(sockaddr_in)) < 0)
        {
          std::cerr << "Error establishing connection!\n";
          perror("cli:echo:");
          return;
        }


        // Send the message
        auto send_retval =
          send(socket_fd, args[1].c_str(), args[1].length(), 0);

        if (send_retval < 0) {
          std::cerr << "Error sending!\n";
          perror("cli:echo:");
          return;
        }
        std::cout << "Sent " << send_retval << " bytes.\n";


        // Wait for response
        std::array<uint8_t, 4096> recv_buf;
        recv_buf.fill(0x00);
        auto recv_retval = recv(socket_fd, recv_buf.data(), recv_buf.size(), 0);

        if (recv_retval < 0) {
          std::cerr << "Error receiving reply!\n"; 
          perror("cli:echo:");
          return;
        }

        
        // Print the reply
        if (recv_retval >= recv_buf.size()) {
          std::cerr << "Reply to large to fit in buffer!\n";
          return;
        }
          

        std::cout << "Received reply:\n";
        std::cout << reinterpret_cast<char const*>(recv_buf.data());
        std::cout << '\n';


        close(socket_fd);
      }
    }
  };

  
  cli.run();
  

  return EXIT_SUCCESS;
}
