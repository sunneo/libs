# Project: BMP
# Makefile created by Dev-C++ 4.9.9.2

CC   = gcc
OBJ  = obj/bmp.o obj/main.o
LINKOBJ  = obj/bmp.o lib/Queue/bin/Queue.a
INCS =  -I"/usr/include"  -I"include"  -I"lib/Queue/include"
BIN  = bin/BMP.a
CFLAGS = $(INCS)   -ansi -fexpensive-optimizations -O3
RM = rm -f

.PHONY: all all-before all-after clean clean-custom

all:	all-before bin/BMP.a all-after
	

clean: clean-custom
	${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	ar r "bin/libBMP.a"  ${LINKOBJ}

obj/bmp.o: src/bmp.c
	$(CC) -c src/bmp.c -o obj/bmp.o $(CFLAGS)

obj/main.o: src/main.c
	$(CC) -c src/main.c -o obj/main.o $(CFLAGS)
