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
 extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>
#pragma pack(1) 


typedef struct {
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
    uint32_t totalSectors32;      
    uint32_t sectorsPerFAT32;    
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



void read_boot_sector(FILE *fp, BootSector_t *bs);
void display_boot_sector_info(const BootSector_t *bs);
uint16_t getRootDirStart(const BootSector_t *bs);
void read_root_directory(FILE *fp, const BootSector_t *bs);


#ifdef __cplusplus
}
#endif

#endif

/********************************* END OF FILE ********************************/
/******************************************************************************/