CC = gcc

BASEFLAGS = -Wall -pthread -std=c99
NODEBUG_FLAGS = -dNDEBUG 
DEBUG_FLAGS = -g

LDLIBS = -lcurses -pthread

OBJS = main.o console.o centipede.o player.o threadwrappers.o

EXE = centipede

debug: CFLAGS = $(BASEFLAGS) $(DEBUG_FLAGS)
debug: $(EXE)

release: CFLAGS = $(BASEFLAGS) $(NODEBUG_FLAGS) 
release: $(EXE)

$(EXE): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $(EXE) $(LDLIBS)

main.o: main.c centipede.h
	$(CC) $(CFLAGS) -c main.c


player.o: player.c player.h centipede.h console.h threadwrappers.h
	$(CC) $(CFLAGS) -c player.c

console.o: console.c console.h
	$(CC) $(CFLAGS) -c console.c


threadwrappers.o: threadwrappers.c threadwrappers.h
	$(CC) $(CFLAGS) -c threadwrappers.c

centipede.o: centipede.h centipede.c console.h
	$(CC) $(CFLAGS) -c centipede.c

clean:
	rm -f $(OBJS)
	rm -f *~
	rm -f $(EXE)
	rm -f $(EXE)_d

run:
	./$(EXE)

run_debug:
	./$(EXE)_d
