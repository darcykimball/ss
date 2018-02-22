#ifndef HTTP_HPP
#define HTTP_HPP


#include <fstream>
#include <map>
#include <memory>
#include <vector>

#include <boost/filesystem.hpp>


// Various protocol-related definitions
namespace http {


using namespace boost::filesystem;


// Response codes
enum class status_code {
  ok = 200,
  forbidden = 403,
  not_found = 404,
  internal_server_error = 500
};


inline std::string print_code(status_code code) {
  switch (code) {
    case status_code::ok:
      return "200 OK";
    case status_code::forbidden:
      return "403 Forbidden";
    case status_code::not_found:
      return "404 Not found";
    case status_code::internal_server_error:
      return "500 Internal server error";
    default:
      return "No code";
  }
}


// Dumb type aliases for header types
using header_type = std::string;
using header_value = std::string;


// Some useful header-related constants
constexpr auto content_type_key = "Content-Type";
constexpr auto content_length_key= "Content-Length";
constexpr auto content_date_key = "Date";


// A minimal HTTP request
struct request {

  enum class method {
    GET,
    PUT, // TODO
    HEAD // TODO
  };


  method type;
  std::string uri;
  std::map<header_type, header_value> headers;

};


// A minimal HTTP response
struct response {

  status_code code;
  std::map<header_type, header_value> headers; // XXX: only date, content-type, and content-len

  
  // Form a proper response
  std::vector<uint8_t> as_bytes(std::vector<uint8_t> const* entity) const {
    std::string status_line{"HTTP/1.0 "}; // To build status line

    // Append status code and explanation
    status_line += print_code(code);
    status_line += '\n';


    // The raw response
    std::vector<uint8_t> bytes{status_line.cbegin(), status_line.cend()};

    // Add headers
    for (auto const& h : headers) {
      std::string header_line{h.first};
      header_line.append(": ").append(h.second).append("\n");
      bytes.insert(bytes.end(), header_line.cbegin(), header_line.cend());
    }
   
    // XXX: gotcha
    bytes.push_back('\n');
    

    switch (code) {
      case status_code::ok:
        // Append entity body
        if (entity != nullptr) {
          bytes.insert(bytes.end(), entity->cbegin(), entity->cend());
          break;
        }
        
      case status_code::forbidden:
      case status_code::not_found:
      case status_code::internal_server_error:
      default:
        // If we reached here, we should send an error response
        break;
    }   


    return bytes;
  }
  

};


} // namespace http


#endif // HTTP_HPP
