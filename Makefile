EXEC += main_hitter
all: $(EXEC)

CFLAGS = -Wall -std=c++11 -O3
HEADER += hash.h datatypes.hpp util.h adaptor.hpp 
SRC += hash.c adaptor.cpp bucket.cpp
SKETCHHEADER += NinoSketch.hpp
SKETCHSRC += NinoSketch.cpp
LIBS= -lpcap 

main_hitter: main_hitter.cpp $(SRC) $(HEADER) $(SKETCHHEADER) 
	g++ $(CFLAGS) -mavx2 $(INCLUDES) -o $@ $< $(SRC) $(SKETCHSRC) $(LIBS) -w

clean:
	rm -rf $(EXEC)
	rm -rf *log*
	rm -rf *out*
