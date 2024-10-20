/*
 ******************************************************************************
 * @file		DirectoryEntry.h                                               *
 * @author	    Luu Ngoc Anh                                                     *
 * @date		19/10/2024                                                     *
 ******************************************************************************
 */

#ifndef __DIRECTORY_H__
#define __DIRECTORY_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include "read_infor.h"

typedef struct
{
    uint16_t day : 5;
    uint16_t month : 4;
    uint16_t year : 7;
} Date_t; 
/*Remember that the year is based at 1980*/

typedef struct
{
    uint16_t  hour: 5;
    uint16_t  min : 6;
    uint16_t  sec : 5;
} Time_t;

typedef struct {
    char name[8];
    char ext[3];            
    uint8_t attr;                 
    uint8_t reserved[10];         
    Time_t time;              
    Date_t date;                
    uint16_t startCluster;        
    uint32_t fileSize;            
} DirectoryEntry;

struct X{
    DirectoryEntry a;
    DirectoryEntry *prev;
};

void read_root_directory(FILE *fp, const BootSector_t *bs);
uint32_t getRootDirStart(const BootSector_t *bs);

#ifdef __cplusplus
}
#endif

#endif

/********************************* END OF FILE ********************************/
/******************************************************************************/