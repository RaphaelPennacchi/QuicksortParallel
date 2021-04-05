MPICC = mpicc
cc = gcc
ccflags = -Wall -O3 -lpthread -fopenmp
target = generator sequencial openmp mpi

all: generator sequencial openmp mpi
	echo "Compiled"
generator:
	$(CC) $(CCFLAGS) -o generator.o generateVector.c
sequencial:
	$(CC) $(CCFLAGS) -o quicksortSequencial.o quicksortSequencial.c sortLibrary.c
openmp:
	$(CC) $(CCFLAGS) -o quicksortParallel.o quicksortParallel.c sortLibrary.c
mpi:
	$(MPICC) $(CCFLAGS) -o quicksortMPI.o quicksortMPI.c sortLibrary.c
generateVector:
	./generator.o 10
	./generator.o 100
	./generator.o 1000
	./generator.o 10000
	./generator.o 100000
	./generator.o 1000000
	./generator.o 10000000

clean:
	rm -rf *.o *.map