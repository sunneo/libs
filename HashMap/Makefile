gcc := gcc
gccd := $(gcc) -DDEBUG
srcdir := src
objdir := obj
bindir := bin
objds := $(objdir)/hashmapd.o
objs := $(objdir)/hashmap.o 
linkopt := 
opt := -O3
inc := -I"include" -I"lib/BinTree/include"
flags := $(linkopt) $(opt) $(inc)

solib := $(bindir)/libhashmap.so 
solibd := $(bindir)/libhashmapd.so
staticlib := $(bindir)/libhashmap.a 
staticlibd := $(bindir)/libhashmapd.a
DEPSO := bin/libbst.so
DEPSOD := bin/libbstd.so
out  := $(solib) $(staticlib) $(solibd) $(staticlibd)
test := $(bindir)/test $(bindir)/test_d

all: $(out) $(test)

remake: 
	make clean
	make all

clean: 
	echo "cd lib/BinTree" >  makeCleanRecursive
	echo "make clean" >> makeCleanRecursive
	rm -fv $(objs) bin/* obj/*
	bash makeCleanRecursive
	rm makeCleanRecursive

$(bindir)/test_d: $(solibd) test/test.c
	$$(export LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:./:bin/)
	$(gccd) -DDEBUG -o $(bindir)/test_d test/test.c  $(flags) -L./ -Lbin -lhashmapd


$(bindir)/test: $(solibd) test/test.c
	$$(export LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:./:bin/)
	$(gcc) -o $(bindir)/test test/test.c  $(flags) -L./ -Lbin bin/libhashmap.a bin/libbst.a

$(solib): $(objs) $(DEPSO)
	$(gcc) -shared -fPIC -o $(solib) $(objs) -Lbin/ -lbst

$(solibd): $(objds) $(DEPSOD)
	$(gccd) -shared -fPIC -o $(solibd) $(objds) -Lbin/  -lbstd

$(staticlib): $(objs) bin/libbst.a
	ar rcsf $(staticlib) $(objs) bin/libbst.a
	ranlib $(staticlib)

$(staticlibd): $(objds) bin/libbstd.a
	ar rcsf $(staticlibd) $(objds) bin/libbstd.a
	ranlib $(staticlibd)

$(DEPSO) $(DEPSOD) bin/libbstd.a bin/libbst.a: lib/BinTree/
	echo "cd lib/BinTree" > makeBst
	echo "make all" >>makeBst
	echo "cd bin/" >>makeBst
	echo 'for all in $$(ls *.so *.a) ; do if [ -e $$(pwd)/../../../bin/$$all ] ; then \
	   	rm $$(pwd)/../../../bin/$$all;\
		fi;\
		ln -s $$(pwd)/$$all $$(pwd)/../../../bin/$$all ; \
		done' >>makeBst;
	bash makeBst
	rm makeBst

$(objdir)/hashmap.o: $(srcdir)/hashmap.c
	$(gcc) -c $(srcdir)/hashmap.c -o $(objdir)/hashmap.o $(flags)
$(objdir)/hashmapd.o: $(srcdir)/hashmap.c
	$(gccd) -c $(srcdir)/hashmap.c -o $(objdir)/hashmapd.o $(flags)
