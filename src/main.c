#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define FALSE 0;
#define TRUE 1;

#define MAX_LINE_LENGTH 1024
#define MAX_TOKEN_LENGTH 128
#define MAX_VALUE_LENGTH 128

void trim_whitespace(char *str);
void generate_makefile(const char *project_name, const char *c_version);
char *get_cwd(char *buf, size_t size);
char *get_full_path(const char *path);

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

void trim_whitespace(char *str) {
    char *end;

    while (isspace(*str)) {
        str += 1;
    }

    end = str + strlen(str) - 1;

    while (end > str && isspace((unsigned char)*end)) {
        end -= 1;
    }

    *(end + 1) = '\0';
}

int main(int argc, char **argv) {
    char *toml_file_path = NULL;

    if (argc < 2) {
        FILE *fp = fopen("cproject.toml", "r");
        if (fp) {
            char *cwd = get_cwd(NULL, 0);
            if (cwd) {
                toml_file_path = malloc(strlen(cwd) + strlen("/cproject.toml") + 1);
                if (toml_file_path) {
                    strcpy(toml_file_path, cwd);
                    strcat(toml_file_path, "/cproject.toml");
                }
                free(cwd);
            }
            fclose(fp);
        }
        else {
            fprintf(stderr, "No cproject.toml found in the current working directory.\n\
If it exists somewhere else, try `%s path/to/cproject.toml`.\n", argv[0]);
            return EXIT_FAILURE;
        }
    }
    else {
        FILE *fp = fopen(argv[1], "r");
        if (!fp) {
            fprintf(stderr, "File %s does not exist.\n", get_full_path(argv[1]));
            return EXIT_FAILURE;
        }
        fclose(fp);

        toml_file_path = get_full_path(argv[1]);
    }

    printf("Using %s.\n", toml_file_path);

    return EXIT_SUCCESS;
}
