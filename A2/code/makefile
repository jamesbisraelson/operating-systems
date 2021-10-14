C=gcc
CFLAGS=-Wall
DEPS=cmds.h signal.h
OBJS=myshell.o cmds.o signal.o
CFILES=myshell.c cmds.c signal.c
DEBUG=a.out
EXE=myshell

myshell: $(OBJS)
	$(CC) -o $(EXE) $(OBJS) $(CFLAGS)

myshell.o: myshell.c $(DEPS)
	$(CC) -c myshell.c $(CFLAGS)

cmds.o: cmds.c $(DEPS)
	$(CC) -c cmds.c $(CFLAGS)

signal.o: signal.c $(DEPS)
	$(CC) -c signal.c $(CFLAGS)

debug:
	$(CC) -g $(CFILES) $(CFLAGS)
	gdb a.out
	rm -f a.out
clean:
	rm -f $(EXE) $(OBJS) $(DEBUG)
