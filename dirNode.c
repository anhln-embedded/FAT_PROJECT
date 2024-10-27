#include <stdlib.h>
#include "dirNode.h"

dirNodeStatus_t init(dirNode_t** head, uint16_t clusterEntry) {
    dirNodeStatus_t status = DIR_NODE_STATUS_OK;
    if (*head != NULL) {
        status = DIR_NODE_STATUS_SECOND_INIT;
    }
    else {
        dirNode_t* temp = malloc(sizeof(dirNode_t));
        if (temp == NULL) {
            status = DIR_NODE_STATUS_INSUFFICIENT_MEMORY;
        }
        else {
            temp->prev = NULL;
            temp->clusterEntry = clusterEntry;
            *head = temp;
        }
    }
    return status;
}

dirNodeStatus_t addEntry(dirNode_t** head, uint16_t clusterEntry) {
    dirNodeStatus_t status = DIR_NODE_STATUS_OK;
    dirNode_t* node = *head;
    if (node == NULL) {
        status = init(head, clusterEntry);
    }
    else {
        dirNode_t* temp = NULL;
        status = init(&temp, clusterEntry);
        if (status == DIR_NODE_STATUS_OK) {
            temp->prev = node;
            *head = temp;
        }
    }
    return status;
}

dirNodeStatus_t addEntryReverse(dirNode_t** head, uint16_t clusterEntry) {
    dirNodeStatus_t status = DIR_NODE_STATUS_OK;
    dirNode_t* node = *head;
    if (node == NULL) {
        status = init(head, clusterEntry);
    }
    else {
        dirNode_t* temp = NULL;
        while(node->prev != NULL) {
            node = node->prev;
        }
        status = init(&temp, clusterEntry);
        if (status == DIR_NODE_STATUS_OK) {
            node->prev = temp;
        }
    }
    return status;
}

dirNodeStatus_t deleteEntry(dirNode_t** head) {
    dirNode_t* pos = *head;
    dirNodeStatus_t status = DIR_NODE_STATUS_OK;
    if (pos == NULL) {
        status = DIR_NODE_STATUS_EMPTY_DIRECTORY;
    }
    else {
        if (pos->prev != NULL) {
            *head = pos->prev;
            free(pos);
        }
        else {
            status = DIR_NODE_STATUS_ROOT_DIR_ONLY;
        }
    }
    return status;
}

dirNodeStatus_t deleteAllEntries(dirNode_t** head) {
    dirNodeStatus_t status = DIR_NODE_STATUS_OK;
    dirNode_t* pos = *head;
    if (pos != NULL) {
        while(pos->prev != NULL) {
            dirNode_t* temp = pos->prev;
            free(pos);
            pos = temp;
        }
        free(pos);
    }
    return status;
}

dirNodeStatus_t copyAllEntries(dirNode_t** dest, dirNode_t** source) {
    dirNodeStatus_t status = DIR_NODE_STATUS_OK;
    dirNode_t* pSource = *source;
    dirNode_t* pDest = *dest;
    if (pDest != NULL) {
        status = DIR_NODE_STATUS_NOT_NULL;
    }
    else if (pSource == NULL) {
        status = DIR_NODE_STATUS_EMPTY_DIRECTORY;
    }
    else {
        while(pSource != NULL) {
            addEntryReverse(dest, pSource->clusterEntry);
            if (status != DIR_NODE_STATUS_OK) {
                deleteAllEntries(dest);
                break;
            }
            else {
                pSource = pSource->prev;
            }
        }
    }
    return status;
}