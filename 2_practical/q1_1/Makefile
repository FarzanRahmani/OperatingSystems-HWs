sort_with_process:
	gcc sort_with_process.c -o sort_with_process

sort_with_process.out: sort_with_process.o
	gcc sort_with_process.o -o sort_with_process.out

sort_with_process.o: sort_with_process.c
	gcc -c sort_with_process.c

clean:
	rm *.o *.out sort_with_process

run:
	./sort_with_process
