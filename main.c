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
    listDirectory(0, NULL, NULL);
    changeDirectory("DOC");
    // changeDirectory("CONCEPTS.DOC");
    // changeDirectory("NEWAGAIN");
    //showFileContent("SAMPLE2.TXT");
    showFileContent("CONCEPTS.DOC");

    printf("\n---NO ERROR---\n");

    return 0;
}
