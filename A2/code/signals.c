/*
 * file: signals.c
 * ---------------
 * defines the signal handler for myshell
 */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "signals.h"

/* function: catch_signal
 * ----------------------
 * handler that catches a signal and resubscribes to it
 *
 * the_signal: the signal caught
 */
void catch_signal(int the_signal) {
	signal(the_signal, catch_signal);
}
