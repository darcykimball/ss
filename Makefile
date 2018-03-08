CXX = g++-6 # XXX: Change this to your C++14-compliant compiler
CXXFLAGS = -Wall -std=c++14 -g
# XXX: You can install the boost dependencies by just getting libboost; it includes these. Pthreads should be available on most every *nix system (including macos).
LDFLAGS = -lboost_system -lboost_filesystem -lpthread

EXES = server


all: $(EXES)


server: server.cpp http_fn.o parser.o util.o fetcher.hpp
	$(CXX) $(CXXFLAGS) -o server server.cpp http_fn.o \
					parser.o util.o $(LDFLAGS)

util.o: util.cpp util.hpp
	$(CXX) $(CXXFLAGS) -c util.cpp

parser.o: parser.cpp parser.hpp	http.hpp
	$(CXX) $(CXXFLAGS) -c parser.cpp

http_fn.o: http_fn.cpp http_fn.hpp http.hpp
	$(CXX) $(CXXFLAGS) -c http_fn.cpp

clean:
	rm -rf *.o $(EXES) *.gch


new:
	make clean
	make


.PHONY: all clean new
