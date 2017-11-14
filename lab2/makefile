CFLAGS = -g -Wall -Wstrict-prototypes
PROGS = make-graph print-graph floyd-serial
#serial-add-list parallel-add-list
LDFLAGS = -lm
CC = gcc
MCC = mpicc

all: $(PROGS)

make-graph: make-graph.o graph.o
	$(CC) $(LDFLAGS) -o make-graph make-graph.o graph.o

print-graph: print-graph.o graph.o
	$(CC) $(LDFLAGS) -o print-graph print-graph.o graph.o

floyd-serial: floyd-serial.o graph.o
	$(CC) $(LDFLAGS) -o floyd-serial floyd-serial.o graph.o 

# parallel-add-list: parallel-add-list.o MyMPI.o
# 	$(MCC) $(LDFLAGS) -o parallel-add-list parallel-add-list.o MyMPI.o

make-graph.o: make-graph.c graph.h
	$(CC) $(CFLAGS) -c make-graph.c

print-list.o: print-graph.c graph.h
	$(CC) $(CFLAGS) -c print-graph.c

floyd-serial.o: floyd-serial.c graph.h
	$(CC) $(CFLAGS) -c floyd-serial.c

# parallel-add-list.o: parallel-add-list.c MyMPI.h
# 	$(MCC) $(CFLAGS) -c parallel-add-list.c

graph.o: graph.c graph.h
	$(CC) $(CFLAGS) -c graph.c

MyMPI.o: MyMPI.c MyMPI.h
	$(MCC) $(CFLAGS) -c MyMPI.c

clean:
	rm -f $(PROGS) *.o core* *.dat