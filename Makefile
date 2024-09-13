# Makefile for compiling and running a .cpp file with 4 arguments

# Compiler
CCC = g++

# Compiler flag
CF = -Wall -std=c++11

# Executable
EX = ex

# Threads
THREADS = -pthread

# SRC Files
SRCS = main.cpp barrier.cpp

# Arg
ARG1 = 1000
ARG2 = 96


# Running
run:	$(EX)
	./$(EX) $(ARG1) $(ARG2) 

# Compiling the program
$(EX):	$(SRCS)
	$(CCC) $(CF) $(THREADS) $(SRCS) -o $(EX) 

# Cleaning up
clean:
	rm -f $(EX)
