SFML_PREFIX := /ucrt64/local/sfml2

CXX       := g++
INCLUDES  := -Iinclude \
             -Iinclude/core \
             -Iinclude/entities \
             -Iinclude/world \
             -Iinclude/ui \
             -Iinclude/systems \
             -Iinclude/utils \
             -I$(SFML_PREFIX)/include

CXXFLAGS  := -std=c++17 $(INCLUDES)
SFMLFLAGS := -L$(SFML_PREFIX)/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

SRC_DIR   := src
OBJ_DIR   := obj

TARGET    := game.exe

SRCS      := $(wildcard $(SRC_DIR)/*/*.cpp)
OBJS      := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(SFMLFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: all clean