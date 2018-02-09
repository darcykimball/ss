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

  threaded_service(Function function) : _function(function) {}


  // Service all connections (in whatever way specified) and destroy
  // connections when necessary.
  // Returns a future to contain an exception to signal if anything went wrong.
  std::future<std::exception_ptr> serve(std::unique_ptr<connection> conn) {
    // For propagating exceptions back to main thread
    std::promise<std::exception_ptr> exception_promise;
    auto exception_future = exception_promise.get_future();


    // TODO: check ref/move semantics for sanity
    // TODO: exception handling!!!
    std::thread worker{
      [this, &conn](auto&& c, auto&& p) {
        std::exception_ptr eptr;

        try {
          auto msg = conn->receive();
          conn->send(_function(msg));
        } catch (...) {
          eptr = std::current_exception();
        }

        p.set_value(eptr);
      },
      std::move(conn),
      std::move(exception_promise)
    };


    // FIXME: ???
    //worker.detach();
    

    return exception_future;
  }

};


// TODO
class tasked_service;


// TODO
class eventful_service;


#endif // SERVICE_HPP
