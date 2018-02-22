#include <cstdlib>
#include <iostream>
#include <sstream>
#include <utility>

#include <boost/filesystem.hpp>

#include "connection_manager.hpp"
#include "fetcher.hpp"
#include "http_fn.hpp"
#include "service.hpp"


int main(int argc, char** argv) { 
  using namespace std::literals;
  using namespace boost::filesystem;


  auto print_usage = [prog_name = argv[0]]() {
    std::cerr << "Usage: " << prog_name <<
      " -document_root directory -port port_no\n";
  };


  // Parse/validate args
  if (argc != 5 || argv[1] != "-document_root"s || argv[3] != "-port"s) {
    print_usage();
    std::exit(EXIT_FAILURE);
  }


  uint16_t port_no;
  if (!(std::istringstream{argv[4]} >> port_no)) {
    std::cerr << "Error parsing port_no!\n";
    std::exit(EXIT_FAILURE);
  }


  std::cout << "Starting server on port " << port_no << "...\n";


  // Setup the service
  http::fetcher fido{argv[2]}; // The file finder/fetcher
  auto serve_http_fn = [&fido](auto&& bytes) {
    return http::http_fn(fido, std::forward<decltype(bytes)>(bytes));
  };


  using service_type = threaded_service<decltype(serve_http_fn)>;
  service_type http_service{serve_http_fn};

  connection_manager<service_type> manager{http_service, port_no};

  std::cout << "Starting to listen for connections...\n";
  manager.start();
    

  return EXIT_SUCCESS;
}
