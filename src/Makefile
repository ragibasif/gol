CXX := clang++
CXXFLAGS := -std=c++20 -O2 -Wall -Wextra

.PHONY: default
default: all

.PHONY: all
all: main

main: main.o
	@$(CXX) $(CXXFLAGS) main.o -o main

main.o: main.cpp
	@$(CXX) $(CXXFLAGS) -c main.cpp -o main.o
	@$(CXX) $(CXXFLAGS) -S main.cpp -o main.s

.PHONY: run
run:
	@make clean
	@make all
	@./main
	@make clean

.PHONY: clean
clean:
	@rm -rf main main.o main.s
