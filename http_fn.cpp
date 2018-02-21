#include <vector>

#include <boost/filesystem.hpp>

#include "http_service.hpp"
#include "fetcher.hpp"
#include "parser.hpp"


namespace http {


using namespace boost::filesystem;


std::vector<uint8_t> http_fn(fetcher& fido, std::vector<uint8_t> raw_req) {
  // Parse the raw_req
  auto req = parse_request(raw_req);
  

  // Try to find the resource
  auto file_path = fido.fetch(res_path);


  // Pack the contents in
  // XXX: horribly inefficient, but then, wouldn't be passing vector values...
  // FIXME: use iterators??
  auto size = file_size(file_path);
  std::vector<uint8_t> resource{size};
  ifstream file{file_path, std::ios::in | std::ios::binary};
  file.read(resource.data(), size);


  return resource;
}


} // namespace http
