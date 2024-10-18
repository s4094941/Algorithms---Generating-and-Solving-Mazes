#Compiler and flag variables
#	CXX: which compiler to use
CXX = g++
#	CXXFLAGS: Compiler flags
CXXFLAGS = -Wall -Werror -std=c++17 -g -O
#	LDFLAGS: Linker flags (lmcpp library and it's directory)
LDFLAGS = -lmcpp -Wl,-rpath,/Users/jonasinocencio/PS2/mcpp/build

# File to compile into
default: test

# Rule to compile the final executable (test)
test: test.o Maze.o MazeNode.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Rule to compile test.cpp into test.o
test.o: test.cpp Maze.h MazeNode.h
	$(CXX) $(CXXFLAGS) -c $<

# Rule to compile Maze.cpp into Maze.o
Maze.o: Maze.cpp Maze.h MazeNode.h
	$(CXX) $(CXXFLAGS) -c $<

# Rule to compile MazeNode.cpp into MazeNode.o
MazeNode.o: MazeNode.cpp MazeNode.h
	$(CXX) $(CXXFLAGS) -c $<

# Clean rule to remove generated files
clean:
	rm -f *.o test

# $^ represents ALL prerequisites
# $< represents the FIRST prerequisite
# $@ represents the target of the current rule
# -g: generates debugging info into compiled output
# -c: 
# -o: 
# -f: 