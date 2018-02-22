#include <iomanip>
#include <string>
#include <sstream>
#include <vector>

#include <ctime>

#include <boost/filesystem.hpp>

#include "http.hpp"
#include "http_fn.hpp"
#include "fetcher.hpp"
#include "parser.hpp"


namespace http {


using namespace boost::filesystem;


std::vector<uint8_t> http_fn(fetcher& fido, std::vector<uint8_t> raw_req) {
  // The eventual return val
  response resp;


  // Parse the raw_req
  request req;
  try {
    req = parse_request(raw_req);
  } catch (parse_error const& e) {
    std::cerr << e.what() << '\n';

    // Send error 400
    resp.code = status_code::bad_request;

    return resp.as_bytes(nullptr);
  }

  // Try to find the resource
  path file_path;

  try {
    file_path = fido.fetch(req.uri);
  } catch (fetch_error const& e) {
    std::cerr << e.what() << ": " << http::print_code(e.code) << '\n';

    // Send an error response
    resp.code = e.code;

    return resp.as_bytes(nullptr);
  }
 

  // Pack the contents in
  // XXX: horribly inefficient, but then, wouldn't be passing vector values...
  std::cout << "http_fn(): file_path = " << file_path << '\n';


  ifstream file{file_path, std::ios::in | std::ios::binary};

  std::vector<uint8_t> resource{
    std::istreambuf_iterator<char>{file},
    std::istreambuf_iterator<char>{}
  };


  std::cout << "http_fn(): ready to reply\n";
  std::cout << "http_fn(): resource is " << resource.size() << " bytes\n";


  resp.code = status_code::ok;

  std::time_t time = std::time(nullptr);
  std::ostringstream oss;
  oss << std::put_time(std::gmtime(&time), "%c %Z");

  resp.headers[content_date_key] = oss.str();


  resp.headers[content_type_key] = file_path.extension().string().substr(1);
  resp.headers[content_length_key] = std::to_string(resource.size());

  
  return resp.as_bytes(&resource);
}


} // namespace http
