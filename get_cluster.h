#ifndef _GET_CLUSTER_H_
#define _GET_CLUSTER_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "read_infor.h"
#include "error_codes.h"

/*******************************************************************************
* Definitions
******************************************************************************/
#define ADDRESS_FAT 0x200U

#define FILE_ATTRIBUTE_TYPE 0U
#define FOLDER_ATTRIBUTE_TYPE 1U

#define ATTR_DIRECTORY 0x10U /* Folder */
#define ATTR_FILE 0x20U      /* File */

typedef enum {
    FREE_CLUSTER = 0x000,
    RESERVED_CLUSTER = 0x001,
    BAD_CLUSTER = 0x007,
    EOF_CLUSTER = 0xFFF
} ERROR_FAT;

/*******************************************************************************
* API
******************************************************************************/
/**
 * @brief Get the next cluster in the FAT table.
 * 
 * @param startCluster The starting cluster.
 * @return The next cluster.
 */
uint16_t getNextCluster(uint16_t startCluster);

/**
 * @brief Get the address of a cluster.
 * 
 * @param bs Pointer to the boot sector structure.
 * @param startCluster The starting cluster.
 * @return The address of the cluster.
 */
uint32_t getAddressCluster(const BootSector_t *bs, uint32_t startCluster);

/**
 * @brief Get an entry from the directory.
 * 
 * @param bs Pointer to the boot sector structure.
 * @param entryOut Pointer to the directory entry structure to be filled.
 * @return Error code.
 */
error_code_t getEntry(const BootSector_t *bs, DirectoryEntry_t *entryOut);

/**
 * @brief Find a name in the directory.
 * 
 * @param bs Pointer to the boot sector structure.
 * @param filename Pointer to the filename to be found.
 * @param startCluster The starting cluster.
 * @param entryOut Pointer to the directory entry structure to be filled.
 * @param attribute Attribute type to be checked.
 * @return Error code.
 */
error_code_t findName(const BootSector_t *bs, char *filename, uint16_t startCluster, DirectoryEntry_t *entryOut, uint8_t attribute);

/**
 * @brief Read a file from the directory.
 * 
 * @param bs Pointer to the boot sector structure.
 * @param startCluster The starting cluster.
 * @param entry Pointer to the directory entry structure.
 * @return Error code.
 */
error_code_t readFile(const BootSector_t *bs, uint16_t startCluster, DirectoryEntry_t *entry);

#endif /* _GET_CLUSTER_H_ */