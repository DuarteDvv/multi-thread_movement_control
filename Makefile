# cc and flags
CC = g++
CXXFLAGS = -std=c++11 -g -Wall
#CXXFLAGS = -std=c++11 -O3 -Wall

# folders
INCLUDE_FOLDER = $(wildcard ./include/*/)
BIN_FOLDER = ./bin/
OBJ_FOLDER = ./obj/
SRC_FOLDER = ./src/

# all sources, objs, and header files
MAIN = main.cpp
TARGET = a.out
SRC = $(wildcard $(SRC_FOLDER)*.cpp)
OBJ = $(patsubst $(SRC_FOLDER)%.cpp, $(OBJ_FOLDER)%.o, $(SRC))

# Add -I before each include path
INCLUDE_FLAGS = $(patsubst %,-I%, $(INCLUDE_FOLDER))

$(OBJ_FOLDER)%.o: $(SRC_FOLDER)%.cpp
	$(CC) $(CXXFLAGS) $(INCLUDE_FLAGS) -c $< -o $@

build: $(OBJ)
	$(CC) $(CXXFLAGS) -o $(BIN_FOLDER)$(TARGET) $(OBJ)

clean:
	@rm -rf $(OBJ_FOLDER)* $(BIN_FOLDER)*

run: 
	$(BIN_FOLDER)$(TARGET) $(F)
