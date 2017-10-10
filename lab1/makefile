CFLAGS = -g -Wall -Wstrict-prototypes
PROGS = make-list print-list serial-add-list
LDFLAGS = -lm
CC = gcc
MCC = mpicc

all: $(PROGS)

make-list: make-list.o 
	$(CC) $(LDFLAGS) -o make-list make-list.o 

print-list: print-list.o
	$(CC) $(LDFLAGS) -o print-list print-list.o 

serial-add-list: serial-add-list.o
	$(CC) $(LDFLAGS) -o serial-add-list serial-add-list.o 

parallel-add-list: parallel-add-list.o
	$(MCC) $(LDFLAGS) -o parallel-add-list parallel-add-list.o

make-list.o: make-list.c
	$(CC) $(CFLAGS) -c make-list.c

print-list.o: print-list.c
	$(CC) $(CFLAGS) -c print-list.c

serial-add-list.o: serial-add-list.c
	$(CC) $(CFLAGS) -c serial-add-list.c

parallel-add-list.o: parallel-add-list.c
	$(MCC) $(CFLAGS) -c parallel-add-list.c

clean:
	rm -f $(PROGS) *.o core* *.dat
