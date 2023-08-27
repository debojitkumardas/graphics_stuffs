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

debug: CXXFLAGS += -DDEBUG -ggdb
debug: $(BIN)

release: CXXFLAGS += -O2 -DNDEBUG
release: $(BIN)

$(BIN): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(BIN_DIR)%.o: $(SRC_DIR)%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(LDFLAGS)

run_with_nvidia:
	__NV_PRIME_RENDER_OFFLOAD=1 __GLX_VENDOR_LIBRARY_NAME=nvidia ./bin/main

clean:
	rm -rf $(HDR) $(OBJ) $(BIN)

clean_o:
	rm -rf .cache compile_commands.json
