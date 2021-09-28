C=gcc
CFLAGS=-Wall
DEPS=cmds.h
OBJS=myshell.o cmds.o
EXE=myshell

myshell: $(OBJS)
	$(CC) -o $(EXE) $(OBJS) $(CFLAGS)

myshell.o: myshell.c $(DEPS)
	$(CC) -c myshell.c $(CFLAGS)

cmds.o: cmds.c $(DEPS)
	$(CC) -c cmds.c $(CFLAGS)

clean:
	rm -f $(EXE) $(OBJS)
