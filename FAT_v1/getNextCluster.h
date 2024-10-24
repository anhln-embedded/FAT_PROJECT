#ifndef _GET_NEXT_CLUSTER_
#define _GET_NEXT_CLUSTER_

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define addressFAT 0x200
typedef uint16_t START_CLUSTER;
typedef enum ERROR_TABLE_FAT
{
    FREE_CLUSTER = 1,
    RESERVED_CLUSTER,
    BAD_CLUSTER,
    EOF_CLUSTER
}ERROR_FAT;

START_CLUSTER getNextCluster(START_CLUSTER startCluster, FILE *file);

#endif