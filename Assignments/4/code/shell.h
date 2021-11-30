#ifndef SHELL_H
#define SHELL_H
#include "fat32.h"

void shellLoop(int fd);
void printInfo(fat32Head* h);
void doDir(fat32Head* h, uint32_t curDirClus);
uint32_t doCD(fat32Head* h, uint32_t curDirClus, char* command);
#endif
