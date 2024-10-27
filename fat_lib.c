#include <time.h>
#include "fat_lib.h"
#include "gui.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/
static dirNode_t *s_pHEAD = NULL;
static BootSector_t s_gBootSector;

/*******************************************************************************
 * Code
 ******************************************************************************/
error_code_t initFat(FILE *file)
{
    HAL_init(file);
    s_pHEAD = NULL;
    init(&s_pHEAD, 0);
    read_boot_sector(&s_gBootSector);
    return ERROR_OK;
}

error_code_t listDirectory(uint8_t showHidden, headerTableCallback headerTableCallback, contentCallback contentCallback)
{
    error_code_t status = ERROR_OK;
    DirectoryEntry_t entry;
    if (s_pHEAD->clusterEntry == 0)
    {
        HAL_fseek(getRootDirStart(&s_gBootSector));

        int i;
        headerTableCallback();
        for (i = 0; i < s_gBootSector.rootEntryCount; i++)
        {
            status = getEntryInRoot(&s_gBootSector, &entry);

            if (entry.name[0] == 0x00)
            {
                continue;
            }

            if (entry.attr == ATTR_LONG_NAME)
            {
                continue;
            }

            if ((entry.name[0] != 0xE5) && (entry.startCluster != 0))
            {
                if (showHidden || !(entry.attr & ATTR_HIDDEN))
                {
                    contentCallback(&entry);
                }
            }
        }
    }
    else
    {
        uint16_t tempCluster = s_pHEAD->clusterEntry;
        HAL_fseek(getAddressCluster(&s_gBootSector, tempCluster));
        int i;
        headerTableCallback();
        uint16_t numberOfEntry = (s_gBootSector.bytesPerSector * s_gBootSector.sectorsPerCluster) / (sizeof(DirectoryEntry_t));
        for (i = 0; i < numberOfEntry; i++)
        {
            getEntry(&s_gBootSector, &entry);
            if (entry.name[0] == 0x00)
            {
                continue;
            }

            if (entry.attr == ATTR_LONG_NAME)
            {
                continue;
            }

            if (entry.name[0] != 0xE5)
            {
                if (showHidden || (!(entry.attr & ATTR_HIDDEN) && i > 1))
                {
                    contentCallback(&entry);
                }
            }

            if (i == numberOfEntry)
            {
                tempCluster = getNextCluster(&s_gBootSector, tempCluster);
                if (tempCluster == FREE_CLUSTER ||
                    tempCluster == RESERVED_CLUSTER ||
                    tempCluster == BAD_CLUSTER ||
                    ((tempCluster >= 0xFF8) && (tempCluster <= 0xFFF)))
                {
                    break;
                }
                else
                {
                    i = 0; // loop again
                }
            }
        }
    }
    return status;
}

void help(helpCallback helpCallback)
{
    if (helpCallback)
    {
        helpCallback();
    }
}

error_code_t changeDirectory(char *dir)
{
    error_code_t status = ERROR_OK;
    DirectoryEntry_t entry;
    if (strcmp(dir, ".") == 0)
    {
        // do nothing
        status = ERROR_NO_DIRECTORY_CHANGE;
    }
    else if (s_pHEAD->clusterEntry == 0)
    {
        if (strcmp(dir, "..") == 0)
        {
            status = ERROR_NO_MORE_PREV_DIR;
        }
        else
        {
            status = findNameInRoot(&s_gBootSector, dir, &entry, FOLDER_ATTRIBUTE_TYPE);
            if (status == ERROR_OK)
            {
                addEntry(&s_pHEAD, entry.startCluster);
            }
        }
    }
    else
    {
        if (strcmp(dir, "..") == 0)
        {
            status = goPrevDirectory();
        }
        else
        {
            status = findName(&s_gBootSector, dir, s_pHEAD->clusterEntry, &entry, FOLDER_ATTRIBUTE_TYPE);
            if (status == ERROR_OK)
            {
                addEntry(&s_pHEAD, entry.startCluster);
            }
        }
    }
    return status;
}

error_code_t goPrevDirectory(void)
{
    error_code_t status = ERROR_OK;
    status = deleteEntry(&s_pHEAD);
    return status;
}

error_code_t copyDirectory(dirNode_t **dest)
{
    error_code_t status = ERROR_OK;
    status = copyAllEntries(dest, &s_pHEAD);
    return status;
}

void restoreDirectory(dirNode_t **source)
{
    deleteAllEntries(&s_pHEAD);
    s_pHEAD = *source;
}

error_code_t showFileContent(char *filename)
{
    error_code_t status = ERROR_OK;
    DirectoryEntry_t entry;
    if (s_pHEAD->clusterEntry == 0)
    {
        status = findNameInRoot(&s_gBootSector, filename, &entry, FILE_ATTRIBUTE_TYPE);
        if (status == ERROR_OK)
        {
            readFile(&s_gBootSector, &entry);
        }
        else
        {
            status = ERROR_FILE_NOT_FOUND;
        }
    }
    else
    {
        status = findName(&s_gBootSector, filename, s_pHEAD->clusterEntry, &entry, FILE_ATTRIBUTE_TYPE);
        if (status == ERROR_OK)
        {
            readFile(&s_gBootSector, &entry);
        }
        else
        {
            status = ERROR_FILE_NOT_FOUND;
        }
    }
    return status;
}

error_code_t createFolder(char *dir)
{
    DirectoryEntry_t entry;
    entry.attr = ATTR_DIRECTORY;
    entry.fileSize = 0;
    uint32_t cluster = 0;
    cluster = findFreeCluster(&s_gBootSector);
    if (cluster == 0)
    {
        return ERROR_INVALID_CLUSTER;
    }
    else
    {
        printf("Cluster: %d\n", cluster);
        markClusterUsed(cluster, &s_gBootSector);
    }

    entry.startCluster = cluster;
    strncpy(entry.name, dir, 8);
    strncpy(entry.ext, "   ", 3);

    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    entry.time.hour = timeinfo->tm_hour;
    entry.time.min = timeinfo->tm_min;
    entry.time.sec = timeinfo->tm_sec / 2;             /* Assuming seconds are stored in 2-second increments */
    entry.date.year = timeinfo->tm_year + 1900 - 1980; /* FAT year is based at 1980 */
    entry.date.month = timeinfo->tm_mon + 1;
    entry.date.day = timeinfo->tm_mday;

    if (s_pHEAD->prev == NULL)
    {
        
        HAL_fseek(getRootDirStart(&s_gBootSector));
        int i;
        for (i = 0; i < s_gBootSector.rootEntryCount; i++)
        {
            DirectoryEntry_t temp;
            getEntryInRoot(&s_gBootSector, &temp);
            if (temp.name[0] == 0x00 || temp.name[0] == 0xE5)
            {
                HAL_fseek(getRootDirStart(&s_gBootSector) + i * sizeof(DirectoryEntry_t));
                if(HAL_fwrite(&entry, sizeof(DirectoryEntry_t), 1) != 1)
                {
                    return ERROR_WRITE_FAILURE;
                }
                return ERROR_OK;
            }
        }
    }
    else
    {
    }
    return ERROR_UNKNOWN;
}