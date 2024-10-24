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
    listDirectory(0);
    showFileContent("SAMPLE.TXT");
    changeDirectory("DOC");

    // // changeDirectory("NEW");

    // // listDirectory(0);

    // printf("NO ERROR\n");
    // cmdLineInterface();
    printf("NO ERROR\n");
    return 0;
}
