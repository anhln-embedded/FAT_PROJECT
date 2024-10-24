#ifndef _GET_CLUSTER_H_
#define _GET_CLUSTER_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "read_infor.h"

#define addressFAT 0x200
typedef uint16_t START_CLUSTER;

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



START_CLUSTER getNextCluster(START_CLUSTER startCluster, FILE *file);

void readFolder(uint16_t startCluster, FILE *file);
void readFile(uint16_t startCluster, FILE * file);
uint16_t findName (uint16_t startCluster, char * filename);
uint16_t findNameInCluster(uint16_t startCluster, char *filename, FILE *file);
//void FindFolderInfor(uint16_t startCluster, char fileName, uint16_t *nextStartCluster);

#endif