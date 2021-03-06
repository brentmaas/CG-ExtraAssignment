# This is a simple Makefile for small projects.  When you 
# type make at the command prompt, it will compile the code.
# For more depth, see http://www.gnu.org/software/make/manual/make.html

CC=g++
CFLAGS=-lglut -lGLU -lGL -lm -std=c++17

all: main

main: atlantis.cpp
	$(CC) -o atlantis atlantis.cpp dolphin.cpp shark.cpp whale.cpp swim.cpp $(CFLAGS) -Wall

clean:
	@rm atlantis.exe
