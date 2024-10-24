
#include "fat_lib.h"

static FILE *gFile = NULL;
static dirNode_t *pHEAD = NULL;
static BootSector_t gBootSector;

DirectoryEntry *ptr; // temp

error_t initFat(FILE *file)
{
    gFile = file;
    pHEAD = NULL;

    // read again*******************
    init(&pHEAD, 0);
    read_boot_sector(gFile, &gBootSector);
}

error_t listDirectory(uint8_t showHidden)
{
    if (pHEAD->clusterEntry == 0)
    {
        read_root_directory(gFile, &gBootSector);
    }
    else
    {
        printf("Cluster: %d\n", pHEAD->clusterEntry);
        uint16_t tempStartCluster = pHEAD->clusterEntry;
        // check FAT --> read Folder
        do
        {
            readFolder(tempStartCluster, gFile);
            tempStartCluster = getNextCluster(tempStartCluster, gFile);

            if (tempStartCluster == FREE_CLUSTER ||
                tempStartCluster == RESERVED_CLUSTER ||
                tempStartCluster == BAD_CLUSTER ||
                tempStartCluster == EOF_CLUSTER)
            {
                break;
            }
        } while (1);
    }
}

error_t help(void)
{
}

error_t changeDirectory(char *dir)
{
    if (pHEAD->clusterEntry == 0)
    {
        fseek(gFile, getRootDirStart(&gBootSector), SEEK_SET);

        int i;
        for (i = 0; i < gBootSector.rootEntryCount; i++)
        {
            DirectoryEntry entry;
            if (fread(&entry, sizeof(DirectoryEntry), 1, gFile) != 1)
            {
                perror("Error reading directory entry");
                return ERROR;
            }

            if (entry.name[0] == 0x00)
            {
                return ERROR;
            }

            if (entry.name[0] != 0xE5)
            {
                if (strstr(entry.name, dir) != NULL)
                {
                    addEntry(&pHEAD, entry.startCluster);
                    return ERROR_OK;
                }
            }
        }
    }
    else
    {
    }
}

error_t showFileContent(char *filename)
{
    if (pHEAD->clusterEntry == 0)
    {
        fseek(gFile, getRootDirStart(&gBootSector), SEEK_SET);

        int i;
        uint16_t tempStartCluster;
        for (i = 0; i < gBootSector.rootEntryCount; i++)
        {
            DirectoryEntry entry;
            if (fread(&entry, sizeof(DirectoryEntry), 1, gFile) != 1)
            {
                perror("Error reading directory entry");
                return ERROR;
            }

            if (entry.name[0] == 0x00)
            {
                return ERROR;
            }

            if (entry.name[0] != 0xE5)
            {
                if (NULL != strstr(entry.name, filename))
                {
                    tempStartCluster = entry.startCluster;
                    do
                    {
                        readFile(tempStartCluster, gFile);
                        tempStartCluster = getNextCluster(tempStartCluster, gFile); // get

                        if (tempStartCluster == FREE_CLUSTER ||
                            tempStartCluster == RESERVED_CLUSTER ||
                            tempStartCluster == BAD_CLUSTER ||
                            tempStartCluster == EOF_CLUSTER)
                        {
                            break;
                        }
                    } while (1);
                }
            }
        }
    }
}