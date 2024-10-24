#ifndef _FAT_LIB_H_
#define _FAT_LIB_H_

#include <stdio.h>
#include <stdint.h>
#include "dirNode.h"
#include "read_infor.h"
#include "DirectoryEntry.h"
#include "readClusterData.h"
#include <string.h>
#include "getNextCluster.h"

typedef enum
{
    ERROR = 0,
    ERROR_OK,

} error_t;

error_t initFat(FILE *file);
error_t listDirectory(uint8_t showHidden);
error_t help(void);
error_t changeDirectory(char *dir);
error_t showFileContent(char *filename);

#endif