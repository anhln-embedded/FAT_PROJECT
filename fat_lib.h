#ifndef _FAT_LIB_H_
#define _FAT_LIB_H_

#include <stdio.h>
#include <stdint.h>
#include "dirNode.h"
#include "read_infor.h"
#include <string.h>
#include "error_codes.h"
#include "get_cluster.h"

typedef void (*contentCallback)(const DirectoryEntry_t *entry);
typedef void (*headerTableCallback)(void);
typedef void (*helpCallback)(void);

error_code_t initFat(FILE *file);
error_code_t listDirectory(uint8_t showHidden, headerTableCallback headerTableCallback, contentCallback contentCallback);
void help(helpCallback helpCallback);
error_code_t changeDirectory(char *dir);
error_code_t showFileContent(char *filename);
error_code_t goPrevDirectory(void);

#endif