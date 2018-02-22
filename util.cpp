#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>
#include <sstream>
#include <system_error>
#include <cerrno>

#include "util.hpp"


namespace util {


void throw_errno() {
  throw std::system_error(errno, std::generic_category());
}


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
