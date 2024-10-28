#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define FALSE 0
#define TRUE 1

#define MAX_LINE_LENGTH 1024
#define MAX_TOKEN_LENGTH 128
#define MAX_VALUE_LENGTH 128

void trim_whitespace(char *str);
void generate_makefile(const char *project_name, const char *c_version);
char *get_cwd(char *buf, size_t size);
char *get_full_path(const char *path);
void assign_value(char *line, char *key, char *value);
void assign_values(char *line, char *key, char **values, int *num_values);

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

void assign_value(char *line, char *key, char *value) {
    if (strstr(line, key) != NULL) {
        sscanf(line, "%*s = \"%[^\"]\"", value);
    }
}

void assign_values(char *line, char *key, char **values, int *num_values) {
    if (strstr(line, key) != NULL) {
        char *token = strtok(line, "\"");
        while (token != NULL) {
            if (*num_values < MAX_VALUE_LENGTH) {
                values[*num_values] = strdup(token);
                (*num_values) += 1;
            }
            token = strtok(NULL, "\"");
        }
    }   
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

    char line[MAX_LINE_LENGTH];

    char section[MAX_VALUE_LENGTH];

    char schema_version[MAX_VALUE_LENGTH] = "";

    char project_name[MAX_VALUE_LENGTH] = "";
    char project_version[MAX_VALUE_LENGTH] = "";
    char project_license[MAX_VALUE_LENGTH] = "";
    char project_description[MAX_VALUE_LENGTH] = "";
    char project_author[MAX_VALUE_LENGTH] = "";

    char compiler_cc[MAX_VALUE_LENGTH] = "gcc";
    int num_cflags = 0;
    char *compiler_cflags[MAX_VALUE_LENGTH] = {};
    char compiler_cversion[MAX_VALUE_LENGTH] = "c11";

    char build_prefix[MAX_VALUE_LENGTH] = "/usr/local";
    char build_src[MAX_VALUE_LENGTH] = "src";
    char build_dir[MAX_VALUE_LENGTH] = "build";
    char build_bin[MAX_VALUE_LENGTH] = "bin";

    char makefile_out[MAX_VALUE_LENGTH] = "Makefile";
    char makefile_log[MAX_VALUE_LENGTH] = "toml2make.log";

    FILE *fp = fopen(toml_file_path, "r");
    if (!fp) {
        fprintf(stderr, "Failed to open %s.\n", toml_file_path);
        free(toml_file_path);
        return EXIT_FAILURE;
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

        if (strcmp(section, "schema") == 0) {
            if (strstr(line, "version =") != NULL) {
                sscanf(line, "version = \"%[^\"]\"", schema_version);
                printf("Using schema version %s.\n", schema_version);
                continue;
            }
            if (strcmp(schema_version, "") == 0) {
                fprintf(stderr, "No schema version found in %s.\n", toml_file_path);
            }
            continue;
        }

        if (strcmp(section, "project") == 0) {
            assign_value(line, "name =", project_name);
            assign_value(line, "version =", project_version);
            assign_value(line, "license =", project_license);
            assign_value(line, "description =", project_description);
            assign_value(line, "author =", project_author);

            continue;
        }

        if (strcmp(section, "compiler") == 0) {
            assign_value(line, "cc =", compiler_cc);
            assign_values(line, "cflags =", compiler_cflags, &num_cflags);
            assign_value(line, "cversion =", compiler_cversion);

            continue;
        }

        if (strcmp(section, "build") == 0) {
            assign_value(line, "prefix =", build_prefix);
            assign_value(line, "src =", build_src);
            assign_value(line, "build =", build_dir);
            assign_value(line, "bin =", build_bin);

            continue;
        }

        if (strcmp(section, "makefile") == 0) {
            assign_value(line, "out =", makefile_out);
            assign_value(line, "log =", makefile_log);
        }
    }

    fclose(fp);
    free(toml_file_path);

    return EXIT_SUCCESS;
}
