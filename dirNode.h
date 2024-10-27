#ifndef DIR_NODE_H
#define DIR_NODE_H

#include <stdio.h>
#include <stdint.h>

#define DIR_NODE_STATUS_OK                      0U
#define DIR_NODE_STATUS_INSUFFICIENT_MEMORY     1U
#define DIR_NODE_STATUS_SECOND_INIT             2U
#define DIR_NODE_STATUS_EMPTY_DIRECTORY         3U
#define DIR_NODE_STATUS_ROOT_DIR_ONLY           4U
#define DIR_NODE_STATUS_NOT_NULL                5U

typedef struct dir_node dirNode_t;
typedef uint8_t dirNodeStatus_t;

struct dir_node {
    dirNode_t* prev;
    uint16_t clusterEntry;
    // dirNode_t* next;
};

/* initialize double linked list */
dirNodeStatus_t init(dirNode_t** head, uint16_t clusterEntry);
/* add next directory entry */
dirNodeStatus_t addEntry(dirNode_t** head, uint16_t clusterEntry);
/* add next directory at root directory */
dirNodeStatus_t addEntryReverse(dirNode_t** head, uint16_t clusterEntry);
/* delete entry when go back to previous directory */
dirNodeStatus_t deleteEntry(dirNode_t** head);
/* delete all cluster entries */
dirNodeStatus_t deleteAllEntries(dirNode_t** head);
/* make a copy of linked list */
dirNodeStatus_t copyAllEntries(dirNode_t** dest, dirNode_t** source);

#endif      /* DIR_NODE_H */