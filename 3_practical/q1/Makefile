q1:
	gcc q1.c -pthread -o q1

q1.out: q1.o
	gcc q1.o -o q1.out

q1.o: q1.c
	gcc -c q1.c

clean:
	rm *.o *.out q1

run:
	./q1
