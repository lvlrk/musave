CXX=g++
CXXFLAGS=-std=c++2b -Iinclude -Ilib/raylib/src -Ilib/raygui/src -Ilib/zlib/include -Ilib/json/include -Ilib/unpac/include -Wno-enum-compare -Wno-narrowing
LDFLAGS=-lz -lunpac -lraylib
TARGET=musave

ifeq ($(OS),Windows_NT)
TARGET=musave.exe
LDFLAGS+=-static-libstdc++
endif

all: $(TARGET)

$(TARGET): src/*.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $(TARGET)

install: $(TARGET)
	cp $(TARGET) /bin

clean:
	rm -f $(TARGET)

.PHONY: all clean install
