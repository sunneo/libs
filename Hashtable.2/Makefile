gcc := gcc
gccd := $(gcc) -DDEBUG
srcdir := src
objdir := obj
bindir := bin
objds := $(objdir)/hashtabled.o
objs := $(objdir)/hashtable.o 
linkopt := 
opt := -O3
inc := -I"include" 
flags := $(linkopt) $(opt) $(inc) 

solib := $(bindir)/libhashtable.so 
solibd := $(bindir)/libhashtabled.so
staticlib := $(bindir)/libhashtable.a 
staticlibd := $(bindir)/libhashtabled.a
out  := $(solib) $(staticlib) 
test := $(bindir)/test

all: $(out) $(test)

clean: 
	rm -fv $(objs) $(out)

remake:
	make clean
	make all

$(test): $(solib) test/test.c
	$(gcc) -o $(test) test/test.c $(solib) $(flags)

$(solib): $(objs)
	$(gcc) -shared -fPIC -o $(solib) $(objs) 

$(solibd): $(objds)
	$(gccd) -shared -fPIC  -o $(solibd) $(objds)

$(staticlib): $(objs)
	ar rcs $(staticlib) $(objs)
	ranlib $(staticlib)

$(staticlibd): $(objds)
	ar rcs $(staticlibd) $(objds)
	ranlib $(staticlibd)

$(objdir)/hashtable.o: $(srcdir)/hashtable.c
	$(gcc) -c $(srcdir)/hashtable.c -o $(objdir)/hashtable.o  $(flags)
$(objdir)/hashtabled.o: $(srcdir)/hashtable.c
	$(gccd) -c $(srcdir)/hashtable.c -o $(objdir)/hashtabled.o $(flags)
