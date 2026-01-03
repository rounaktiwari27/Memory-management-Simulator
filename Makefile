CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

SRC = src/main.cpp src/physical_memory.cpp src/buddy_allocator.cpp src/cache.cpp src/virtual_memory.cpp
OUT = memsim

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

clean:
	rm -f $(OUT)