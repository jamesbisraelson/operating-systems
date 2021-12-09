/* 
 * fat32.h:
 *
 * The .h file for fat32.c. Also holds all of the structs for loading in memory from the diskimage.
 */
#ifndef FAT32_H
#define FAT32_H

#include <inttypes.h>

#define ATTR_READ_ONLY 0x01
#define ATTR_HIDDEN 0x02
#define ATTR_SYSTEM 0x04
#define ATTR_VOLUME_ID 0x08
#define ATTR_DIRECTORY 0x10
#define ATTR_ARCHIVE 0x20
#define ATTR_LONG_NAME (ATTR_READ_ONLY | ATTR_HIDDEN | ATTR_SYSTEM | ATTR_VOLUME_ID)
#define ATTR_LONG_NAME_MASK (ATTR_READ_ONLY | ATTR_HIDDEN | ATTR_SYSTEM | ATTR_VOLUME_ID | ATTR_DIRECTORY | ATTR_ARCHIVE)
#define FREE_DIR 0xE5
#define FREE_AND_LAST_DIR 0x00
#define EOC 0x0FFFFFF8

//boot sector constants
#define BS_OEMName_LENGTH 8
#define BS_VolLab_LENGTH 11
#define BS_FilSysType_LENGTH 8 
#define BS_jmpBoot_LENGTH 3
#define BPB_Reserved_LENGTH 12
#define BS_CodeReserved_LENGTH 420

//the struct for the boot sector
#pragma pack(push)
#pragma pack(1)
struct fat32BS_struct {
	char BS_jmpBoot[BS_jmpBoot_LENGTH];
	char BS_OEMName[BS_OEMName_LENGTH];
	uint16_t BPB_BytesPerSec;
	uint8_t BPB_SecPerClus;
	uint16_t BPB_RsvdSecCnt;
	uint8_t BPB_NumFATs;
	uint16_t BPB_RootEntCnt;
	uint16_t BPB_TotSec16;
	uint8_t BPB_Media;
	uint16_t BPB_FATSz16;
	uint16_t BPB_SecPerTrk;
	uint16_t BPB_NumHeads;
	uint32_t BPB_HiddSec;
	uint32_t BPB_TotSec32;
	uint32_t BPB_FATSz32;
	uint16_t BPB_ExtFlags;
	uint8_t BPB_FSVerLow;
	uint8_t BPB_FSVerHigh;
	uint32_t BPB_RootClus;
	uint16_t BPB_FSInfo;
	uint16_t BPB_BkBootSec;
	char BPB_Reserved[BPB_Reserved_LENGTH];
	uint8_t BS_DrvNum;
	uint8_t BS_Reserved1;
	uint8_t BS_BootSig;
	uint32_t BS_VolID;
	char BS_VolLab[BS_VolLab_LENGTH];
	char BS_FilSysType[BS_FilSysType_LENGTH];
	char BS_CodeReserved[BS_CodeReserved_LENGTH];
	uint8_t BS_SigA;
	uint8_t BS_SigB;
};
#pragma pack(pop)
typedef struct fat32BS_struct fat32BS;


//fsinfo constants
#define FSI_Reserved1_LENGTH 480
#define FSI_Reserved2_LENGTH 12

//the struct for the FSI
#pragma pack(push)
#pragma pack(1)
struct fsInfo_struct {
	uint32_t FSI_LeadSig;
	char FSI_Reserved1[FSI_Reserved1_LENGTH];
	uint32_t FSI_StrucSig;
	uint32_t FSI_Free_Count;
	uint32_t FSI_Nxt_Free;
	char FSI_Reserved2[FSI_Reserved2_LENGTH];
	uint32_t FSI_TrailSig;
};
#pragma pack(pop)
typedef struct fsInfo_struct fsInfo;


//directory constants
#define DIR_Reserved_LENGTH 4
#define DIR_Name_LENGTH 11

//the struct for the directory
#pragma pack(push)
#pragma pack(1)
struct fat32Dir_struct {
	char DIR_Name[DIR_Name_LENGTH];
	char DIR_Attr;
	char DIR_NTRes;
	char DIR_CrtTimeTenth;
	uint16_t DIR_CrtTime;
	uint16_t DIR_CrtDate;
	uint16_t DIR_LstAccDate;
	uint16_t DIR_FstClusHI;
	uint16_t DIR_WrtTime;
	uint16_t DIR_WrtDate;
	uint16_t DIR_FstClusLO;
	uint32_t DIR_FileSize;
};
#pragma pack(pop)
typedef struct fat32Dir_struct fat32Dir;

//the struct for the boot sector, fsinfo, volumeID, and the file descriptor for the disk image
#pragma pack(push)
#pragma pack(1)
struct fat32Head_struct {
	fat32BS* bs;
	fsInfo* fsinfo;
	int fd;
	char* volumeID;
};
#pragma pack(pop)
typedef struct fat32Head_struct fat32Head;

//creates a fat32Head
fat32Head* createHead(int fd);

//frees the memory for a fat32Head
void cleanupHead(fat32Head* h);

//gets the first data sector
uint32_t getFirstDataSector(fat32Head* h);

//gets the number of data sectors
uint32_t getDataSectors(fat32Head* h);

//gets the number of clusters in a sector
uint32_t getClusterCount(fat32Head* h);

//gets the first sector of a cluster
uint32_t getFirstSectorOfCluster(fat32Head* h, uint32_t cluster);

//get the sector for a particular cluster in the fat
uint32_t getThisFatSecNum(fat32Head* h, uint32_t cluster);

//get the offset byte of the first part of data in a cluster
uint32_t getThisFatEntOffset(fat32Head* h, uint32_t cluster);

//get the number of bytes in a cluster
uint32_t getBytesPerCluster(fat32Head* h);

//get the number of bytes in a sector
uint16_t getBytesPerSector(fat32Head* h);

//load an entire cluster into memory and return a pointer to the front of it
uint8_t* loadCluster(fat32Head* h, uint32_t curDirClus);

//get the volumeID of the root directory
char* getVolumeID(fat32Head* h);

//download a file from the disk image to the hard drive
void downloadFile(fat32Head* h, fat32Dir* dir, uint32_t firstCluster, char* filename);
#endif
