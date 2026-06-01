CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2
DEPFLAGS := -MMD -MP

TARGET := installer

OBJS := main.o \
        InstallationEngine.o \
        installable.o \
        module.o \
        package.o \
        systemLog.o

DEPS := $(OBJS:.o=.d)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(DEPS) $(TARGET)

-include $(DEPS)