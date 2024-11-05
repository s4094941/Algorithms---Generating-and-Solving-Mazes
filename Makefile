# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Werror -std=c++17 -g -O
# Default linker flags (if you installed ur stuff right)
LDFLAGS = -lmcpp

default: all

all: mazeRunner

# Clean rule
clean:
	rm -f mazeRunner Agent Maze MazeNode *.o

# Standard build
mazeRunner: Maze.o MazeNode.o Agent.o mazeRunner.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# fix for jonas'  path
jonas: LDFLAGS += -Wl,-rpath,/Users/jonasinocencio/PS2/mcpp/build
jonas: mazeRunner


%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<