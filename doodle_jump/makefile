SFML_PREFIX := /ucrt64/local/sfml2

CXX       := g++
CXXFLAGS  := -std=c++17 -Iinclude -I$(SFML_PREFIX)/include
SFMLFLAGS := -L$(SFML_PREFIX)/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

SRC_DIR   := src
OBJ_DIR   := obj

TARGET    := game.exe

SRCS      := $(wildcard $(SRC_DIR)/*.cpp)
OBJS      := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

all: create_obj $(TARGET)

create_obj:
	@mkdir -p $(OBJ_DIR)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(SFMLFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | create_obj
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: all clean create_obj