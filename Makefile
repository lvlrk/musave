CXX=g++
CXXFLAGS=-std=c++2b -Wno-enum-compare -Wno-narrowing
INCLUDES=-Iinclude/raygui/src -Iinclude/unpac/include
LDFLAGS=
TARGET=musave

# LINUX or WINDOWS
PLATFORM_OS ?= LINUX

# x86_64-w64-mingw32-g++ src/*.cpp -Iinclude/unpac/include
# -Iinclude/raygui/src -Llib -lraylib -lz -std=c++2b
#  -Wno-narrowing -Wno-enum-compare -lm -pthread
#  -lgdi32 -luser32 -lkernel32 -lwinmm
#  -Llib/static/windows-x86_64 -lpthread -lwinpthread
#  -lunpac -static-libstdc++ -static-libgcc -static

ifeq ($(PLATFORM_OS),WINDOWS)
CXX=x86_64-w64-mingw32-g++
LDFLAGS=-static -Llib -lraylib -lunpac -lz -lgdi32 -lwinmm
TARGET=musave.exe
else
LDFLAGS=-lraylib -lunpac -lz
endif

all: $(TARGET)

$(TARGET): src/*.cpp
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) $(INCLUDES) -o $(TARGET)

install: $(TARGET)
	cp $(TARGET) /bin

clean:
	rm -f $(TARGET)

.PHONY: all clean install
