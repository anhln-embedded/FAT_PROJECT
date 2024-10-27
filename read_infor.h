/*
 ******************************************************************************
 * @file        read_infor.h
 * @author      Luu Ngoc Anh
 * @date        00/00/0000
 ******************************************************************************
 */

#ifndef __READ_INFOR_H__
#define __READ_INFOR_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "error_codes.h"
#include "hal.h"

/*******************************************************************************
* Definitions
******************************************************************************/
#define FILE_ATTRIBUTE_TYPE 0U
#define FOLDER_ATTRIBUTE_TYPE 1U

#define ATTR_READ_ONLY 0x01U /* Read only */
#define ATTR_HIDDEN 0x02U    /* Hidden */
#define ATTR_SYSTEM 0x04U    /* System */
#define ATTR_VOLUME_ID 0x08U /* Volume ID */
#define ATTR_DIRECTORY 0x10U /* Directory */
#define ATTR_ARCHIVE 0x20U   /* Archive */
#define ATTR_LONG_NAME 0x0FU /* Long file name */

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
} BootSector_t;

typedef struct {
    uint16_t day : 5;
    uint16_t month : 4;
    uint16_t year : 7;
} Date_t;
/* Remember that the year is based at 1980 */

typedef struct {
    uint16_t hour : 5;
    uint16_t min : 6;
    uint16_t sec : 5;
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
} DirectoryEntry_t;
#pragma pack()

/*******************************************************************************
* API
******************************************************************************/
/**
 * @brief Read the boot sector.
 * 
 * @param bs Pointer to the boot sector structure.
 * @return Error code.
 */
error_code_t read_boot_sector(BootSector_t *bs);

/**
 * @brief Get the start of the root directory.
 * 
 * @param bs Pointer to the boot sector structure.
 * @return The start cluster of the root directory.
 */
uint16_t getRootDirStart(const BootSector_t *bs);

/**
 * @brief Get an entry in the root directory.
 * 
 * @param bs Pointer to the boot sector structure.
 * @param entryOut Pointer to the directory entry structure to be filled.
 * @return Error code.
 */
error_code_t getEntryInRoot(const BootSector_t *bs, DirectoryEntry_t *entryOut);

/**
 * @brief Find a name in the root directory.
 * 
 * @param bs Pointer to the boot sector structure.
 * @param filename Pointer to the filename to be found.
 * @param entryOut Pointer to the directory entry structure to be filled.
 * @param attribute Attribute type to be checked.
 * @return Error code.
 */
error_code_t findNameInRoot(const BootSector_t *bs, char *filename, DirectoryEntry_t *entryOut, uint8_t attribute);

/**
 * @brief Compare the filename with the directory entry name.
 * 
 * @param entryOut Pointer to the directory entry structure.
 * @param filename Pointer to the filename to be compared.
 * @return Comparison result.
 */
int8_t compareFileName(DirectoryEntry_t *entryOut, const char *filename);


#ifdef __cplusplus
}
#endif

#endif /* __READ_INFOR_H__ */

/********************************* END OF FILE ********************************/
/******************************************************************************/