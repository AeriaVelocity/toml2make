#include <platform.h>

#ifdef _WIN32

#include <direct.h>
#include <fileapi.h>
char *get_cwd(char *buf, size_t size) {
    return _getcwd(buf, size);
}
char *get_full_path(const char *path) {
    return GetFullPathName(path, MAX_PATH, NULL, NULL);
}

#else

#include <unistd.h>
char *get_cwd(char *buf, size_t size) {
    return getcwd(buf, size);
}
char *get_full_path(const char *path) {
    return realpath(path, NULL);
}

#endif