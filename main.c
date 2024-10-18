#include "read_infor.h"
#include "DirectoryEntry.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <image_file>\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "rb");
    if (fp == NULL) {
        perror("Error opening file");
        return 1;
    }

    BootSector_t bs;
    read_boot_sector(fp, &bs);
    // display_boot_sector_info(&bs);
    read_root_directory(fp, &bs);

    fclose(fp);
    return 0;

}