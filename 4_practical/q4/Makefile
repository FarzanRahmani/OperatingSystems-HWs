compileMother:
	gcc -o Mother DistributedMergeSortMother.c -lrt -pthread

compileChildren:
	gcc -o Children DistributedMergeSortChildren.c -lrt -pthread

runMother:
	./Mother

runChildren:
	./Children

clean:
	rm -f Mother Children