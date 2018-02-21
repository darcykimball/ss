CXX = clang++-5.0
CXXFLAGS = -Wall -std=c++17 -g

EXES = server client


all: $(EXES)


server: server.cpp
	$(CXX) $(CXXFLAGS) -o server -lpthread server.cpp

client: client.cpp
	$(CXX) $(CXXFLAGS) -o client client.cpp

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
