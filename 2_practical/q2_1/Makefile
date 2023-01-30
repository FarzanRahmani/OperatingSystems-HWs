server:
	gcc server.c -o server -pthread

server.out: server.o
	gcc server.o -o server.out

server.o: server.c
	gcc -c server.c

clean:
	rm *.o *.out server

run:
	./server