# Project: BST
# Makefile created by Dev-C++ 4.9.9.2

CPP  = g++
CC   = gcc
OBJ  = obj/bst.o obj/bst_iter.o obj/main.o 
LINKOBJ  = obj/bst.o obj/bst_iter.o
LINKOBJD = obj/bstd.o obj/bst_iterd.o
LIBS =  -L"/usr/lib"  -s 
DEP = "lib/Stack/bin/Stack.a"
DEPSO = bin/libstack.so 
DEPSOD = bin/libstackd.so 

INCS =  -I"/usr/include"  -I"lib"  -I"include" -I"lib/Stack/include"
BIN  = bin/BST.a bin/libbst.so bin/libbstd.so
EXEC = bin/BSTDEMO
CHANGE_LD_PATH= export LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:./:bin/:lib/
PROB_LD_PATH = -L./ -Lbin/ -Llib/ 
CFLAGS = $(INCS)    -O3 -funroll-all-loops
SRC = src/bst_iter.c src/bst.c
RM = rm -f


all:  bin/libbst.a bin/libbstd.a bin/libbst.so bin/libbstd.so

test: $(EXEC)
	./$(EXEC)
	@echo 'Press any key to continue...'
	@read null
clean: 
	echo "cd lib/Stack" > makeCleanRecursive
	echo "make clean" >> makeCleanRecursive
	${RM} bin/* obj/*
	bash makeCleanRecursive
	rm makeCleanRecursive
remake:
	make clean
	make all

bin/libbst.a: $(LINKOBJ) bin/libstack.a
	ar rcf bin/libbst.a $(LINKOBJ) 
	ranlib bin/libbst.a

bin/libbstd.a: $(LINKOBJD) bin/libstackd.a
	ar rcf bin/libbstd.a $(LINKOBJ) bin/libstackd.a
	ranlib bin/libbstd.a

$(EXEC): $(BIN) $(OBJ)
	$(CC) -o $(EXEC) obj/main.o $(BIN) $(DEP)

obj/bst.o obj/bstd.o: src/bst.c
	$(CC) -c src/bst.c -o obj/bst.o $(CFLAGS)
	$(CC) -DDEBUG -c src/bst.c -o obj/bstd.o $(CFLAGS)

obj/bst_iter.o obj/bst_iterd.o: src/bst_iter.c
	$(CC) -c src/bst_iter.c -o obj/bst_iter.o $(CFLAGS)
	$(CC) -DDEBUG -c src/bst_iter.c -o obj/bst_iterd.o $(CFLAGS)


obj/main.o: src/main.c
	$(CC) -c src/main.c -o obj/main.o $(CFLAGS)

$(DEPSO): lib/Stack/
	echo "cd lib/Stack/" > makeStk
	echo "make bin/libstack.so" >> makeStk
	bash makeStk
	if [ -e $$(pwd)/bin/libstack.so ] ; then rm $$(pwd)/bin/libstack.so ; fi 
	ln -s $$(pwd)/lib/Stack/bin/libstack.so $$(pwd)/bin/libstack.so
	rm makeStk
$(DEPSOD): lib/Stack/
	echo "cd lib/Stack/" > makeStkd
	echo "make bin/libstackd.so" >> makeStkd
	bash makeStkd
	if [ -e $$(pwd)/bin/libstackd.so ] ; then rm $$(pwd)/bin/libstackd.so ; fi 
	ln -s $$(pwd)/lib/Stack/bin/libstackd.so $$(pwd)/bin/libstackd.so
	rm makeStkd
bin/libstack.a : lib/Stack/
	echo "cd lib/Stack/" > makeStkA
	echo "make bin/libstack.a" >> makeStkA
	bash makeStkA
	if [ -e $$(pwd)/bin/libstack.a ] ; then rm $$(pwd)/bin/libstack.a ; fi 
	ln -s $$(pwd)/lib/Stack/bin/libstack.a $$(pwd)/bin/libstack.a
	rm makeStkA
bin/libstackd.a : lib/Stack/
	echo "cd lib/Stack/" > makeStkAd
	echo "make bin/libstackd.a" >> makeStkAd
	bash makeStkAd
	if [ -e $$(pwd)/bin/libstackd.a ] ; then rm $$(pwd)/bin/libstackd.a ; fi 
	ln -s $$(pwd)/lib/Stack/bin/libstackd.a $$(pwd)/bin/libstackd.a
	rm makeStkAd
bin/libbst.so: $(LINKOBJ) $(DEPSO)
	$(CC) -shared -fPIC $(LINKOBJ) -o bin/libbst.so -Lbin/  -lstack
bin/libbstd.so: $(SRC) $(DEPSOD)
	gcc -DDEBUG -fPIC -shared $(SRC) -o bin/libbstd.so $(CFLAGS) -Lbin/ -lstackd

