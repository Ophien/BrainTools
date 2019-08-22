
OBJECTS=BrainTools.o
LIBRARY=libbraintools.a
LIBRARY_PATH=/usr/local/lib/libbraintools/
INCLUDE_PATH=/usr/local/include/libbraintools/
HEADERS=BrainTools.h

CXX=g++
CXXFLAGS=-std=c++11

all: $(OBJECTS)
	ar -rvs $(LIBRARY) $(OBJECTS)
	rm -rf *.o

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -rf $(OBJECTS) $(LIBRARY)

install:
	mkdir -p $(LIBRARY_PATH) && cp $(LIBRARY) $(LIBRARY_PATH)
	mkdir -p $(INCLUDE_PATH) && cp $(HEADERS) $(INCLUDE_PATH)

remove:
	rm -rf $(LIBRARY_PATH) $(INCLUDE_PATH)