C=gcc
CFLAGS=-Wall -std=gnu99
DEPS=fat32.h
OBJS=main.o fat32.o shell.o
CFILES=main.c fat32.c shell.c
DEBUG=a.out
EXE=fat32
IMG=diskimage
DOWNLOADS=TRAVEL.JPG FOOD.JPG STORY.TXT 1.JPG

main: $(OBJS)
	$(CC) -o $(EXE) $(OBJS) $(CFLAGS)

main.o: main.c $(DEPS)
	$(CC) -c main.c $(CFLAGS)

fat32.o: fat32.c $(DEPS)
	$(CC) -c fat32.c $(CFLAGS)

shell.o: shell.c $(DEPS)
	$(CC) -c shell.c $(CFLAGS)

debug:
	$(CC) -g3 $(CFILES) $(CFLAGS)
	gdb --args a.out $(IMG)
	rm -f a.out

valgrind: main
	valgrind -s --leak-check=full --track-origins=yes ./$(EXE) $(IMG)

clean:
	rm -f $(DOWNLOADS) $(EXE) $(OBJS) $(DEBUG)

run: main
	./$(EXE) $(IMG)
