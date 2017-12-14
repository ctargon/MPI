CFLAGS = -g -Wall -Wstrict-prototypes
PROGS = make-graph print-graph room-serial room-parallel
LDFLAGS = -lm
CC = gcc
MCC = mpicc

all: $(PROGS)

make-graph: make-graph.o graph.o
	$(CC) -o make-graph make-graph.o graph.o $(LDFLAGS)

print-graph: print-graph.o graph.o
	$(CC) -o print-graph print-graph.o graph.o $(LDFLAGS)

room-serial: room-serial.o graph.o
	$(CC) -o room-serial room-serial.o graph.o $(LDFLAGS)

room-parallel: room-parallel.o graph.o MyMPI.o
	$(MCC) -o room-parallel room-parallel.o MyMPI.o graph.o $(LDFLAGS)

make-graph.o: make-graph.c graph.h
	$(CC) $(CFLAGS) -c make-graph.c

print-list.o: print-graph.c graph.h
	$(CC) $(CFLAGS) -c print-graph.c

room-serial.o: room-serial.c graph.h
	$(CC) $(CFLAGS) -c room-serial.c

room-parallel.o: room-parallel.c graph.h MyMPI.h
	$(MCC) $(CFLAGS) -c room-parallel.c

graph.o: graph.c graph.h
	$(CC) $(CFLAGS) -c graph.c

MyMPI.o: MyMPI.c MyMPI.h
	$(MCC) $(CFLAGS) -c MyMPI.c

clean:
	rm -f $(PROGS) *.o core* *.dat *.seq