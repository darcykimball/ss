#include <iostream>
#include <sstream>

#include "parser.hpp"
#include "util.hpp"


namespace http {


// Parse an HTTP/1.0 request, crudely
request parse_request(std::vector<uint8_t> const& raw_req) {
  using namespace std::literals;


  // Setup as (assumed) string stream
  std::string s{reinterpret_cast<char const*>(raw_req.data()), raw_req.size()};
  std::istringstream iss{s};


  std::cout << "http::parse_request(): The entire request:\n";
  std::cout << s << "\nhttp::parse_request(): End request\n";


  request req; // Return value


  // Parse request line
  std::string line;
  if (std::getline(iss, line)) {
    auto tokens = util::tokenize(line);
    
    // Only accept HTTP/1.0 or 1.1 GET requests
    if (
        tokens.size() != 3  // Is this a proper req. line?
     || tokens[0] != "GET"s // Is this a GET?
     || (tokens[2] != "HTTP/1.0" && tokens[2] != "HTTP/1.1"s) // Supported ver.?
    ) {
      throw parse_error{"Malformed req line"};
    }


    // Alright so far.
    req.type = request::method::GET;


    // Use index.html as the default
    if (tokens[1] == "/"s) {
      req.uri = "/index.html";
    } else {
      req.uri = util::decode_uri(tokens[1]);
    }

  } else {
    // Can't be right.
    throw parse_error{"No request line"};
  }

  
  // Parse headers
  while (std::getline(iss, line)) {
    // If a whitespace line was read, assume it ends the header section
    if (line.find_first_not_of(" \t\n\v\f\r") != std::string::npos) {
      break;
    }

    // Split type and value
    auto split_pos = line.find(':');

    // Error if no colon-delimited pair found
    if (split_pos == std::string::npos) {
      throw parse_error{"Bad header"};
    }


    // OK; keep track of this header
    req.headers.insert(
      std::make_pair(
        line.substr(0, split_pos), // Header type
        line.substr(split_pos + 1) // Value
      )
    );
  }


  return req;
}


} // namespace http
