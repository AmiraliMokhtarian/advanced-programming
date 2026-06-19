CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

SRC = main.cpp \
      song.cpp \
      MusicLibrary.cpp \
      csvLoader.cpp \
      playlist.cpp \
      m3uLoader.cpp \
      player.cpp \
      configManager.cpp \
      UIRender.cpp \
      inputHandler.cpp \
      mainMenu.cpp \
      nowPlaying.cpp \
      playlistScreen.cpp \
      browsePlaylist.cpp \
      settingScreen.cpp \
      Application.cpp

TARGET = player

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) -lpthread -lm -ldl

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)