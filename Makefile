CXX ?= g++
CPPFLAGS ?= -Iinclude -Isource
CXXFLAGS ?= -std=c++17 -Wall -Wextra -O3
TARGET := test_app
SRC := test.cpp

.PHONY: all build run clean

all: $(TARGET)

$(TARGET): $(SRC) $(wildcard include/*.hpp) $(wildcard source/*.tpp)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(SRC) -o $(TARGET)

build: $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
