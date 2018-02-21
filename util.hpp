#ifndef UTIL_HPP
#define UTIL_HPP


#include <iostream>
#include <sstream>
#include <system_error>
#include <cerrno>


namespace util {


// Helper for error-handling
void throw_errno() {
  throw std::system_error(errno, std::generic_category());
}




// Helper for tokenizing a line
std::vector<std::string> tokenize(std::string const& str) {
  std::istringstream iss{str};
  std::vector<std::string> tokens;

  std::for_each(
    std::istream_iterator<std::string>{iss},
    std::istream_iterator<std::string>{},
    [&tokens](auto&& s) {
      tokens.push_back(s);
    }
  );
            

  return tokens;
}


} // namespace util


#endif // UTIL_HPP
