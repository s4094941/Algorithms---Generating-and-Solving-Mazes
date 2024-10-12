.default: Motivation.o

# Rule to compile test.cpp into test.o
Motivation.o: Motivation.cpp
	g++ -Wall -Werror -std=c++17 -g -O -o $@ $^ -lmcpp -Wl,-rpath,/Users/ubuntuCaiden/PS2/mcpp/build



# $^ represents ALL prerequisites
# $< represents the FIRST prerequisite

clean:
	rm -f Motivation.o
