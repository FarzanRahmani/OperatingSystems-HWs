q4:
	gcc q4.c -pthread -o q4

q4.out: q4.o
	gcc q4.o -o q4.out

q4.o: q4.c
	gcc -c q4.c

clean:
	rm *.o *.out q4

run:
	./q4
