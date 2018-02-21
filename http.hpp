#ifndef HTTP_HPP
#define HTTP_HPP


#include <fstream>
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


// A dumb type to store header info
struct header {
  std::string type;
  std::string value;
};


// A minimal HTTP request
struct request {

  enum class rtype {
    GET,
    PUT, // TODO
    HEAD // TODO
  };


  rtype type;
  std::string uri;
  std::vector<header> headers;

};


// A minimal HTTP response
struct response {

  status_code code;
  std::vector<header> headers;
  std::unique_ptr<ifstream> entity; // A handle to the resource to send

};


} // namespace http


#endif // HTTP_HPP
