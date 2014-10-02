# Project: Stack
# Makefile created by Dev-C++ 4.9.9.2

CC   = gcc
OBJ  = obj/main.o obj/stack.o
LINKOBJ  = obj/stack.o
LIBS =  -L"/usr/lib"  -s 
INCS =  -I"/usr/include"  -I"include" 
BIN  = bin/libstack.a bin/libstackd.a bin/libstack.so bin/libstackd.so
EXEC = bin/StackDemo
CFLAGS = $(INCS)   -O3 -funroll-all-loops
RM = rm -f
SRC = src/stack.c


all: $(BIN) 

clean: 
	${RM} obj/* bin/*

test: $(EXEC)
	./$(EXEC)

bin/libstack.a: $(OBJ)
	ar rcf bin/libstack.a $(LINKOBJ)
	ranlib bin/libstack.a
bin/libstackd.a: obj/stackd.o
	ar rcf bin/libstackd.a obj/stackd.o
	ranlib bin/libstackd.a

$(EXEC): $(BIN) $(OBJ)
	$(CC) -o $(EXEC) obj/main.o $(BIN)

obj/main.o: src/main.c
	$(CC) -c src/main.c -o obj/main.o $(CFLAGS)

obj/stack.o: src/stack.c
	$(CC) -c src/stack.c -o obj/stack.o $(CFLAGS)

obj/stackd.o: src/stack.c
	$(CC) -DDEBUG -c src/stack.c -o obj/stackd.o $(CFLAGS)

bin/libstack.so: $(SRC)
	$(CC) -shared -fPIC -o bin/libstack.so $(SRC) $(CFLAGS)
bin/libstackd.so: $(SRC)
	$(CC) -DDEBUG -shared -fPIC -o bin/libstackd.so $(SRC) $(CFLAGS)
