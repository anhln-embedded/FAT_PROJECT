#include "read_infor.h"
#include "DirectoryEntry.h"
#include "read_cluster.h"


int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <image_file>\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "rb");
    if (fp == NULL)
    {
        perror("Error opening file");
        return 1;
    }

    BootSector_t bs;
    read_boot_sector(fp, &bs);
    display_boot_sector_info(&bs);



    read_root_directory(fp, &bs);
    fseek(fp, getRootDirStart(&bs), SEEK_SET);

    int i;
    for (i = 0; i < bs.rootEntryCount; i++)
    {
        DirectoryEntry entry;
        if (fread(&entry, sizeof(DirectoryEntry), 1, fp) != 1)
        {
            perror("Error reading directory entry");
            return 1;
        }

        if (entry.name[0] == 0x00)
        {
            break;
        }
        if (entry.name[0] != 0xE5 && entry.startCluster != 0)
        {
            printf("%x: ", entry.name[0]);
            printf("File Name: %.8s.%.3s\n", entry.name, entry.ext);
            if (entry.attr & 0x10)
            {
                printf("Directory\n");
            }
            else
            {
                printf("File size: %u bytes\n", entry.fileSize);
                char *content = getFileContent(fp, &bs, &entry);       
                (void) content;
                // free(content);
            
            }
        }
    }
    
    fclose(fp);
    return 0;
}