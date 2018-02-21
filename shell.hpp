#ifndef SHELL_HPP
#define SHELL_HPP


#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <sstream>
#include <map>
#include <vector>

#include "util.hpp"


namespace cli {


using namespace std::literals;


// A simple CLI
// TODO graceful, non-^C quit??
class shell {

  using command_type = void (std::vector<std::string>);
  using command_name = std::string;
  using fn = std::function<command_type>; // XXX: type-erased b/c laziness
  using map_type = std::map<command_name, fn>;

  map_type _command_map;
  std::istream& _input_stream;

  static constexpr auto prompt_string = ">>>";

public:

  shell(std::initializer_list<std::pair<command_name const, fn>> il,
        std::istream& is = std::cin) :
    _command_map(il), _input_stream(is) {
  }


  void run() {
    std::cout << "Starting shell...\n";

    std::cout << "Available commands:\n";
    for (auto const& p : _command_map) {
      std::cout << p.first << '\n';
    }
    // FIXME Ugh...
    std::cout << prompt_string;

    
    std::string input;
    while (std::getline(_input_stream, input)) {
      // Tokenize/check for command
      auto tokens = util::tokenize(input);

      if (tokens.size() == 0) {
        // Ignore whitespace lines
        std::cout << prompt_string;
        continue;
      }

      auto found = _command_map.find(tokens[0]);
      if (found == _command_map.end()) {
        std::cerr << "Command \"" << tokens[0] << "\" not found\n";
        std::cout << prompt_string;
        continue;
      }


      // Issue the command
      (found->second)(tokens);
      std::cout << prompt_string;
    }

    
    // For prettiness
    std::cout << '\n';
  }
};


} // namespace cli


#endif// SHELL_HPP
