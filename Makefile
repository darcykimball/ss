CXX = clang++-5.0
CXXFLAGS = -Wall -std=c++17 -g

EXES = server client


all: $(EXES)


server: server.cpp
	$(CXX) $(CXXFLAGS) -o server -lpthread server.cpp

client: client.cpp
	$(CXX) $(CXXFLAGS) -o client client.cpp

parser.o: parser.cpp parser.hpp
	$(CXX) $(CXXFLAGS) -c parser.cpp

clean:
	rm -rf *.o $(EXES) *.gch


new:
	make clean
	make


.PHONY: all clean new
