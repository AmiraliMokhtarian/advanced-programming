CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = .

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
TARGET = $(BIN_DIR)/game

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(TARGET)
