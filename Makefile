# This is a simple Makefile for small projects.  When you 
# type make at the command prompt, it will compile the code.
# For more depth, see http://www.gnu.org/software/make/manual/make.html

CC=g++
CFLAGS=-lfreeglut -lopengl32 -lglu32 #-lglut -lGLU -lGL -lm

all: main

main: atlantis.cpp
	$(CC) -o atlantis atlantis.cpp dolphin.cpp shark.cpp whale.cpp swim.cpp  $(CFLAGS)

clean:
	@rm atlantis.exe