#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "check_fat.h"


int main()
{

    FILE *file = fopen("floppy.img", "rb");
    if (file == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    read_fat_table(file);

    fclose("floppy.img");
    return EXIT_SUCCESS;
}
