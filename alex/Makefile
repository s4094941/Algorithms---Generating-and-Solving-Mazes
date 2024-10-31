.default: all

all: mazeRunner

clean:
	rm -f mazeRunner Maze Agent MazeNode *.o

mazeRunner: Maze.o MazeNode.o Agent.o mazeRunner.o
	g++ -Wall -Werror -std=c++17 -g -O -o $@ $^ -lmcpp

%.o: %.cpp
	g++ -Wall -Werror -std=c++17 -g -O -c $^
