#include "fat_lib.h"
#include "cli.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <image_file>\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "rb");
    initFat(fp);
    //listDirectory(0);
    changeDirectory("DOC");
    changeDirectory("NEW");
    //changeDirectory("PIC");
    listDirectory(0);

    printf("\nNO ERROR\n");
    return 0;
}
