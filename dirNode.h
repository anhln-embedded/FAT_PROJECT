#ifndef DIR_NODE_H
#define DIR_NODE_H

#include <stdio.h>
#include <stdint.h>

/*******************************************************************************
* Definitions
******************************************************************************/
#define DIR_NODE_STATUS_OK                      0U
#define DIR_NODE_STATUS_INSUFFICIENT_MEMORY     1U
#define DIR_NODE_STATUS_SECOND_INIT             2U
#define DIR_NODE_STATUS_EMPTY_DIRECTORY         3U
#define DIR_NODE_STATUS_ROOT_DIR_ONLY           4U
#define DIR_NODE_STATUS_NOT_NULL                5U

typedef struct dir_node dirNode_t;
typedef uint8_t dirNodeStatus_t;

struct dir_node {
    dirNode_t *prev;
    uint16_t clusterEntry;
    /* dirNode_t *next; */
};

/*******************************************************************************
* API
******************************************************************************/
/**
 * @brief Initialize double linked list.
 * 
 * @param head Pointer to the head of the list.
 * @param clusterEntry Cluster entry to initialize.
 * @return Status of the initialization.
 */
dirNodeStatus_t init(dirNode_t **head, uint16_t clusterEntry);

/**
 * @brief Add next directory entry.
 * 
 * @param head Pointer to the head of the list.
 * @param clusterEntry Cluster entry to add.
 * @return Status of the addition.
 */
dirNodeStatus_t addEntry(dirNode_t **head, uint16_t clusterEntry);

/**
 * @brief Add next directory at root directory.
 * 
 * @param head Pointer to the head of the list.
 * @param clusterEntry Cluster entry to add.
 * @return Status of the addition.
 */
dirNodeStatus_t addEntryReverse(dirNode_t **head, uint16_t clusterEntry);

/**
 * @brief Delete entry when going back to previous directory.
 * 
 * @param head Pointer to the head of the list.
 * @return Status of the deletion.
 */
dirNodeStatus_t deleteEntry(dirNode_t **head);

/**
 * @brief Delete all cluster entries.
 * 
 * @param head Pointer to the head of the list.
 * @return Status of the deletion.
 */
dirNodeStatus_t deleteAllEntries(dirNode_t **head);

/**
 * @brief Make a copy of the linked list.
 * 
 * @param dest Pointer to the destination list.
 * @param source Pointer to the source list.
 * @return Status of the copy operation.
 */
dirNodeStatus_t copyAllEntries(dirNode_t **dest, dirNode_t **source);

#endif /* DIR_NODE_H */