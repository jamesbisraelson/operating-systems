CC=gcc
CFLAGS=-Wall
DEPS=Process.h Summary.h
OBJS=a1.o
EXE=a1

a1: $(OBJS)
	$(CC) -o $(EXE) $(OBJS) $(CFLAGS)

a1.o: a1.c $(DEPS)
	$(CC) -c a1.c $(CFLAGS)

run:
	./$(EXE)

clean:
	rm -f $(EXE) $(OBJS)
