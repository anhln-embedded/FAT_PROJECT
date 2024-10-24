#include "readClusterData.h"
#include "fat_lib.h"

// display contents in folder
void readFolder(uint16_t startCluster, FILE *file)
{
    int i = 0;
    DirectoryEntry folder[ENTRIES_PER_CLUSTER];
    fseek(file, (startCluster + 33 - 2) * 512, SEEK_SET);

    uint8_t buffer[CLUSTER_SIZE];
    fread(buffer, 1, CLUSTER_SIZE, file);
    /// convert buffet --> struct

    for (i = 0; i < ENTRIES_PER_CLUSTER; i++)
    {
        DirectoryEntry *entryTemp = (DirectoryEntry *)&buffer[i * DIR_ENTRY_SIZE];
        folder[i] = *entryTemp;
    }

    // Printf
    for (i = 0; i < ENTRIES_PER_CLUSTER; i++)
    {
        DirectoryEntry entry = folder[i];

        // check name = temp
        if (entry.name[0] == 0x00)
        {
            break;
        }

        // check error
        if (entry.name[0] == 0xE5)
        {
            continue;
        }

        printf("Name: %.8s", entry.name);

        if (entry.attr == ATTR_DIRECTORY)
        {
            printf(".%.3s", entry.ext); // extern name
        }

        printf("\n");

        // display attribute
        if (entry.attr == ATTR_DIRECTORY)
        {
            printf("Type: Folder\n");
        }
        else
        {
            printf("Type: File\n");
            printf("Size: %d bytes\n", entry.fileSize);
            printf("next cluster: %x\n", entry.startCluster);
        }

        // display time date
        printf("Date: %d/%d/%d\n", entry.date.year + 1980, entry.date.month, entry.date.day);
        printf("Time: %d:%d:%d\n", entry.time.hour, entry.time.min, entry.time.sec);
        printf("\n");

        // printf("Start Cluster: %d\n\n", entry.startCluster);
    }
}

void readFile(uint16_t startCluster, FILE *file)
{
    fseek(file, (startCluster + 33 - 2) * 512, SEEK_SET);
    uint8_t buffer[CLUSTER_SIZE + 1];
    fread(buffer, 1, CLUSTER_SIZE, file);
    buffer[CLUSTER_SIZE] = '\0';
    printf("%s", buffer);
    // printf("-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-");
}

uint16_t findNameInCluster(uint16_t startCluster, char *filename, FILE *file)
{
    fseek(file, (startCluster + 33 - 2) * 512, SEEK_SET);
    DirectoryEntry *entry;
    char arr[sizeof(DirectoryEntry)];
    uint16_t counter = 0;
    {
        fread(arr, sizeof(DIR_ENTRY_SIZE), 1, file);
        entry = (DirectoryEntry *)arr;
        if (strstr(entry->name, filename) != NULL)
        {
            return entry->startCluster;
        }
        counter += 1;
    }
    while (counter != ENTRIES_PER_CLUSTER-1);
}

uint16_t findName (uint16_t startCluster, char *filename, FILE *file)
{
    uint16_t counter = 0;
    uint16_t tempNextCluster = startCluster;
    do
    {
        findNameInCluster(tempNextCluster, filename, file);
        counter +=1;
        if(counter == ENTRIES_PER_CLUSTER-1)
        {
            tempNextCluster = getNextCluster(tempNextCluster, file);
            counter == 0;
        }
    } while (1);
    
}

/* // example
do
        {
            readFolder(tempStartCluster, gFile);
            tempStartCluster = getNextCluster(tempStartCluster, gFile); //get

            if (tempStartCluster == FREE_CLUSTER ||
                tempStartCluster == RESERVED_CLUSTER ||
                tempStartCluster == BAD_CLUSTER ||
                tempStartCluster == EOF_CLUSTER)
            {
                break;
            }
        } while (1);

*/

/*
void readFolder(uint16_t startCluster, FILE *file)
{
    int i = 0;
    DirectoryEntry folder[ENTRIES_PER_CLUSTER];
    fseek(file, (startCluster + 33 - 2) * 512, SEEK_SET);

    uint8_t buffer[CLUSTER_SIZE];
    fread(buffer, 1, CLUSTER_SIZE, file);
    /// convert buffet --> struct

    for (i = 0; i < ENTRIES_PER_CLUSTER; i++)
    {
        DirectoryEntry *entryTemp = (DirectoryEntry *)&buffer[i * DIR_ENTRY_SIZE];
        folder[i] = *entryTemp;
    }

    ////////////////////////////////

    for (i = 0; i < ENTRIES_PER_CLUSTER; i++)
    {
        DirectoryEntry entry = folder[i];

        // check name = temp
        if (entry.name[0] == 0x00)
        {
            break;
        }

        // check error
        if (entry.name[0] == 0xE5)
        {
            continue;
        }

        printf("Name: %.8s", entry.name);

        if (!(entry.attr & ATTR_DIRECTORY))
        {
            printf(".%.3s", entry.ext); // extern name
        }

        printf("\n");

        // display attribute
        if (entry.attr == ATTR_DIRECTORY)
        {
            printf("Type: Folder\n");
        }
        else
        {
            printf("Type: File\n");
            printf("Size: %d bytes\n", entry.fileSize);
        }

        // display time date
        printf("Date: %d/%d/%d\n", entry.date.year + 1980, entry.date.month, entry.date.day);
        printf("Time: %d:%d:%d\n", entry.time.hour, entry.time.min, entry.time.sec);
        printf("\n");

        // printf("Start Cluster: %d\n\n", entry.startCluster);
    }
}
*/