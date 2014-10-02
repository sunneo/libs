# Project: Stack
# Makefile created by Dev-C++ 4.9.9.2

CPP  = g++
CC   = gcc
OBJ  = obj/main.o obj/stack.o
LINKOBJ  = obj/main.o obj/stack.o
LIBS =  -L"/usr/lib"  -s 
INCS =  -I"/usr/include"  -I"include" 
BIN  = bin/Stack
CXXFLAGS = $(CXXINCS)   -ansi -fexpensive-optimizations -O3
CFLAGS = $(INCS)   -ansi -fexpensive-optimizations -O3
RM = rm -f

.PHONY: all all-before all-after clean clean-custom

all: all-before bin/Stack all-after


clean: clean-custom
	${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CC) $(LINKOBJ) -o "bin/Stack" $(LIBS)

obj/main.o: src/main.c
	$(CC) -c src/main.c -o obj/main.o $(CFLAGS)

obj/stack.o: src/stack.c
	$(CC) -c src/stack.c -o obj/stack.o $(CFLAGS)
