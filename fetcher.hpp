#ifndef FETCHER_HPP
#define FETCHER_HPP


#include <exception>
#include <iostream>
#include <memory>

#include <sys/stat.h>

#include <boost/filesystem.hpp>

#include "http.hpp"


namespace http {


using namespace std::literals;
using namespace boost::filesystem;


// Fetcher exception type
struct fetch_error : std::runtime_error {
  status_code code;

  fetch_error(std::string const& msg) : std::runtime_error{msg} {}
  fetch_error(status_code c) : std::runtime_error{"HTTP error"s}, code{c} {}
};


// A HTTP resource (file) fetcher
class fetcher {

  /// FIXME: Abs or rel??
  path _root; // Path to root directory

public:
  
  fetcher(std::string const& root_path) : _root(root_path) {
    try {
      // Check that it's a directory
      if (!exists(_root) || !is_directory(_root)) {
        throw fetch_error{"Root directory not found!"s};
      }

    } catch (filesystem_error const& e) {
      // Something went horribly wrong on our end
      std::cerr << "fetcher(): " << e.what() << '\n';
      throw fetch_error{status_code::internal_server_error};
    }

    _root = canonical(_root);
  }


  // Fetch a resource based on relative path
  // Throws an exception if couldn't fetch resource for whatever reason
  path fetch(path rel_path) {
    // Make path wrt root  
    path abs_path{_root};
    abs_path /= rel_path;


    std::cout << "fetcher::fetch(): trying to fetch " << abs_path << '\n';
    std::cout << "current_path = " << current_path() << '\n';
    std::cout << "exists? = " << exists(abs_path) << '\n';
    std::cout << "is_reg_file? = " << is_regular_file(abs_path) << '\n';

    try {
      // Check existence of resource
      if (!exists(abs_path) || !is_regular_file(abs_path)) {
        throw fetch_error{status_code::not_found};
      } 
      
      // Check permissions
      // TODO/FIXME
      if (!(status(abs_path).permissions() & S_IROTH)) {
        throw fetch_error{status_code::forbidden};
      }

    } catch (filesystem_error const& e) {
      // Something went horribly wrong on our end
      std::cerr << "fetcher::fetch(): " << e.what() << '\n';
      throw fetch_error{status_code::internal_server_error};
    }


    return abs_path;
  }

};


} // namespace http


#endif // FETCHER_HPP
