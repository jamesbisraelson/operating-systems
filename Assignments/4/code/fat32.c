#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "fat32.h"

fat32Head* createHead(int fd) {
	fat32Head* h = malloc(sizeof(fat32Head));
	h->bs = malloc(sizeof(fat32BS));
	read(fd, h->bs, sizeof(fat32BS));
	return h;
}

void cleanupHead(fat32Head* h) {
	free(h->bs);
	free(h);
}

static void println(char* string) {
	printf("%s\n", string);
}

void printInfo(fat32Head* h) {
	fat32BS* bs = h->bs;
	long long sizeB = (long long)((long)bs->BPB_BytesPerSec * (long)bs->BPB_TotSec32);
	int sizeMB = (int)(sizeB / 1000000);
	float sizeGB = (float)sizeMB / 1000;

	println("---- Device Info ----");
	printf("OEM Name: %.*s\n", BS_OEMName_LENGTH, bs->BS_OEMName);
	printf("Label: %.*s\n", BS_VolLab_LENGTH, bs->BS_VolLab);
	printf("File System Type: %.*s\n", BS_FilSysType_LENGTH, bs->BS_FilSysType);
	printf("Media Type: 0x%X (fixed)\n", (int)bs->BPB_Media);
	printf("Size: %lld bytes (%d MB, %1.3f GB)\n", sizeB, sizeMB, sizeGB);
	printf("Drive #: %u (hard disk)\n", (int)bs->BS_DrvNum);
	println("");

	println("---- Geometry ----");
	printf("Bytes per Sector: %d\n", (int)bs->BPB_BytesPerSec);
	printf("Sectors per Cluster: %d\n", (int)bs->BPB_SecPerClus);
	printf("Total Sectors: %li\n", (long)bs->BPB_TotSec32);
	printf("Geom: Sectors per Track: %d\n", (int)bs->BPB_SecPerTrk);
	printf("Geom: Heads: %d\n", (int)bs->BPB_NumHeads);
	printf("Hidden Sectors: %li\n", (long)bs->BPB_HiddSec);
	println("");

	println("---- FS Info ----");
	printf("Volume ID: TODO\n"); //(long)bs->BS_VolID);
	printf("Version: %d.%d\n", (int)bs->BPB_FSVerHigh, (int)bs->BPB_FSVerLow);
	printf("Reserved Sectors: %d\n", (int)bs->BPB_RsvdSecCnt);
	printf("# of FATs: %d\n", (int)bs->BPB_NumFATs);
	printf("Fat Size: %li\n", (long)bs->BPB_FATSz32);
	printf("Mirrored FAT: %d (%s)\n", (int)bs->BPB_ExtFlags, ((int)bs->BPB_ExtFlags ? "no" : "yes"));
	printf("Boot Sector Backup Sector #: %d\n", (int)bs->BPB_BkBootSec);
}
