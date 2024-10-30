#include "hal.h"

/*******************************************************************************
* Variables
******************************************************************************/
char *path = NULL;
FILE *g_stream = NULL;

/*******************************************************************************
* Code
******************************************************************************/
void HAL_init(char *stream)
{
    path = stream;
    g_stream = fopen(path, "r+b");
    if (g_stream == NULL)
    {
        perror("Error opening file");
    }
}

size_t HAL_fread(void *ptr, size_t size, size_t count)
{
    return fread(ptr, size, count, g_stream);
}

int HAL_fseek(uint32_t offset)
{
    return fseek(g_stream, offset, SEEK_SET);
}

char HAL_fgetc(void)
{
    return fgetc(g_stream);
}

size_t HAL_fwrite(const void *ptr, size_t size, size_t count){
    return fwrite(ptr, size, count, g_stream);
}

void HAL_deinit(void){
    fclose(g_stream);
}