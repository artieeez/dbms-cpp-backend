
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Wpedantic

all:
	$(CXX) -o cpf-fa.exe src/main.cpp $(CXXFLAGS)

DFLAGS="-L/opt/homebrew/opt/llvm/lib/c++ -Wl,-rpath,/opt/homebrew/opt/llvm/lib/c++"