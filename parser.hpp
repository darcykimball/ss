#ifndef PARSER_HPP
#define PARSER_HPP


#include <exception>

#include "http.hpp"


namespace http {


struct parse_error : std::runtime_error {
  parse_error() : std::runtime_error{"HTTP parse error!"} {}
};


// Parse an HTTP/1.0 request, crudely
request parse_request(std::vector<uint8_t> const& raw_req); 


} // namespace http


#endif // PARSER_HPP
