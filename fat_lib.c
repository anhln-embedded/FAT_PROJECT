#include "fat_lib.h"

static FILE *gFile = NULL;
static dirNode_t *pHEAD = NULL;
static BootSector_t gBootSector;

error_code_t initFat(FILE *file)
{
    gFile = file;
    pHEAD = NULL;
    init(&pHEAD, 0);
    read_boot_sector(gFile, &gBootSector);
    return ERROR_OK;
}
error_code_t listDirectory(uint8_t showHidden)
{
    error_code_t status = ERROR_OK;
    DirectoryEntry_t entry;
    if (pHEAD->clusterEntry == 0)
    {
        fseek(gFile, getRootDirStart(&gBootSector), SEEK_SET);

        int i;
        for (i = 0; i < gBootSector.rootEntryCount; i++)
        {
            status = getEntryInRoot(gFile, &gBootSector, &entry);

            if (entry.name[0] == 0x00)
            {
                continue;
            }

            if (entry.name[0] != 0xE5)
            {
                if (showHidden || !(entry.attr & ATTR_HIDDEN))
                {
                    // Ánh cho file hiện thị ở đây
                }
                printf("File Name: %.8s.%.3s\n", entry.name, entry.ext);
                printf("Size: %u bytes\n", entry.fileSize);
                printf("Starting Cluster: %u\n", entry.startCluster);
                printf("Attributes: 0x%X\n", entry.attr);
                printf("Date: %d/%d/%d\n", entry.date.year + 1980, entry.date.month, entry.date.day);
                printf("Time: %d:%d:%d\n", entry.time.hour, entry.time.min, entry.time.sec);
                printf("------------------------------\n");
            }
        }
    }
    else
    {
        // Không phải là thư mục gốc
    }
    return status;
}
void help(void)
{
    printf("Commands available:\n");
    printf("ls : List files\n");
    printf("cd : Change directory\n");
    printf("cat : Display file contents\n");
    printf("exit : Exit terminal\n");
}
error_code_t changeDirectory(char *dir)
{
    error_code_t status = ERROR_OK;
    DirectoryEntry_t entry;
    if (pHEAD->clusterEntry == 0)
    {
        status = findNameInRoot(gFile, &gBootSector, dir, &entry);
        if (status == ERROR_OK)
        {
            printf("\nDirectory found\n");
            addEntry(&pHEAD, entry.startCluster);
        }
    }
    else
    {
        // Đọc từng cluster

    }
    return ERROR_OK;
}
error_code_t showFileContent(char *filename)
{
    error_code_t status = ERROR_OK;
    DirectoryEntry_t entry;
    if (pHEAD->clusterEntry == 0)
    {
        status = findNameInRoot(gFile, &gBootSector, filename, &entry);
        if (status == ERROR_OK)
        {
            readFile(entry.startCluster, gFile);
        }
        else
        {
            printf("File not found\n");
        }
    }
    else
    {
        // Đọc từng cluster

    }
    return ERROR_OK;
}