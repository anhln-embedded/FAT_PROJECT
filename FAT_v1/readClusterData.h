#ifndef _READ_CLUSTER_DATA_
#define _READ_CLUSTER_DATA_

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "DirectoryEntry.h"

#define ATTR_DIRECTORY 0x10 // folder
#define ATTR_FILE 0x20      // file

#define CLUSTER_SIZE 512
#define DIR_ENTRY_SIZE 32
#define ENTRIES_PER_CLUSTER (CLUSTER_SIZE / DIR_ENTRY_SIZE)

// typedef struct
// {
//     /* data */
// };

void readFolder(uint16_t startCluster, FILE *file);
void readFile(uint16_t startCluster, FILE * file);
//void FindFolderInfor(uint16_t startCluster, char fileName, uint16_t *nextStartCluster);

#endif
