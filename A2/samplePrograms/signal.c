/*
  COMP 3430 Operating Systems
  signal.c
  This program sends the SIGUSR1 signal to the process
  specified on the command line
*/
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
        int i = 0;

        for (i = 0; i < 4; i++)
        {
                kill(atoi(argv[1]), SIGUSR1);
                sleep(i);
        }

} // end main

