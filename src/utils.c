#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "platform.h"
#include "config.h"

char* get_toml_file_path(int argc, char **argv) {
    char* toml_file_path = NULL;

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
            return NULL;
        }
    }
    else {
        FILE *fp = fopen(argv[1], "r");
        if (!fp) {
            fprintf(stderr, "File %s does not exist.\n", get_full_path(argv[1]));
            return NULL;
        }
        fclose(fp);

        toml_file_path = get_full_path(argv[1]);
    }

    return toml_file_path;
}

void assign_value(char *line, char *key, char *value) {
    if (strstr(line, key) != NULL) {
        sscanf(line, "%*s = \"%[^\"]\"", value);
    }
}