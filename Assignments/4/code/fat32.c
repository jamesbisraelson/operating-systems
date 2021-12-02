#define _FILE_OFFSET_BITS 64
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "fat32.h"

fat32Head* createHead(int fd) {
	//malloc the head struct
	fat32Head* h = malloc(sizeof(fat32Head));
	
	//malloc and read the boot sector struct
	h->bs = malloc(sizeof(fat32BS));
	read(fd, h->bs, sizeof(fat32BS));
	
	h->fsinfo = malloc(sizeof(fsInfo));
	read(fd, h->fsinfo, sizeof(fsInfo));
	
	h->fd = fd;
	h->volumeID = getVolumeID(h);
	return h;
}

void cleanupHead(fat32Head* h) {
	free(h->bs);
	free(h->fsinfo);
	free(h->volumeID);
	free(h);
}

uint32_t getFirstDataSector(fat32Head* h) {
	uint32_t firstDataSector = h->bs->BPB_RsvdSecCnt + (h->bs->BPB_NumFATs * h->bs->BPB_FATSz32);
	return firstDataSector;
}

uint32_t getFirstSectorOfCluster(fat32Head* h, uint32_t cluster) {
	uint32_t firstSectorOfCluster = ((cluster-2) * h->bs->BPB_SecPerClus) + getFirstDataSector(h);
	return firstSectorOfCluster;
}

uint32_t getDataSectors(fat32Head* h) {
	uint32_t dataSectors = h->bs->BPB_TotSec32 - (h->bs->BPB_RsvdSecCnt + (h->bs->BPB_NumFATs * h->bs->BPB_FATSz32));
	return dataSectors;
}

uint32_t getClusterCount(fat32Head* h) {
	uint32_t dataSectors = getDataSectors(h);
	uint32_t clusterCount = dataSectors / h->bs->BPB_SecPerClus;	
	return clusterCount;
}

uint32_t getThisFatSecNum(fat32Head* h, uint32_t cluster) {
	uint64_t fatOffset = (uint64_t)cluster * 4;
	return h->bs->BPB_RsvdSecCnt + (uint32_t)(fatOffset / h->bs->BPB_BytesPerSec);
}

uint32_t getThisFatEntOffset(fat32Head* h, uint32_t cluster) {
	uint64_t fatOffset = (uint64_t)cluster * 4;
	return (uint32_t)(fatOffset % h->bs->BPB_BytesPerSec);
}

//gets the amount of byes in a cluster
uint32_t getBytesPerCluster(fat32Head* h) {
	return h->bs->BPB_SecPerClus * h->bs->BPB_BytesPerSec;
}

uint16_t getBytesPerSector(fat32Head* h) {
	return h->bs->BPB_BytesPerSec;
}

uint8_t* loadCluster(fat32Head* h, uint32_t curDirClus) {
	//get the first byte of the first sector of the directory cluster and seek to it
	uint32_t sector = getFirstSectorOfCluster(h, curDirClus);
	off_t sectorByte = (off_t)sector * h->bs->BPB_BytesPerSec;
	lseek(h->fd, sectorByte, SEEK_SET);

	uint32_t bytesPerClus = getBytesPerCluster(h);

	//calculate the cluster byte size, create the cluster, and read it in
	uint8_t* cluster = malloc(sizeof(uint8_t)*bytesPerClus);
	read(h->fd, cluster, bytesPerClus);	
	return cluster;
}

char* getVolumeID(fat32Head* h) {
	uint8_t* cluster = loadCluster(h, h->bs->BPB_RootClus);
	fat32Dir* dir = (fat32Dir*)(&cluster[0]);
	uint32_t dirCount = getBytesPerSector(h) / sizeof(fat32Dir);

	for(int i=0; i<dirCount; i++) {
		if((dir->DIR_Attr & ATTR_LONG_NAME_MASK) != ATTR_LONG_NAME) {
			if((dir->DIR_Attr & (ATTR_DIRECTORY | ATTR_VOLUME_ID)) == ATTR_VOLUME_ID) {
				char* copy = malloc(sizeof(char)*100);
				strcpy(copy, dir->DIR_Name);
				free(cluster);
				return copy;
			}
		}
	}
	free(cluster);
	return NULL;
}

void downloadFile(fat32Head* h, fat32Dir* dir, uint32_t firstCluster, char* filename) {
	uint32_t currentClus = firstCluster;
	int fd_out = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	uint32_t fileSize = dir->DIR_FileSize;
	uint32_t clusterSize = getBytesPerCluster(h);
	uint32_t writeBuffer = clusterSize;

	if(fd_out == -1) {
		perror("open error");
		exit(1);
	}
	do {
		uint8_t* cluster = loadCluster(h, currentClus);

		if(fileSize < clusterSize) {
			writeBuffer = fileSize;
		}
		
		if(write(fd_out, (void*)cluster, writeBuffer) == -1) {
			perror("write error");
			exit(1);
		}
		fileSize -= clusterSize;
		free(cluster);
		
		uint32_t secByte = getThisFatSecNum(h, currentClus) * getBytesPerSector(h);
		uint32_t entOffset = getThisFatEntOffset(h, currentClus);
		uint32_t dword;
		lseek(h->fd, secByte+entOffset, SEEK_SET);
		read(h->fd, &dword, sizeof(uint32_t));	
		currentClus = dword & 0x0FFFFFFF;

	} while(currentClus < EOC);
	if(close(fd_out) == -1) {
		perror("close error");
		exit(1);
	}
	printf("Done.\n");
}
