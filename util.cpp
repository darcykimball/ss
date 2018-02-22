#include <algorithm>
#include <exception>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <vector>
#include <sstream>
#include <system_error>
#include <cerrno>
#include <cstdio>

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


std::string decode_uri(std::string const& uri) {
  std::istringstream iss{uri};
  std::string retval;

  // Look for percent-encoded sequences
  std::string line;
  int decoded; 
  while (std::getline(iss, line, '%')) {
    std::cerr << "Yo: line =" << line << '\n';


    if (iss) {
      std::string escaped;
      escaped.resize(2);
      iss.read(&escaped[0],2);


      std::istringstream{escaped} >> std::hex >> decoded;

      std::cout << "decoded =" << std::hex << decoded << '\n';


      retval += line;
      retval += static_cast<char>(decoded);
      continue;
    }

    retval += line;
  }

  if (iss) {
    iss >> line;
    retval += line;
  }

  return retval;
}


} // namespace util
