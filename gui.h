#ifndef GUI_H
#define GUI_H

#include <stdio.h>
#include <stdint.h>
#include "read_infor.h"


void printHeader(void);
void printDirectoryEntry(const DirectoryEntry_t *entry);\
void printHelp(void);

#endif // GUI_H