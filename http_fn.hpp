#ifndef HTTP_FN
#define HTTP_FN


#include <vector>

#include <boost/filesystem.hpp>

#include "fetcher.hpp"
#include "http.hpp"


namespace http {


std::vector<uint8_t> http_fn(fetcher& fido, std::vector<uint8_t> raw_req);


} // namespace http



#endif // HTTP_FN
