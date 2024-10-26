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
<<<<<<< HEAD
    listDirectory(0, NULL, NULL);
    showFileContent("SAMPLE.TXT");
=======
    //listDirectory(0);
>>>>>>> 5c6b0084e0998db9f56d72ec3bbaa66f05e91cad
    changeDirectory("DOC");
    changeDirectory("NEW");
    //changeDirectory("PIC");
    listDirectory(0);

<<<<<<< HEAD
    printf("NO ERROR\n");
=======
    printf("\nNO ERROR\n");
>>>>>>> 5c6b0084e0998db9f56d72ec3bbaa66f05e91cad
    return 0;
}
