C=gcc
CFLAGS=-Wall -lcurses -pthread
DEPS=centipede.h console.h player.h gameglobals.h threadwrappers.h keyboard.h
OBJS=main.o centipede.o console.o player.o gameglobals.o threadwrappers.o keyboard.o
CFILES=main.c centipede.c console.c player.c gameglobals.c threadwrappers.c keyboard.c
DEBUG=a.out
EXE=centipede

main: $(OBJS)
	$(CC) -o $(EXE) $(OBJS) $(CFLAGS)

main.o: main.c $(DEPS)
	$(CC) -c main.c $(CFLAGS)

centipede.o: centipede.c $(DEPS)
	$(CC) -c centipede.c $(CFLAGS)

console.o: console.c $(DEPS)
	$(CC) -c console.c $(CFLAGS)

player.o: player.c $(DEPS)
	$(CC) -c player.c $(CFLAGS)

gameglobals.o: gameglobals.c $(DEPS)
	$(CC) -c gameglobals.c $(CFLAGS)

threadwrappers.o: threadwrappers.c $(DEPS)
	$(CC) -c threadwrappers.c $(CFLAGS)

keyboard.o: keyboard.c $(DEPS)
	$(CC) -c keyboard.c $(CFLAGS)

debug:
	$(CC) -g $(CFILES) $(CFLAGS)
	gdb a.out
	rm -f a.out

clean:
	rm -f $(EXE) $(OBJS) $(DEBUG)
