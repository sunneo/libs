OBJS:= coroutine.o main.o
CC:=gcc
CFLAGS+=-O0
all: main

clean:
	rm -f $(OBJS) main libjbf.so

main: $(OBJS)
	gcc -o $@ $(OBJS) $(CFLAGS)
libcoroutine.so: coroutine.c
	gcc -shared -fPIC -o libcoroutine.so -O0 coroutine.c
