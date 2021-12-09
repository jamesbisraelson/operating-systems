/*
 * shell.c:
 *
 * Holds the code for the main functions of the shell loop.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdbool.h>
#include "shell.h"
#include "fat32.h"

//define constants
#define BUF_SIZE 256
#define CMD_INFO "INFO"
#define CMD_DIR "DIR"
#define CMD_CD "CD"
#define CMD_GET "GET"

//the main loop for the shell of the fat32 program
void shellLoop(int fd) {
	int running = true;
	uint32_t curDirClus;
	char buffer[BUF_SIZE];
	char bufferRaw[BUF_SIZE];

	fat32Head *h = createHead(fd);

	if(h == NULL) {
		running = false;
	}
	else {// valid, grab the root cluster	
		curDirClus = h->bs->BPB_RootClus;
	}	

	while(running) 
	{
		printf(">");
		if(fgets(bufferRaw, BUF_SIZE, stdin) == NULL) 
		{
			running = false;
			break;
		}
		bufferRaw[strlen(bufferRaw)-1] = '\0'; /* cut new line */
		for(int i=0; i < strlen(bufferRaw)+1; i++)
			buffer[i] = toupper(bufferRaw[i]);
		
		//print info
		if(strncmp(buffer, CMD_INFO, strlen(CMD_INFO)) == 0) {
			printInfo(h);
		}
		//print dir info
		else if(strncmp(buffer, CMD_DIR, strlen(CMD_DIR)) == 0) {
			doDir(h, curDirClus);	
		}
		//cd to a new directory
		else if(strncmp(buffer, CMD_CD, strlen(CMD_CD)) == 0) {
			uint32_t temp = doCD(h, curDirClus, buffer);
			if(temp == -1) {
				printf("Not a directory\n");
			}
			else {
				curDirClus = temp;
			}
		}
		//download a file
		else if(strncmp(buffer, CMD_GET, strlen(CMD_GET)) == 0) {
			doDownload(h, curDirClus, buffer);
		}
		//not valid command
		else { 
			printf("\nCommand not found\n");
		}
	}
	printf("\nExited...\n");
	
	cleanupHead(h);
}

//a helper function that prints the specified string on a new line
static void println(char* string) {
	printf("%s\n", string);
}

//prints the info about the disk image
void printInfo(fat32Head* h) {
	fat32BS* bs = h->bs;
	uint64_t sizeB = (uint64_t)bs->BPB_BytesPerSec * bs->BPB_TotSec32;
	uint16_t sizeMB = (sizeB / 1000000);
	float sizeGB = (float)sizeMB / 1000;

	println("---- Device Info ----");
	printf("OEM Name: %.*s\n", BS_OEMName_LENGTH, bs->BS_OEMName);
	printf("Label: %.*s\n", BS_VolLab_LENGTH, bs->BS_VolLab);
	printf("File System Type: %.*s\n", BS_FilSysType_LENGTH, bs->BS_FilSysType);
	printf("Media Type: 0x%X (fixed)\n", (unsigned char)bs->BPB_Media);
	printf("Size: %llu bytes (%u MB, %1.3f GB)\n", (unsigned long long)sizeB, (unsigned int)sizeMB, sizeGB);
	printf("Drive #: %u (hard disk)\n", (unsigned int)bs->BS_DrvNum);
	println("");

	println("---- Geometry ----");
	printf("Bytes per Sector: %u\n", (unsigned int)bs->BPB_BytesPerSec);
	printf("Sectors per Cluster: %u\n", (unsigned int)bs->BPB_SecPerClus);
	printf("Total Sectors: %lu\n", (unsigned long)bs->BPB_TotSec32);
	printf("Geom: Sectors per Track: %u\n", (unsigned int)bs->BPB_SecPerTrk);
	printf("Geom: Heads: %u\n", (unsigned int)bs->BPB_NumHeads);
	printf("Hidden Sectors: %lu\n", (unsigned long)bs->BPB_HiddSec);
	println("");

	println("---- FS Info ----");
	printf("Volume ID: %s\n", h->volumeID); //comes from cluster
	printf("Version: %u.%u\n", (unsigned int)bs->BPB_FSVerHigh, (unsigned int)bs->BPB_FSVerLow);
	printf("Reserved Sectors: %u\n", (unsigned int)bs->BPB_RsvdSecCnt);
	printf("# of FATs: %u\n", (unsigned int)bs->BPB_NumFATs);
	printf("Fat Size: %lu\n", (unsigned long)bs->BPB_FATSz32);
	printf("Mirrored FAT: %u (%s)\n", (unsigned int)bs->BPB_ExtFlags, (bs->BPB_ExtFlags ? "no" : "yes"));
	printf("Boot Sector Backup Sector #: %u\n", (unsigned int)bs->BPB_BkBootSec);
}

