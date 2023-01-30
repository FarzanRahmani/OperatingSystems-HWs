q3:
	gcc q3.c -pthread -o q3

q3.out: q3.o
	gcc q3.o -o q3.out

q3.o: q3.c
	gcc -c q3.c

clean:
	rm *.o *.out q3

run:
	./q3
