#include <vector>

#include <boost/filesystem.hpp>

#include "http.hpp"
#include "http_fn.hpp"
#include "fetcher.hpp"
#include "parser.hpp"


namespace http {


using namespace boost::filesystem;


std::vector<uint8_t> http_fn(fetcher& fido, std::vector<uint8_t> raw_req) {
  // Parse the raw_req
  auto req = parse_request(raw_req);
  

  // Try to find the resource
  path file_path;

  try {
    file_path = fido.fetch(req.uri);
  } catch (fetch_error const& e) {
    std::cerr << e.what() << ": " << http::print_code(e.code) << '\n';

    // TODO: send an error response!!
    return {};
  }
 
  // Pack the contents in
  // XXX: horribly inefficient, but then, wouldn't be passing vector values...
  // FIXME: use iterators??
  auto size = file_size(file_path);

  std::cout << "http_fn(): file_path = " << file_path << '\n';
  std::cout << "http_fn(): preparing to send a file of size " << size << '\n';


  ifstream file{file_path, std::ios::in | std::ios::binary};

  std::vector<uint8_t> resource{
    std::istreambuf_iterator<char>{file},
    std::istreambuf_iterator<char>{}
  };

  if (!file) {
    std::cerr << "File bad!! wtf??\n";
  }


  std::cout << "http_fn(): ready to reply\n";
  std::cout << "http_fn(): resource is " << resource.size() << " bytes\n";

  // TODO pack
  return resource;
}


} // namespace http
