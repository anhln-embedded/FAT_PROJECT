#ifndef _GET_CLUSTER_H_
#define _GET_CLUSTER_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "read_infor.h"
#include "error_codes.h"

#define addressFAT 0x200

#define ATTR_DIRECTORY 0x10 // folder
#define ATTR_FILE 0x20      // file

#define CLUSTER_SIZE 512
#define DIR_ENTRY_SIZE 32
#define ENTRIES_PER_CLUSTER (CLUSTER_SIZE / DIR_ENTRY_SIZE)

typedef enum ERROR_TABLE_FAT
{
    FREE_CLUSTER = 1,
    RESERVED_CLUSTER,
    BAD_CLUSTER,
    EOF_CLUSTER
}ERROR_FAT;



uint16_t getNextCluster(uint16_t startCluster, FILE *file);
uint16_t getAddressCluster(const BootSector_t *bs, uint16_t startcluster);

error_code_t getEntry(FILE *fp, const BootSector_t *bs, DirectoryEntry_t *entryOut);
error_code_t findName(FILE *fp, const BootSector_t *bs, char *filename, DirectoryEntry_t *entryOput);
error_code_t readFile(FILE * file, uint16_t startCluster);

//void FindFolderInfor(uint16_t startCluster, char fileName, uint16_t *nextStartCluster);

#endif