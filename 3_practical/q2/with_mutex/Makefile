q2_mutex:
	gcc q2_mutex.c -pthread -o q2_mutex

q2_mutex.out: q2_mutex.o
	gcc q2_mutex.o -o q2_mutex.out

q2_mutex.o: q2_mutex.c
	gcc -c q2_mutex.c

clean:
	rm *.o *.out q2_mutex

run:
	./q2_mutex
