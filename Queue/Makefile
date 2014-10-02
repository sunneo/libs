# Project: Queue
# Makefile created by Dev-C++ 4.9.9.2

CPP  = g++
CC   = gcc
OBJ  = obj/main.o obj/queue.o 
LINKOBJ  =  obj/queue.o
LIBS =  -L"/usr/lib"  -s 
INCS =  -I"/usr/include"  -I"include" 
BIN  = bin/Queue.a
EXEC = bin/QueueDemo
CFLAGS = $(INCS)   -O3 -funroll-all-loops
RM = rm -f

all: bin/Queue.a

test: $(EXEC)
	./$(EXEC)
$(EXEC): $(BIN) $(OBJ)
	$(CC) -o $(EXEC) obj/main.o $(BIN)
clean: 
	${RM} $(OBJ) $(BIN)

$(BIN):	$(LINKOBJ)
	ar r $(BIN) $(LINKOBJ)
	ranlib $(BIN)

obj/main.o: src/main.c
	$(CC) -c src/main.c -o obj/main.o $(CFLAGS)

obj/queue.o: src/queue.c
	$(CC) -c src/queue.c -o obj/queue.o $(CFLAGS)
