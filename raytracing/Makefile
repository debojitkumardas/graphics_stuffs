# simple makefile to build and run
# use: make debug -> to build with debugging info
# use: make release -> to build for release

# dir for source code
SRC_DIR = ./src/
# dir for headers
HDR_DIR = ./include/
# build dir
BLD_DIR = ./build/

# type of build
TYPE = debug/

# dir for object files
OBJ_DIR = $(BLD_DIR)$(TYPE)obj/
# dir for bin
BIN_DIR = $(BLD_DIR)$(TYPE)bin/

# compiler
CXX = g++
# generate files that encode make rules for .h dependencies
DEPFLAGS = -MP -MD
# options for debug(release) mode build
OPTS = -DDEBUG -ggdb -fsanitize=address
# compiler flags
CXXFLAGS = -Wall -Wextra -Werror -pedantic -std=c++20 $(OPTS) -I$(HDR_DIR) $(DEPFLAGS)
# library flags
# LDFLAGS = -lsfml-window -lsfml-graphics -lsfml-system -lGL -lGLEW

# for release build
release: OPTS = -O2 -DNDEBUG
release: TYPE = release/

# src files
SRC = $(wildcard $(SRC_DIR)*.cpp)
# obj files
OBJ = $(patsubst %.cpp, $(OBJ_DIR)%.o, $(notdir $(SRC)))
# dep files
DEP = $(patsubst %.cpp, $(OBJ_DIR)%.d, $(notdir $(SRC)))
# binary
BIN = $(BIN_DIR)main.exe

all: $(BIN)

# debug build
debug: $(BIN)

# release build
release: $(BIN)

$(BIN): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# include dependency for the build process
-include $(DEP)

run:
	@$(BIN)

run_with_valgrind:
	@valgrind --enable-debuginfod=no --leak-check=full --show-leak-kinds=all --errors-for-leak-kinds=all --track-origins=yes -s --verbose --log-file=valgrind_out.txt $(BIN)

clean:
	@rm -rf $(BIN)

cleanall:
	@rm -rf $(BIN) $(OBJ_DIR)*
