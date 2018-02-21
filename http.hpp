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
  std::vector<uint8_t> entity; // Entity-body

};


} // namespace http


#endif // HTTP_HPP
