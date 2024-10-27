#ifndef _FAT_LIB_H_
#define _FAT_LIB_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "dirNode.h"
#include "read_infor.h"
#include "error_codes.h"
#include "get_cluster.h"

/*******************************************************************************
* API
******************************************************************************/

/**
 * @brief Callback function type for processing directory entries.
 * 
 * @param entry Pointer to the directory entry.
 */
typedef void (*contentCallback)(const DirectoryEntry_t *entry);

/**
 * @brief Callback function type for displaying the header of a table.
 */
typedef void (*headerTableCallback)(void);

/**
 * @brief Callback function type for displaying help information.
 */
typedef void (*helpCallback)(void);

/**
 * @brief Initialize the FAT library with a given file.
 * 
 * @param file Pointer to the file.
 * @return Error code.
 */
error_code_t initFat(FILE *file);

/**
 * @brief List the directory contents.
 * 
 * @param showHidden Flag to show hidden files.
 * @param headerTableCallback Callback function to display the header of the table.
 * @param contentCallback Callback function to process each directory entry.
 * @return Error code.
 */
error_code_t listDirectory(uint8_t showHidden, headerTableCallback headerTableCallback, contentCallback contentCallback);

/**
 * @brief Display help information.
 * 
 * @param helpCallback Callback function to display help information.
 */
void help(helpCallback helpCallback);

/**
 * @brief Change the current directory.
 * 
 * @param dir Pointer to the directory name.
 * @return Error code.
 */
error_code_t changeDirectory(char *dir);

/**
 * @brief Show the content of a file.
 * 
 * @param filename Pointer to the file name.
 * @return Error code.
 */
error_code_t showFileContent(char *filename);

/**
 * @brief Go to the previous directory.
 * 
 * @return Error code.
 */
error_code_t goPrevDirectory(void);

/**
 * @brief Copy the current directory to the destination.
 * 
 * @param dest Pointer to the destination directory node.
 * @return Error code.
 */
error_code_t copyDirectory(dirNode_t **dest);

error_code_t createFolder(char *dir);
error_code_t deleteFolder(char *dir);

/**
 * @brief Restore the directory from the source.
 * 
 * @param source Pointer to the source directory node.
 */
void restoreDirectory(dirNode_t **source);


#endif /* _FAT_LIB_H_ */