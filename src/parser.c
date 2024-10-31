#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"

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

int parse_toml_config(char* file_path, ProjectConfig *config) {
    char line[MAX_LINE_LENGTH];
    char section[MAX_VALUE_LENGTH];

    FILE *fp = fopen(file_path, "r");
    if (!fp) {
        return -1;
    }

    while (fgets(line, MAX_LINE_LENGTH, fp)) {
        trim_whitespace(line);

        if (line[0] == '#') {
            continue;
        }

        if (strstr(line, "[") != NULL) {
            sscanf(line, "[%[^]]", section);
            continue;
        }

        if (strcmp(section, "cproject.toml") == 0) {
            if (strstr(line, "version =") != NULL) {
                sscanf(line, "version = \"%[^\"]\"", config->schema_version);
                printf("Using cproject schema version %s.\n", config->schema_version);
            }
            continue;
        }

        if (strcmp(section, "project") == 0) {
            assign_value(line, "name =", config->project_name);
            assign_value(line, "version =", config->project_version);
            assign_value(line, "license =", config->project_license);
            assign_value(line, "description =", config->project_description);
            assign_value(line, "author =", config->project_author);

            continue;
        }

        if (strcmp(section, "compiler") == 0) {
            assign_value(line, "cc =", config->compiler_cc);
            assign_value(line, "cflags =", config->compiler_cflags);
            assign_value(line, "cversion =", config->compiler_cversion);

            continue;
        }

        if (strcmp(section, "options") == 0) {
            assign_value(line, "file_extension =", config->options_lang);

            if (strstr(line, "use_include =") != NULL) {
                if (strstr(line, "false") != NULL) {
                    config->options_include = 0;
                }
                else {
                    config->options_include = 1;
                }
            }
        }

        if (strcmp(section, "paths") == 0) {
            assign_value(line, "install =", config->paths_install);
            assign_value(line, "src =", config->paths_src);
            assign_value(line, "build =", config->paths_build);
            assign_value(line, "include =", config->paths_include);
            assign_value(line, "bin =", config->paths_bin);
            assign_value(line, "makefile =", config->paths_makefile);

            continue;
        }
    }

    fclose(fp);
    return 0;
}
