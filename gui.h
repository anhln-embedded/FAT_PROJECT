#ifndef GUI_H
#define GUI_H

#include <stdio.h>
#include <stdint.h>
#include "read_infor.h"

/*******************************************************************************
* API
******************************************************************************/
/**
 * @brief Print the header for the directory listing.
 */
void printHeader(void);

/**
 * @brief Print a directory entry.
 * 
 * @param entry Pointer to the directory entry to be printed.
 */
void printDirectoryEntry(const DirectoryEntry_t *entry);

/**
 * @brief Print the help information.
 */
void printHelp(void);

#endif /* GUI_H */