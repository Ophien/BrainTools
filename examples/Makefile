LIBRARY_PATH=/usr/local/lib/libbraintools/

CXX=g++
CXXFLAGS=-std=c++11
SRCS=$(wildcard *.cpp)

all: $(SRCS:.cpp=)

.cpp:
	$(CXX) $(CXXFLAGS) -L $(LIBRARY_PATH) -lbraintools $< -o $@.out

clean:
	rm -rf *.out