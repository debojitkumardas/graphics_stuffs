BIN_DIR=./bin/

CXX=g++
CXXFLAGS=-Wall -Werror -Wextra -pedantic -std=c++17
LDFLAGS=-lGL -lglfw -lGLEW

SRC=$(wildcard *.cpp)
OBJ=$(patsubst %.cpp, %.o, $(SRC))
BIN=$(BIN_DIR)main

all: $(BIN)

debug: CXXFLAGS += -DDEBUG -g

$(BIN): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(LDFLAGS)

clean:
	rm -rf $(OBJ) $(BIN)
