#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "platform.h"
#include "config.h"

char* get_toml_file_path() {
    char* toml_file_path = NULL;

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

    return toml_file_path;
}

void assign_value(char *line, char *key, char *value) {
    if (strstr(line, key) != NULL) {
        sscanf(line, "%*s = \"%[^\"]\"", value);
    }
}