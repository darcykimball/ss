CXX = clang++-5.0
CXXFLAGS = -Wall -std=c++17 -g
LDFLAGS = -lboost_system -lboost_filesystem

EXES = server client


all: $(EXES)


server: server.cpp http_fn.o parser.o util.o fetcher.hpp
	$(CXX) $(CXXFLAGS) -o server -lpthread server.cpp http_fn.o \
					parser.o util.o $(LDFLAGS)

client: client.cpp util.o
	$(CXX) $(CXXFLAGS) -o client client.cpp util.o $(LDFLAGS)

util.o: util.cpp util.hpp
	$(CXX) $(CXXFLAGS) -c util.cpp

parser.o: parser.cpp parser.hpp	http.hpp
	$(CXX) $(CXXFLAGS) -c parser.cpp

http_fn.o: http_fn.cpp http_fn.hpp http.hpp
	$(CXX) $(CXXFLAGS) -c http_fn.cpp

test_parser: parser.o test_parser.cpp
	$(CXX) $(CXXFLAGS) -o test_parser test_parser.cpp -lboost_system parser.o

clean:
	rm -rf *.o $(EXES) *.gch


new:
	make clean
	make


.PHONY: all clean new
