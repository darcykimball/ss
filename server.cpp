#include <cstdlib>
//#include <filesystem> // FIXME
#include <iostream>
#include <sstream>

#include "connection_manager.hpp"
#include "service.hpp"


int main(int argc, char** argv) { 
  using namespace std::literals;


  auto print_usage = [prog_name = argv[0]]() {
    std::cerr << "Usage: " << prog_name <<
      " -document_root directory -port port_no\n";
  };


  // Parse/validate args
  if (argc != 5 || argv[1] != "-document_root"s || argv[3] != "-port"s) {
    print_usage();
    std::exit(EXIT_FAILURE);
  }


  // TODO: validate directory path
  //if (!std::filesystem::directory


  uint16_t port_no;
  if (!(std::istringstream{argv[4]} >> port_no)) {
    std::cerr << "Error parsing port_no!\n";
    std::exit(EXIT_FAILURE);
  }


  std::cout << "Starting server on port " << port_no << "...\n";


  // FIXME: remove!!
  auto echo_function = [](auto bytes) {
    return bytes;
  };

  using service_type = threaded_service<decltype(echo_function)>;
  service_type echo_service{echo_function};

  connection_manager<service_type> manager{echo_service, port_no};

  manager.start();
    



  return EXIT_SUCCESS;
}
