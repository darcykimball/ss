#include <cstdlib>
#include <filesystem>
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


  if (!std::filesystem::directory


  uint16_t port_no;
  if (!(std::istringstream{argv[4]} >> port_no)) {
    std::cerr << "Error parsing port_no!\n";
    std::exit(EXIT_FAILURE);
  }


  std::cout << "Starting server on port " << port_no << "...\n";



  return EXIT_SUCCESS;
}
