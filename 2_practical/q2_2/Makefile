server:
	gcc server.c -o server -pthread

server.out: server.o
	gcc server.o -o server.out -pthread

server.o: server.c
	gcc -c server.c -pthread

clean:
	rm *.o *.out server

run:
	./server