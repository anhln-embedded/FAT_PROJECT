#include "fat_lib.h"
#include "cli.h"

/*******************************************************************************
 * Code
 ******************************************************************************/
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <image_file>\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "rb+");
    if (fp == NULL)
    {
        perror("Error opening file");
        return 1;
    }

    error_code_t status = initFat(fp);
    if (status != ERROR_OK)
    {
        fprintf(stderr, "Error initializing FAT: %s\n", get_error_message(status));
        fclose(fp);
        return 1;
    }

    cmdLineInterface();

    fclose(fp);
    return 0;
}