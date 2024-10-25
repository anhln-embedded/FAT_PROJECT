/*
 ******************************************************************************
 * @file		read_infor.h                                                   *
 * @author	Luu Ngoc Anh                                                       *
 * @date		00/00/0000                                                     *
 ******************************************************************************
 */

#ifndef __READ_INFOR_H__
#define __READ_INFOR_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "error_codes.h"

#define ATTR_READ_ONLY  0x01 /*Read only*/
#define ATTR_HIDDEN     0x02 /*Hidden*/
#define ATTR_SYSTEM     0x04 /*System*/
#define ATTR_VOLUME_ID  0x08 /*Volume ID*/
#define ATTR_DIRECTORY  0x10 /*Directory*/
#define ATTR_ARCHIVE    0x20 /*Archive*/

/**/
#pragma pack(1) 

    typedef struct
    {
        uint8_t jump[3];
        char oem[8];
        uint16_t bytesPerSector;
        uint8_t sectorsPerCluster;
        uint16_t reservedSectors;
        uint8_t numberOfFATs;
        uint16_t rootEntryCount;
        uint16_t totalSectors16;
        uint8_t mediaDescriptor;
        uint16_t sectorsPerFAT16;
        uint16_t sectorsPerTrack;
        uint16_t numberOfHeads;
        uint32_t hiddenSectors;
    } BootSector_t;

    typedef struct
    {
        uint16_t day : 5;
        uint16_t month : 4;
        uint16_t year : 7;
    } Date_t;
    /*Remember that the year is based at 1980*/

    typedef struct
    {
        uint16_t hour : 5;
        uint16_t min : 6;
        uint16_t sec : 5;
    } Time_t;

    typedef struct
    {
        char name[8];
        char ext[3];
        uint8_t attr;
        uint8_t reserved[10];
        Time_t time;
        Date_t date;
        uint16_t startCluster;
        uint32_t fileSize;
    } DirectoryEntry_t;

#pragma pack()


error_code_t read_boot_sector(FILE *fp, BootSector_t *bs);
uint16_t getRootDirStart(const BootSector_t *bs);
error_code_t getEntryInRoot(FILE *fp, const BootSector_t *bs, DirectoryEntry_t *entryOut);
error_code_t findNameInRoot(FILE *fp, const BootSector_t *bs, char *filename, DirectoryEntry_t *entryOput);
int8_t compareFileName(DirectoryEntry_t *entryOput, const char *filename);

#ifdef __cplusplus
}
#endif

#endif

/********************************* END OF FILE ********************************/
/******************************************************************************/