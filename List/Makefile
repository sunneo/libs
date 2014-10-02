# Project: List
# Makefile created by Dev-C++ 4.9.9.2

CC   = gcc
OBJ  = obj/list.o obj/main.o obj/listiter.o
LINKOBJ  = obj/list.o obj/listiter.o
LIBS =  -L"/usr/lib"  -s 
INCS =  -I"/usr/include"  -I"include" 
BIN  = bin/List.a
EXEC = bin/ListDemo
CFLAGS = $(INCS)   -ansi -fexpensive-optimizations -O3
RM = rm -f

all: bin/List.a
test: $(EXEC)
	./$(EXEC)
	@echo ""
	@echo "Press any key to continue..."
	@read null
remake:
	@make clean
	@make all
$(EXEC): $(OBJ)
	$(CC) -o $(EXEC) $(LINKOBJ) obj/main.o
clean: 
	${RM} $(OBJ) $(BIN)

$(BIN): $(LINKOBJ)
	ar r $(BIN) $(LINKOBJ)
	ranlib $(BIN)

obj/list.o: src/list.c
	$(CC) -c src/list.c -o obj/list.o $(CFLAGS)

obj/main.o: src/main.c
	$(CC) -c src/main.c -o obj/main.o $(CFLAGS)

obj/listiter.o: src/listiter.c
	$(CC) -c src/listiter.c -o obj/listiter.o $(CFLAGS)
