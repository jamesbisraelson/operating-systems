/*
 * shell.h:
 *
 * The .h file for shell.c.
 *
 */
#ifndef SHELL_H
#define SHELL_H
#include "fat32.h"

//the main loop for the shell of the fat32 program
void shellLoop(int fd);

//prints the info about the disk image
void printInfo(fat32Head* h);

//prints the info about a directory
void doDir(fat32Head* h, uint32_t curDirClus);

//returns the cluster of another directory specified in command
uint32_t doCD(fat32Head* h, uint32_t curDirClus, char* command);

//downloads the file specified in command to the hard drive
void doDownload(fat32Head* h, uint32_t curDirClus, char* command);
#endif
