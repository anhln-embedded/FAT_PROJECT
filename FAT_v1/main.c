#include "fat_lib.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <image_file>\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "rb");
    initFat(fp);

    listDirectory(0);
    printf("==========================\n\n");
    // changeDirectory("DOC");
    // listDirectory(0);
    // showFileContent("SAMPLE2");

    // listDirectory(0);

    printf("\n--NO ERROR--\n");
}
