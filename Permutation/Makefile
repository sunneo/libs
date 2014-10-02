CC:=gcc
INC:=-I"include"
BIN:= bin/main
OBJ:= obj/arrayiter.o obj/iter.o obj/permutation.o obj/main.o 
OPT:= -O3 -funroll-all-loops 

all: $(BIN)

clean: $(BIN) $(OBJ)
	@rm -fv $(BIN) $(OBJ)

remake: 
	@make clean
	@make all

$(BIN): $(OBJ)
	$(CC) -o $(BIN) $(OBJ) 

obj/arrayiter.o: src/arrayiter.c
	$(CC) -c src/arrayiter.c -o obj/arrayiter.o $(OPT) $(INC)

obj/iter.o: src/iter.c
	$(CC) -c src/iter.c -o obj/iter.o $(OPT) $(INC)

obj/permutation.o: src/permutation.c
	$(CC) -c src/permutation.c -o obj/permutation.o $(OPT) $(INC)

obj/main.o: src/main.c
	$(CC) -c src/main.c -o obj/main.o $(OPT) $(INC)

