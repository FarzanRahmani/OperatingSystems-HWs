sort_with_thread:
	gcc sort_with_thread.c -o sort_with_thread -pthread

sort_with_thread.out: sort_with_thread.o
	gcc sort_with_thread.o -o sort_with_thread.out -pthread

sort_with_thread.o: sort_with_thread.c
	gcc -c sort_with_thread.c -pthread

clean:
	rm *.o *.out sort_with_thread

run:
	./sort_with_thread