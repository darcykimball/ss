#ifndef UTIL_HPP
#define UTIL_HPP


#include <system_error>
#include <cerrno>


namespace util {


// Helper for error-handling
void throw_errno() {
  throw std::system_error(errno, std::generic_category());
}


} // namespace util


#endif // UTIL_HPP
