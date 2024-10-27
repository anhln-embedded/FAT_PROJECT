#ifndef _GET_CLUSTER_H_
#define _GET_CLUSTER_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "read_infor.h"
#include "error_codes.h"

#define addressFAT 0x200

#define FILE_ATTRIBUTE_TYPE     0U
#define FOLDER_ATTRIBUTE_TYPE   1U

#define ATTR_DIRECTORY 0x10 // folder
#define ATTR_FILE 0x20      // file

// #define CLUSTER_SIZE 512
//  #define DIR_ENTRY_SIZE 32
//  #define ENTRIES_PER_CLUSTER (CLUSTER_SIZE / DIR_ENTRY_SIZE)

typedef enum ERROR_TABLE_FAT
{
    FREE_CLUSTER = 0x000,
    RESERVED_CLUSTER = 0x001,
    BAD_CLUSTER = 0x007,
    EOF_CLUSTER = 0xfff
} ERROR_FAT;

uint16_t getNextCluster(uint16_t startCluster, FILE *file);
uint32_t getAddressCluster(const BootSector_t *bs, uint32_t startCluster);

error_code_t getEntry(FILE *fp, const BootSector_t *bs, DirectoryEntry_t *entryOut);
error_code_t findName(FILE *fp, 
                      const BootSector_t *bs, 
                      char *filename, 
                      uint16_t startCluster, 
                      DirectoryEntry_t *entryOut,
                      uint8_t attribute);
error_code_t readFile(FILE *fp, const BootSector_t *bs, uint16_t startCluster, DirectoryEntry_t *entry);

// void FindFolderInfor(uint16_t startCluster, char fileName, uint16_t *nextStartCluster);

#endif