SRC_DIR=./src/
BIN_DIR=./bin/

CXX=g++
DEPFLAGS=-MD -MP
CXXFLAGS=-Wall -Werror -Wextra -pedantic -std=c++17 -I . $(DEPFLAGS)
LDFLAGS=-lGL -lglfw -lGLEW -lpthread -ldl

SRC=$(wildcard $(SRC_DIR)*.cpp)
HDR=$(patsubst %.cpp, $(BIN_DIR)%.d, $(notdir $(SRC)))
OBJ=$(patsubst %.cpp, $(BIN_DIR)%.o, $(notdir $(SRC)))
BIN=$(BIN_DIR)main

all: $(BIN)

debug: CXXFLAGS += -DDEBUG -g
debug: $(BIN)

$(BIN): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(BIN_DIR)%.o: $(SRC_DIR)%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(LDFLAGS)

clean:
	rm -rf $(HDR) $(OBJ) $(BIN)

clean_o:
	rm -rf .cache compile_commands.json
