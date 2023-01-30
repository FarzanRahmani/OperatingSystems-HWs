q2_semaphore:
	gcc q2_semaphore.c -pthread -o q2_semaphore

q2_semaphore.out: q2_semaphore.o
	gcc q2_semaphore.o -o q2_semaphore.out

q2_semaphore.o: q2_semaphore.c
	gcc -c q2_semaphore.c

clean:
	rm *.o *.out q2_semaphore

run:
	./q2_semaphore
