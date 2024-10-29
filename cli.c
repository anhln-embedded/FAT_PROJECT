#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "cli.h"
#include "fat_lib.h"
#include "gui.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/
char input[MAX_INPUT];
char command[MAX_INPUT];
char argument[MAX_INPUT];
char current_path[MAX_PATH_LENGTH] = "/"; /* Start in the root directory */

/*******************************************************************************
 * Code
 ******************************************************************************/
void deleteDirectory(char *current_path)
{
    if (strcmp(current_path, "/") == 0)
    {
        printf("Error: Already at the root directory\n");
    }
    else
    {
        /* Remove the last directory from the path */
        char *last_slash = strrchr(current_path, '/');
        if (last_slash != NULL)
        {
            *last_slash = '\0'; /* Truncate the path */
            if (strlen(current_path) == 0)
            {
                strcpy(current_path, "/"); /* If it becomes empty, set it back to root */
            }
        }
    }
}

void trim_trailing_whitespace(char *str)
{
    int end = strlen(str) - 1;
    while (end >= 0 && isspace((unsigned char)str[end]))
    {
        str[end] = '\0'; /* Set trailing spaces to null character */
        end--;
    }
}

void test_cd(char *current_path, char *new_path)
{
    /* Make a copy of directory list */
    dirNode_t *temp = NULL;
    copyDirectory(&temp);

    char temp_path[MAX_PATH_LENGTH];
    strcpy(temp_path, current_path); /* Store the original path */

    char *token = strtok(new_path, "/");
    int change_count = 0;

    while (token != NULL)
    {
        error_code_t status = changeDirectory(token);
        if (status == ERROR_OK)
        {
            /* Update the path */
            if (strcmp(token, "..") == 0)
            {
                deleteDirectory(current_path);
            }
            else if (strcmp(current_path, "/") == 0)
            {
                snprintf(current_path, MAX_PATH_LENGTH, "/%s", token);
            }
            else
            {
                snprintf(current_path + strlen(current_path), MAX_PATH_LENGTH - strlen(current_path), "/%s", token);
            }
            change_count = 1; /* Indicate directory changed */
        }
        else
        {
            if (status == ERROR_WRONG_ATTRIBUTE)
            {
                printf("Error: Try to access a file as subdirectory. Use 'cat %s' instead.\n", token);
            }
            else if (status == ERROR_NO_MORE_PREV_DIR)
            {
                if (!change_count)
                {
                    printf("Error: Cannot access previous directory at root directory\n");
                }
                else
                {
                    printf("Error: Cannot access directory\n");
                }
            }
            else if (status != ERROR_NO_DIRECTORY_CHANGE)
            {
                printf("Error: No directory name '%s' found\n", token);
            }
            /* If error, restore the path by calling deleteEntry() for each successful change */
            if (change_count)
            {
                restoreDirectory(&temp);
                strcpy(current_path, temp_path);
            }
            return;
        }
        /* Get the next directory in the path */
        token = strtok(NULL, "/");
    }
}

void test_ls(int show_all)
{
    listDirectory(show_all, printHeader, printDirectoryEntry);
}

void test_help(void)
{
    help(printHelp);
}

void test_cat(char *file_name)
{
    showFileContent(file_name);
}

void test_mkdir(char *dir_name)
{
    printf("%s\n", get_error_message(createFolder(dir_name)));
}

void test_rm(char *target)
{
    printf("Removing: %s\n", target);
    printf("%s\n", get_error_message(deleteFile(target)));
}

void test_rmdir(char *target)
{
    printf("Removing: %s\n", target);
    printf("%s\n", get_error_message(deleteFolder(target)));
}

void cmdLineInterface(void)
{
    printf("Command Line Interpreter:\n");

    while (1)
    {
        strcpy(input, "");
        strcpy(command, "");
        printf("\033[31mT\033[32me\033[33ma\033[34mm\033[35m \033[36m1\033[0m%s$ ", current_path);

        fgets(input, MAX_INPUT, stdin);
        input[strcspn(input, "\n")] = '\0';
        trim_trailing_whitespace(input);

        if (strcmp(input, "exit") == 0)
        {
            printf("Exiting...\n");
            break;
        }

        argument[0] = '\0'; /* Initialize argument to empty string */
        sscanf(input, "%s %s", command, argument);

        /* Handle each command */
        if (strcmp(command, "cd") == 0)
        {
            if (strlen(argument) == 0)
            {
                printf("Error: 'cd' requires a directory argument\n");
            }
            else
            {
                test_cd(current_path, argument);
            }
        }
        else if (strcmp(command, "ls") == 0)
        {
            if (strcmp(argument, "-a") == 0)
            {
                test_ls(1); /* ls -a (show all) */
            }
            else if (strlen(argument) == 0)
            {
                test_ls(0); /* ls (normal) */
            }
            else
            {
                printf("Error: invalid argument for 'ls'\n");
            }
        }
        else if (strcmp(command, "mkdir") == 0)
        {
            if (strlen(argument) == 0)
            {
                printf("Error: 'mkdir' requires a directory name argument\n");
            }
            else
            {
                test_mkdir(argument);
            }
        }
        else if (strcmp(command, "rm") == 0)
        {
            if (strlen(argument) == 0)
            {
                printf("Error: 'rm' requires a file or directory name argument\n");
            }
            else
            {
                test_rm(argument);
            }
        }
        else if (strcmp(command, "rmdir") == 0)
        {
            if (strlen(argument) == 0)
            {
                printf("Error: 'rm' requires a file or directory name argument\n");
            }
            else
            {
                test_rmdir(argument);
            }
        }
        else if (strcmp(command, "cat") == 0)
        {
            if (strlen(argument) == 0)
            {
                printf("Error: 'cat' requires a file name argument\n");
            }
            else
            {
                test_cat(argument);
            }
        }
        else if (strcmp(command, "help") == 0)
        {
            if (strlen(argument) == 0)
            {
                test_help(); /* ls (normal) */
            }
            else
            {
                printf("Error: invalid argument for 'help'\n");
            }
        }
        else if (strcmp(command, "") == 0)
        {
            /* do nothing */
        }
        else
        {
            printf("Error: Unknown command: %s\n", command);
        }
    }
}