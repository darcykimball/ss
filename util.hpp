#ifndef UTIL_HPP
#define UTIL_HPP


namespace util {


// Helper for error-handling
void throw_errno();


// Helper for tokenizing a line
std::vector<std::string> tokenize(std::string const& str);


} // namespace util


#endif // UTIL_HPP
