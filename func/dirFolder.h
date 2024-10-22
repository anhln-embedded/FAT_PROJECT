#ifndef     _DIR_FOLDER_
#define     _DIR_FOLDER_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "read_FAT.h"
#include "readClusterData.h"

void dirFolder(uint16_t addressEntryFAT, FILE * file);

#endif // CHECK_FAT
