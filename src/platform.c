#include <stdio.h>
#include <string.h>

#include "platform.h"

#ifdef _WIN32

#include <direct.h>
#include <windows.h>
#include <fileapi.h>
char *get_cwd(char *buf, size_t size) {
    return _getcwd(buf, size);
}
char *get_full_path(const char *path) {
    return GetFullPathName(path, MAX_PATH, NULL, NULL);
}
int directory_is_empty(const char *path) {
    WIN32_FIND_DATA findFileData;

    HANDLE hFind = FindFirstFile(path, &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        perror("Failed to get directory information.");
        return -1;
    }

    do {
        if (findFileData.cFileName[0] != '.') {
            FindClose(hFind);
            return 0;
        }
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);

    return 1;
}

#else

#include <unistd.h>
#include <dirent.h>
char *get_cwd(char *buf, size_t size) {
    return getcwd(buf, size);
}
char *get_full_path(const char *path) {
    return realpath(path, NULL);
}
int directory_is_empty(const char *path) {
    DIR *dir = opendir(path);
    if (!dir) {
        fprintf(stderr, "Failed to get directory information.");
        return -1;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            closedir(dir);
            return 0;
        }
    }

    closedir(dir);

    return 1;
}

#endif