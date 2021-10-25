C=gcc
CFLAGS=-Wall
DEPS=cmds.h signals.h printing.h
OBJS=myshell.o cmds.o signals.o printing.o
CFILES=myshell.c cmds.c signals.c printing.c
DEBUG=a.out
EXE=myshell

myshell: $(OBJS)
	$(CC) -o $(EXE) $(OBJS) $(CFLAGS)

myshell.o: myshell.c $(DEPS)
	$(CC) -c myshell.c $(CFLAGS)

cmds.o: cmds.c $(DEPS)
	$(CC) -c cmds.c $(CFLAGS)

signals.o: signals.c $(DEPS)
	$(CC) -c signals.c $(CFLAGS)

printing.o: printing.c $(DEPS)
	$(CC) -c printing.c $(CFLAGS)

debug:
	$(CC) -g $(CFILES) $(CFLAGS)
	gdb a.out
	rm -f a.out
clean:
	rm -f $(EXE) $(OBJS) $(DEBUG)
