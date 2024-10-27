#ifndef _ADD_FILES_
#define _ADD_FILES_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "read_infor.h"
#include "error_codes.h"
#include "get_cluster.h"

uint32_t getSizeFile(const char *filePath);
uint16_t findTempEntryFat(FILE *file, const BootSector_t *bs);
uint8_t writeContent(FILE *file, const BootSector_t *bs, uint32_t addressCluster, const char *filePath, uint32_t fileSize);
char *getFileName(const char *filePath);
char *getFileExtension(const char *filePath);

error_code_t addContent(FILE *file, const BootSector_t *bs, const char *filePath, uint16_t startCluster, DirectoryEntry_t *entryOut);



#endif