#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

int is_directory_empty(char dirname[]) {
    struct dirent *entry;
    int count = 0;
    
    DIR *dir = opendir(dirname);
    if (dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            count++;
        }
    }

    closedir(dir);

    return count == 0;
}

int count_empty_directories(char dirname[]) {
    int empty_directories = 0;
    struct dirent *entry;
    struct stat statbuf;
    char path[1024];

    DIR *dir = opendir(dirname);
    if (dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {
        snprintf(path, sizeof(path), "%s/%s", dirname, entry->d_name);
        if (stat(path, &statbuf) == -1) {
            perror("stat");
            exit(EXIT_FAILURE);
        }
        if (S_ISDIR(statbuf.st_mode)) {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                if (is_directory_empty(path)) {
                    empty_directories++;
                }
                empty_directories += count_empty_directories(path); // 递归处理子目录
            }
        }
    }

    closedir(dir);

    return empty_directories;
}

int main() {
    char target_directory[] = "treeroot";
   
    int empty_directories_count = count_empty_directories(target_directory);
    
    printf("There are %d empty dirs in '%s' .\n", empty_directories_count,target_directory);

    return 0;
}
