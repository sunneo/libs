CC = gcc
OBJ = obj/heap_nr.o obj/main.o
LINKOBJ = obj/heap_nr.o
BIN = bin/HeapNR.a
INC = -I"include" -I"/usr/include"
LIB = -L"/usr/lib"
EXEC = bin/heapNRDemo
CFLAGS = $(INC) -O3 -funroll-all-loops
RM = rm -f

all: $(BIN)
clean: 
	$(RM) $(OBJ) $(BIN)
remake:
	@make clean
	@make all
test: $(EXEC)
	./$(EXEC)
	
$(BIN): $(OBJ)
	ar rcs $(BIN) $(LINKOBJ)
$(EXEC): $(BIN)
	$(CC) -o $(EXEC) obj/main.o $(BIN)
	
obj/heap_nr.o: src/heap_nr.c
	$(CC) -c src/heap_nr.c -o obj/heap_nr.o $(LIB) $(CFLAGS)
obj/main.o: src/main.c
	$(CC) -c src/main.c -o obj/main.o $(LIB) $(CFLAGS) 
