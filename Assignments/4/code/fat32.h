#ifndef FAT32_H
#define FAT32_H

#include <inttypes.h>

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


//boot sector constants
#define FSI_Reserved1_LENGTH 480
#define FSI_Reserved2_LENGTH 12
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


//the struct for the boot sector and fsinfo
#pragma pack(push)
#pragma pack(1)
struct fat32Head_struct {
	fat32BS* bs;
	fsInfo* fsinfo;
	int fd;
};
#pragma pack(pop)
typedef struct fat32Head_struct fat32Head;


//directory constants
#define DIR_Reserved_LENGTH 4
#define DIR_Name_LENGTH 11
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


fat32Head* createHead(int fd);
void cleanupHead(fat32Head* h);
uint32_t getDataSectors(fat32Head* h);
uint32_t getClusterCount(fat32Head* h);
uint32_t getFirstSectorOfCluster(fat32Head* h, uint32_t cluster);
uint8_t* loadCluster(fat32Head* h, uint32_t curDirClus);
#endif
