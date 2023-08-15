CXX=g++
CXXFLAGS=-std=c++2b -Iinclude -Ilib/raylib/src -Ilib/raygui/src -Ilib/zlib/include -Wno-enum-compare -Wno-narrowing
LDFLAGS=-lraylib -lz -lunpac -lfmon
TARGET=musave

all: $(TARGET)

$(TARGET): src/*.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean
