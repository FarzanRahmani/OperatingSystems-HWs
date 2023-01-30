read_file:
	gcc read_file.c -o read_file 

read_file.out: read_file.o
	gcc read_file.o -o read_file.out 

read_file.o: read_file.c
	gcc -c read_file.c 

clean:
	rm *.o *.out read_file

run:
	./read_file file_name.txt