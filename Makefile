MPICC = mpicc
LADCOMP = ladcomp -env
GCC = gcc
MPI_FILE = bubble_fases.c
MPI_EXECUTABLE = bubble_fases
FILE = bubble.c
EXECUTABLE = bubble

seq: $(FILE)
	$(GCC) -o $(EXECUTABLE) $(FILE)

mpi: $(MPI_FILE)
	$(MPICC) -o $(MPI_EXECUTABLE) $(MPI_FILE)

lad: $(MPI_FILE)
	$(LADCOMP) $(MPICC) -o $(MPI_EXECUTABLE) $(MPI_FILE)

clean:
	rm -f bubble bubble_fases

build: clean seq mpi

build_lad: clean seq lad