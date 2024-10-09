.default: test.o

# Rule to compile test.cpp into test.o
test.o: test.cpp
	g++ -Wall -Werror -std=c++17 -g -O -o $@ $^ -lmcpp -Wl,-rpath,/Users/jonasinocencio/PS2/mcpp/build

# $^ represents ALL prerequisites
# $< represents the FIRST prerequisite

clean:
	rm -f test.o