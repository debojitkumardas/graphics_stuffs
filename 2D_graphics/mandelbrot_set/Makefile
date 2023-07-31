BIN_DIR=./bin/

CXX=g++
CXXFLAGS=-Wall -Werror -Wextra -pedantic -g -std=c++17
LDFLAGS=-fopenmp -lsfml-window -lsfml-graphics -lsfml-system -lGL

SRC=$(wildcard *.cpp)
OBJ=$(patsubst %.cpp, %.o, $(SRC))
BIN=$(BIN_DIR)main

all: $(BIN)

$(BIN): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(LDFLAGS)

clean:
	rm -rf $(OBJ) $(BIN)
