#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "check_fat.h"

uint16_t temp = 0x200 + 0xfe;

int main()
{
    FILE *file = fopen("floppy.img", "rb");
    if (file == NULL) {
        printf("Error opening file");
        return EXIT_FAILURE;
    }



    do
    {
        temp = readEntry(temp, file);
        printf("%03x \n", temp);

        // dk
        if(temp - 0x200 == 0x000)
        {
            printf("cluster's empty");
            break;
        }
        else if((temp - 0x200) <= 0xFFF && (temp - 0x200) >= 0xFF8)
        {
            printf("EOF");
            break;
        }
        else if((temp - 0x200) <= 0xFF6 && (temp - 0x200) >= 0xFF0)
        {
            printf("Reserved Values");
            break;
        }
        else if (temp - 0x200 == 0xFF7)
        {
            printf("cluster error");
            break;
        }
    }while( temp != 0xFFF);

    fclose(file);
    return EXIT_SUCCESS;
}
