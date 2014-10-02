CC:=gcc
INC:=-I"include"
BIN:= bin/main
OBJ:= obj/iter.o obj/powerset.o obj/main.o 
OPT:= -O3 -funroll-all-loops 

all: $(BIN)

clean: $(BIN) $(OBJ)
	@rm -fv $(BIN) $(OBJ)

remake: 
	@make clean
	@make all

$(BIN): $(OBJ)
	$(CC) -o $(BIN) $(OBJ) 

obj/iter.o: src/iter.c
	$(CC) -c src/iter.c -o obj/iter.o $(OPT) $(INC)

obj/powerset.o: src/powerset.c
	$(CC) -c src/powerset.c -o obj/powerset.o $(OPT) $(INC)

obj/main.o: src/main.c
	$(CC) -c src/main.c -o obj/main.o $(OPT) $(INC)

