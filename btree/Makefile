FND_VERSION=4
TEST=test
#1: pth
#2: plain
#3: cuda
#4: cuda

all: libbpt.so test

clean:
	rm -f libbpt.so $(OBJS)
cleanobj:
	rm -f $(OBJS)

remake:
	make clean
	make all

OBJS=bpt.o bpt_cu.o bpt_pth.o

#test: bpt_test.c libbpt.so
#	gcc -o test bpt_test.c -L./ -lbpt 
#
test: bpt_test.c $(OBJS)
	gcc -o test bpt_test.c $(OBJS) -L/usr/local/cuda/lib -lcudart -lpthread

libbpt.so: $(OBJS)
	gcc -shared -fPIC $(OBJS) -o libbpt.so -lpthread -L/usr/local/cuda/lib -lcudart

bpt_cu.o: bpt_cu.cu
	nvcc -c bpt_cu.cu -o bpt_cu.o -O3

bpt_pth.o: bpt_pth.c
	gcc -c bpt_pth.c -o bpt_pth.o

bpt.o: bpt.c
	gcc -c bpt.c -o bpt.o -DFIND_VERSION=$(FND_VERSION)
