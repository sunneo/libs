# Project: Vector
# Makefile created by Dev-C++ 4.9.9.2

CC   = gcc
OBJ  = obj/main.o obj/vector.o obj/vector_iter.o
LINKOBJ  = obj/vector.o obj/vector_iter.o
LIBS =  -L"/usr/lib"  -s 
INCS =  -I"/usr/include" -I"include"
EXEC = bin/VectorDemo
BIN  = bin/Vector.a
CFLAGS = $(INCS)   -fexpensive-optimizations -O3 -funroll-all-loops
RM = rm -f


all:  bin/Vector.a 
test: $(EXEC)
	./$(EXEC)
	@echo 'Press any key to continue...'
	@read null
clean:
	${RM} $(OBJ) $(BIN) $(EXEC)
$(EXEC): $(BIN) $(OBJ)
	$(CC) -o $(EXEC) obj/main.o $(BIN)
$(BIN): $(LINKOBJ)
	ar rcs $(BIN) $(LINKOBJ)
	ranlib $(BIN)

obj/main.o: src/main.c
	$(CC) -c src/main.c -o obj/main.o $(CFLAGS)

obj/vector.o: src/vector.c
	$(CC) -c src/vector.c -o obj/vector.o $(CFLAGS)

obj/vector_iter.o: src/vector_iter.c
	$(CC) -c src/vector_iter.c -o obj/vector_iter.o $(CFLAGS)
