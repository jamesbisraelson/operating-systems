#define _FILE_OFFSET_BITS 64
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
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
	return h;
}

void cleanupHead(fat32Head* h) {
	free(h->bs);
	free(h->fsinfo);
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

uint8_t* loadCluster(fat32Head* h, uint32_t curDirClus) {
	//get the first byte of the first sector of the directory cluster and seek to it
	uint32_t sector = getFirstSectorOfCluster(h, curDirClus);
	off_t sectorByte = (off_t)sector * h->bs->BPB_BytesPerSec;
	lseek(h->fd, sectorByte, SEEK_SET);

	//calculate the cluster byte size, create the cluster, and read it in
	uint32_t clusterByteSize = h->bs->BPB_SecPerClus * h->bs->BPB_BytesPerSec;
	uint8_t* cluster = malloc(sizeof(uint8_t)*clusterByteSize);
	read(h->fd, cluster, clusterByteSize);	
	return cluster;
}
