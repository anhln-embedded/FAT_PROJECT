#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cli.h"
// #include "fat_lib.h"

char input[MAX_INPUT];
char command[MAX_INPUT];
char argument[MAX_INPUT];
char current_path[MAX_PATH_LENGTH] = "/";  // Start in the root directory

void test_cd(char *current_path, char *new_dir) {
    // error_t status = changeDirectory(new_dir);
    if (strcmp(new_dir, "..") == 0) {
        /**
         * Going to the parent directory
         * If this is root dir, status should be ERROR 
         */
        // if (status == ERROR) {
        if (strcmp(current_path, "/") == 0) {
            printf("Error: Already at the root directory\n");
        } else {
            /* Remove the last directory from the path */
            char *last_slash = strrchr(current_path, '/');
            if (last_slash != NULL) {
                *last_slash = '\0';  // Truncate the path
                if (strlen(current_path) == 0) {
                    strcpy(current_path, "/");  // If it becomes empty, set it back to root
                }
            }
        }
    } else if (new_dir[0] == '/') {
        /* Absolute path (starting from root) */
        // if (status == ERROR_OK) 
        strcpy(current_path, new_dir);
    } else {
        /* Relative path (add new directory to current path) */ 
        if (strcmp(current_path, "/") == 0) {
            /* If we're at the root, we don't need to add a slash before the new dir */ 
            snprintf(current_path, MAX_PATH_LENGTH, "/%s", new_dir);
        } else {
            // Add the new directory to the current path
            snprintf(current_path + strlen(current_path), MAX_PATH_LENGTH - strlen(current_path), "/%s", new_dir);
        }
    }

    // Validation check: here we just test a valid directory change.
    // You can expand this with actual directory existence checks if needed.
    if (strlen(current_path) > MAX_PATH_LENGTH - 1) {
        printf("Error: Path length exceeded maximum limit\n");
    } else {
        printf("Current directory: %s\n", current_path);
    }
}

void test_ls(int show_all) {
    // listDirectory(show_all);
    if (show_all)
        printf("Listing all directories and files (including hidden files)...\n");
    else
        printf("Listing directories and files...\n");
}

void test_help() {
    // help();  
    printf("Print user manual.\n");
}

void test_mkdir(char *dir_name) {
    printf("Creating directory: %s\n", dir_name);
}

void test_rm(char *target) {
    printf("Removing: %s\n", target);
}

void test_touch(char *file_name) {
    printf("Creating file: %s\n", file_name);
}

void cmdLineInterface() {
    printf("Command Line Interpreter:\n");

    while (1) {
        printf("%s> ", current_path);

        fgets(input, MAX_INPUT, stdin);
        input[strcspn(input, "\n")] = '\0';

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
        } else if (strcmp(command, "touch") == 0) {
            if (strlen(argument) == 0) {
                printf("Error: 'touch' requires a file name argument\n");
            } else {
                test_touch(argument);
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