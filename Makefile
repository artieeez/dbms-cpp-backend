
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Wpedantic

all:
	$(CXX) -o cpd-fa.exe src/main.cpp $(CXXFLAGS)

run:
	./cpd-fa.exe
