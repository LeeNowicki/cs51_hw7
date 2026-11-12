all : wrapper

wrapper.o: wrapper.c
	gcc -no-pie -c -g wrapper.c

fun.o: fun.S
	gcc -no-pie -c fun.S

wrapper: wrapper.o fun.o
	gcc -no-pie -o wrapper wrapper.o fun.o

clean:
	rm -f *.o *~ wrapper
