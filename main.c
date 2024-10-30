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

    error_code_t status = initFat(argv[1]);
    if(status != ERROR_OK)
    {
        fprintf(stderr, "Error: Unable to initialize FAT library\n");
        deinintFat();
        return 1;
    }
    cmdLineInterface();

    deinintFat();
    return 0;
}