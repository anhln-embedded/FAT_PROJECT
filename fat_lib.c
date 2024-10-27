#include "fat_lib.h"
#include "gui.h"

static dirNode_t *pHEAD = NULL;
static BootSector_t gBootSector;

error_code_t initFat(FILE *file)
{
    HAL_intit(file);
    pHEAD = NULL;
    init(&pHEAD, 0);
    read_boot_sector(&gBootSector);
    return ERROR_OK;
}

error_code_t listDirectory(uint8_t showHidden, headerTableCallback headerTableCallback, contentCallback contentCallback)
{
    error_code_t status = ERROR_OK;
    DirectoryEntry_t entry;
    if (pHEAD->clusterEntry == 0)
    {

        HAL_fseek(getRootDirStart(&gBootSector));

        int i;
        headerTableCallback();
        for (i = 0; i < gBootSector.rootEntryCount; i++)
        {
            status = getEntryInRoot(&gBootSector, &entry);

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
        uint16_t tempCluster = pHEAD->clusterEntry;
        HAL_fseek(getAddressCluster(&gBootSector, tempCluster));
        int i;
        headerTableCallback();
        uint16_t numberOfEntry = (gBootSector.bytesPerSector * gBootSector.sectorsPerCluster) / (sizeof(DirectoryEntry_t));
        for (i = 0; i < numberOfEntry; i++)
        {
            getEntry(&gBootSector, &entry);
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
                tempCluster = getNextCluster(tempCluster);
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
    else if (pHEAD->clusterEntry == 0)
    {
        if (strcmp(dir, "..") == 0)
        {
            status = ERROR_NO_MORE_PREV_DIR;
        }
        else
        {
            status = findNameInRoot(&gBootSector, dir, &entry, FOLDER_ATTRIBUTE_TYPE);
            if (status == ERROR_OK)
            {
                addEntry(&pHEAD, entry.startCluster);
            }
        }
    }
    else
    {
        if (strcmp(dir, "..") == 0)
        {
            status = goPrevDirectory();
        }
        else {
            status = findName(&gBootSector, dir, pHEAD->clusterEntry, &entry, FOLDER_ATTRIBUTE_TYPE);
            if (status == ERROR_OK)
            {
                addEntry(&pHEAD, entry.startCluster);
            }
        }
    }
    return status;
}

error_code_t goPrevDirectory()
{
    error_code_t status = ERROR_OK;
    status = deleteEntry(&pHEAD);
    return status;
}

error_code_t copyDirectory(dirNode_t** dest) {
    error_code_t status = ERROR_OK;
    status = copyAllEntries(dest, &pHEAD);
    return status;
}

void restoreDirectory(dirNode_t** source) {
    deleteAllEntries(&pHEAD);
    pHEAD = *source;
}

error_code_t showFileContent(char *filename)
{
    error_code_t status = ERROR_OK;
    DirectoryEntry_t entry;
    if (pHEAD->clusterEntry == 0)
    {
        status = findNameInRoot(&gBootSector, filename, &entry, FILE_ATTRIBUTE_TYPE);
        if (status == ERROR_OK)
        {
            readFile(&gBootSector, entry.startCluster, &entry);
        }
        else
        {
            status = ERROR_FILE_NOT_FOUND;
        }
    }
    else
    {
        status = findName(&gBootSector, filename, pHEAD->clusterEntry, &entry, FILE_ATTRIBUTE_TYPE);
        if (status == ERROR_OK)
        {
            readFile(&gBootSector, entry.startCluster, &entry);
        }
        else
        {
            status = ERROR_FILE_NOT_FOUND;
        }
    }
    return status;
}