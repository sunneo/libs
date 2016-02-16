OBJS:= coroutine.o main.o

all: main

clean:
	rm -f $(OBJS) main 

main: $(OBJS)
	gcc -o $@ $(OBJS)
