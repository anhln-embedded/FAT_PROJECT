#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "cli.h"
#include "fat_lib.h"

char input[MAX_INPUT];
char command[MAX_INPUT];
char argument[MAX_INPUT];
char current_path[MAX_PATH_LENGTH] = "/";  // Start in the root directory

void deleteDirectory(char *current_path) {
    if (strcmp(current_path, "/") == 0) {
        printf("Error: Already at the root directory\n");
    } else {
        // Remove the last directory from the path
        char *last_slash = strrchr(current_path, '/');
        if (last_slash != NULL) {
            *last_slash = '\0';  // Truncate the path
            if (strlen(current_path) == 0) {
                strcpy(current_path, "/");  // If it becomes empty, set it back to root
            }
        }
    }
}

void trim_trailing_whitespace(char *str) {
    int end = strlen(str) - 1;
    while (end >= 0 && isspace((unsigned char)str[end])) {
        str[end] = '\0';  // Set trailing spaces to null character
        end--;
    }
}

void test_cd(char *current_path, char *new_path) {
    char temp_path[MAX_PATH_LENGTH];
    strcpy(temp_path, current_path);  // Store the original path

    char *token = strtok(new_path, "/");
    int change_count = 0;

    // printf("%s\n", token);
    while (token != NULL) {
        if (changeDirectory(token) == 0) {
            // Update the path
            if (strcmp(token, "..") == 0) {
                deleteDirectory(current_path);
            } 
            else if (strcmp(current_path, "/") == 0) {
                snprintf(current_path, MAX_PATH_LENGTH, "/%s", token);
            } else {
                snprintf(current_path + strlen(current_path), MAX_PATH_LENGTH - strlen(current_path), "/%s", token);
            }
            change_count++;  // Count successful changes
        } else {
            printf("Error: Failed to change directory to '%s'\n", token);
            // If error, restore the path by calling deleteEntry() for each successful change
            while (change_count > 0) {
                goPrevDirectory();
                deleteDirectory(current_path);
                change_count--;
            }
            return;
        }
        // Get the next directory in the path
        token = strtok(NULL, "/");
    }
}

void test_ls(int show_all) {
    listDirectory(show_all);
}

void test_help() {
    help();
}

void test_cat(char *file_name) {
    showFileContent(file_name);
}

void test_mkdir(char *dir_name) {
    printf("Creating directory: %s\n", dir_name);
}

void test_rm(char *target) {
    printf("Removing: %s\n", target);
}

void cmdLineInterface() {
    printf("Command Line Interpreter:\n");

    while (1) {
        printf("%s> ", current_path);

        fgets(input, MAX_INPUT, stdin);
        input[strcspn(input, "\n")] = '\0';
        trim_trailing_whitespace(input);

        if (strcmp(input, "exit") == 0) {
            printf("Exiting...\n");
            break;
        }

        argument[0] = '\0';  // Initialize argument to empty string
        sscanf(input, "%s %s", command, argument);

        // Handle each command
        if (strcmp(command, "cd") == 0) {
            if (strlen(argument) == 0) {
                printf("Error: 'cd' requires a directory argument\n");
            } else {
                test_cd(current_path, argument);
            }
        } else if (strcmp(command, "ls") == 0) {
            if (strcmp(argument, "-a") == 0) {
                test_ls(1);  // ls -a (show all)
            } else if (strlen(argument) == 0) {
                test_ls(0);  // ls (normal)
            } else {
                printf("Error: invalid argument for 'ls'\n");
            }
        } else if (strcmp(command, "mkdir") == 0) {
            if (strlen(argument) == 0) {
                printf("Error: 'mkdir' requires a directory name argument\n");
            } else {
                test_mkdir(argument);
            }
        } else if (strcmp(command, "rm") == 0) {
            if (strlen(argument) == 0) {
                printf("Error: 'rm' requires a file or directory name argument\n");
            } else {
                test_rm(argument);
            }
        } else if (strcmp(command, "cat") == 0) {
            if (strlen(argument) == 0) {
                printf("Error: 'cat' requires a file name argument\n");
            } else {
                test_cat(argument);
            }
        } else if (strcmp(command, "help") == 0) {
            if (strlen(argument) == 0) {
                test_help();  // ls (normal)
            } else {
                printf("Error: invalid argument for 'help'\n");
            }
        }
        else {
            printf("Error: Unknown command: %s\n", command);
        }
    }
}