# Project: HashTable
# Makefile created by Dev-C++ 4.9.9.2

CC   = gcc
OBJ  = obj/main.o obj/hashtbl.o obj/hashkey_iter.o
LINKOBJ  = obj/hashtbl.o obj/hashkey_iter.o
LIBS =   -L"/usr/lib" -L"lib" lib/List/bin/List.a  -s 
INCS =  -I"/usr/include"  -I"include" -I"lib/List/include"
BIN  = bin/HashTable.a
DEMO	=	bin/HashTableDemo
CFLAGS = $(INCS)  -O3 -funroll-all-loops
RM = rm -f

all: bin/HashTable.a
test: ${DEMO}
	./${DEMO}


clean: 
	${RM} $(OBJ) $(BIN)

remake:
	@make clean
	@make all
lib/List/bin/List.a: include lib/List/Makefile

bin/HashTableDemo:	${OBJ} ${BIN} lib/List/bin/List.a
	${CC} -o ${DEMO} ${OBJ} ${LIBS} ${BIN}

$(BIN): $(LINKOBJ)
	ar rcf $(BIN) $(LINKOBJ)
	ranlib $(BIN)

obj/main.o: src/main.c
	$(CC) -c src/main.c -o obj/main.o $(CFLAGS)

obj/hashtbl.o: src/hashtbl.c
	$(CC) -c src/hashtbl.c -o obj/hashtbl.o $(CFLAGS)

obj/hashkey_iter.o: src/hashkey_iter.c
	$(CC) -c src/hashkey_iter.c -o obj/hashkey_iter.o $(CFLAGS)