//prints the info about a directory
void doDir(fat32Head* h, uint32_t curDirClus) {	
		println("");
		println("DIRECTORY LISTING");
		printf("VOL_ID: %s\n", h->volumeID);
		println("");
	do {
		//read the cluster from memory
		uint8_t* cluster = loadCluster(h, curDirClus);
	
		//cast the cluster as a fat32Dir
		fat32Dir* dir = (fat32Dir*)(&cluster[0]);	
		uint32_t dirCount = getBytesPerCluster(h) / sizeof(fat32Dir); 
		
		for(int i=0; i<dirCount; i++) {
			//filter out long names
			if((dir->DIR_Attr & ATTR_LONG_NAME_MASK) != ATTR_LONG_NAME) {
				//make sure the entry is not empty
				if(dir->DIR_Name[0] != FREE_DIR) {
					//make sure the entry is not the last one
					if(dir->DIR_Name[0] == FREE_AND_LAST_DIR) break;
					//check if file
					if((dir->DIR_Attr & (ATTR_DIRECTORY | ATTR_VOLUME_ID)) == 0x00) {
						char* dirName = dir->DIR_Name;
						char filename[DIR_Name_LENGTH + 1];//add 1 for the "."
						char* saveptr = NULL;
						
						//this just makes the output pretty
						strcpy(filename, strtok_r(dirName, " ", &saveptr));
						strcat(filename, ".");
						strcat(filename, strtok_r(NULL, " ", &saveptr));
						printf("%-15s%10lu\n", filename, (unsigned long)dir->DIR_FileSize);
					}
					//check if directory
					else if((dir->DIR_Attr & (ATTR_DIRECTORY | ATTR_VOLUME_ID)) == ATTR_DIRECTORY) {
						char* dirName = dir->DIR_Name;
						char filename[DIR_Name_LENGTH + 2];//add 2 for the "<" and ">"
						char* saveptr = NULL;
						
						//again making it pretty
						strcpy(filename, "<");
						strcat(filename, strtok_r(dirName, " ", &saveptr));
						strcat(filename, ">");
						printf("%-15s%10lu\n", filename, (unsigned long)dir->DIR_FileSize);
					}
				}
			}
			dir++;
		}
		free(cluster);
	
		uint32_t secByte = getThisFatSecNum(h, curDirClus) * getBytesPerSector(h);
		uint32_t entOffset = getThisFatEntOffset(h, curDirClus);
		uint32_t dword;
		lseek(h->fd, secByte+entOffset, SEEK_SET);
		read(h->fd, &dword, sizeof(uint32_t));	
		curDirClus = dword & 0x0FFFFFFF;

	} while(curDirClus < EOC);
	
	uint64_t totalFreeBytes = (uint64_t)h->fsinfo->FSI_Free_Count * getBytesPerCluster(h);
	printf("---Bytes Free: %llu\n", (unsigned long long)totalFreeBytes);
	println("---DONE");
}

//returns the cluster of another directory specified in command
uint32_t doCD(fat32Head* h, uint32_t curDirClus, char* command) {
	char* saveptr = NULL;
	char cdDirectory[DIR_Name_LENGTH];
	strtok_r(command, " ", &saveptr);
	strcpy(cdDirectory, strtok_r(NULL, " ", &saveptr));

	uint8_t* cluster = loadCluster(h, curDirClus);
	fat32Dir* dir = (fat32Dir*)(&cluster[0]);	
	uint32_t dirCount = getBytesPerCluster(h) / sizeof(fat32Dir); 
	
	for(int i=0; i<dirCount; i++) {
		//filter out long names
		if((dir->DIR_Attr & ATTR_LONG_NAME_MASK) != ATTR_LONG_NAME) {
			//make sure the entry is not empty
			if(dir->DIR_Name[0] != FREE_DIR) {
				//make sure the entry is not the last one
				if(dir->DIR_Name[0] == FREE_AND_LAST_DIR) break;
				//check if directory
				if((dir->DIR_Attr & (ATTR_DIRECTORY | ATTR_VOLUME_ID)) == ATTR_DIRECTORY) {
					char* dirName = dir->DIR_Name;
					char filename[DIR_Name_LENGTH];
					saveptr = NULL;
					
					strcpy(filename, strtok_r(dirName, " ", &saveptr));
					//check if directory matches
					if(!strcmp(filename, cdDirectory)) {
						//return the cluster number
						uint32_t output = dir->DIR_FstClusHI * 0x100 + dir->DIR_FstClusLO;
						if(output == 0) {
							output = h->bs->BPB_RootClus;
						}
						free(cluster);
						return output;
					}
				}
			}
		}
		dir++;
	}
	free(cluster);
	return -1;
}

//downloads the file specified in command to the hard drive
void doDownload(fat32Head* h, uint32_t curDirClus, char* command) {
	char* saveptr = NULL;
	char downDirectory[DIR_Name_LENGTH];
	strtok_r(command, " ", &saveptr);
	strcpy(downDirectory, strtok_r(NULL, " ", &saveptr));

	uint8_t* cluster = loadCluster(h, curDirClus);
	fat32Dir* dir = (fat32Dir*)(&cluster[0]);	
	uint32_t dirCount = getBytesPerCluster(h) / sizeof(fat32Dir); 

	for(int i=0; i<dirCount; i++) {
		//filter out long names
		if((dir->DIR_Attr & ATTR_LONG_NAME_MASK) != ATTR_LONG_NAME) {
			//make sure the entry is not empty
			if(dir->DIR_Name[0] != FREE_DIR) {
				//make sure the entry is not the last one
				if(dir->DIR_Name[0] == FREE_AND_LAST_DIR) break;
				//check if file
				if((dir->DIR_Attr & (ATTR_DIRECTORY | ATTR_VOLUME_ID)) == 0x00) {
					char* dirName = dir->DIR_Name;
					char filename[DIR_Name_LENGTH + 1];//add 1 for the "."
					char* saveptr = NULL;
					
					//this just makes the output pretty
					strcpy(filename, strtok_r(dirName, " ", &saveptr));
					strcat(filename, ".");
					strcat(filename, strtok_r(NULL, " ", &saveptr));
					//check if directory matches
					if(!strcmp(filename, downDirectory)) {
						//download file
						uint32_t firstCluster = dir->DIR_FstClusHI * 0x100 + dir->DIR_FstClusLO;
						downloadFile(h, dir, firstCluster, filename);
						break;
					}
				}
			}
		}
		dir++;
	}
	free(cluster);
}
