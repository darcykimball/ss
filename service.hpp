#ifndef SERVICE_HPP
#define SERVICE_HPP

#include <vector>

#include "connection.hpp"


// A simple (web) service
template <typename ThreadingPolicy, typename Function>
class service {
  
public:
  service() = default; // FIXME


  // Service all connections (in whatever way specified) and destroy
  // connections when necessary.
  void serve(std::vector<connection>& connections) {
     
  }

};



#endif // SERVICE_HPP
