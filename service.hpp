#ifndef SERVICE_HPP
#define SERVICE_HPP


#include <exception>
#include <future>
#include <memory>
#include <thread>
#include <vector>

#include "connection.hpp"


// A simple (web) service
template <typename Function>
class threaded_service {

  Function _function; // The actual functionality provided by this service

public:

  using conn_ptr = std::unique_ptr<connection>;
  
  // The type of promise sent back to the main thread
  using promise_back = std::promise<std::exception_ptr>;
  using future_back = std::future<std::exception_ptr>;


  threaded_service(Function function) : _function(function) {}


  // Service all connections (in whatever way specified) and destroy
  // connections when necessary.
  // Returns a future to contain an exception to signal if anything went wrong.
  future_back serve(conn_ptr conn) {
    // For propagating exceptions back to main thread
    promise_back exception_promise;
    auto exception_future = exception_promise.get_future();


    std::thread worker{
      [this, c{std::move(conn)}, p{std::move(exception_promise)}]() mutable {
        std::exception_ptr eptr;

        if constexpr (DEBUG) {
          std::cerr << "service::serve: worker started here.\n";
        }

        try {
          auto msg = c->receive();
          c->send(_function(msg));
        } catch (...) {
          eptr = std::current_exception();
        }

        p.set_value(eptr);
      }
    };


    // Go forth
    if (worker.joinable()) {
      worker.detach();
    }


    return exception_future;
  }

};


// TODO
class tasked_service;


// TODO
class eventful_service;


#endif // SERVICE_HPP
