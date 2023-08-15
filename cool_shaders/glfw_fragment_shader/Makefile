BIN_DIR=./bin/

CXX=g++
DEPFLAGS=-MD -MP
CXXFLAGS=-Wall -Werror -Wextra -pedantic -std=c++17 -I . $(DEPFLAGS)
LDFLAGS=-lGL -lglfw -lGLEW -lpthread -ldl

SRC=$(wildcard *.cpp)
HDR=$(patsubst %.cpp, %.d, $(SRC))
OBJ=$(patsubst %.cpp, %.o, $(SRC))
BIN=$(BIN_DIR)main

all: $(BIN)

debug: CXXFLAGS += -DDEBUG -g
debug: $(BIN)

$(BIN): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(LDFLAGS)

clean:
	rm -rf $(HDR) $(OBJ) $(BIN)
