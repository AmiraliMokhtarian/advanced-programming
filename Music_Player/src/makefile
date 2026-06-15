CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

SRC = main.cpp \
      song.cpp \
      MusicLibrary.cpp \
      csvLoader.cpp \
      playlist.cpp \
      m3uLoader.cpp

TARGET = player

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)