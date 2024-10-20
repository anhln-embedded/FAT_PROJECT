/*
 ******************************************************************************
 * @file		read_cluster.h                                                *
 * @author	    Luu Ngoc Anh                                                  *
 * @date		19/10/2024                                                    *
 ******************************************************************************
 */

#ifndef __READ_CLUSTER_H__
#define __READ_CLUSTER_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <stdlib.h>
#include "read_infor.h"
#include "DirectoryEntry.h"

typedef struct {
    uint32_t cluster;
    uint32_t clusterSize;
    uint32_t clusterAddress;
    char *buffer;
} ClusterBuffer_t;

uint32_t getNextCluster(FILE *fp, const BootSector_t *bs, uint32_t cluster);
void read_cluster(FILE *fp, const BootSector_t *bs, uint32_t cluster, char *buffer);
char * getFileContent(FILE *fp, const BootSector_t *bs, const DirectoryEntry *entry);


#ifdef __cplusplus
}
#endif

#endif

/********************************* END OF FILE ********************************/
/******************************************************************************/