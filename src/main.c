#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define FALSE 0
#define TRUE 1

#define LATEST_SCHEMA "0.4.0"

#define MAX_LINE_LENGTH 1024
#define MAX_TOKEN_LENGTH 128
#define MAX_VALUE_LENGTH 128

void trim_whitespace(char *str);
void generate_makefile(const char *project_name, const char *project_version, const char *project_license, const char *project_description, const char *project_author,
                       const char *compiler_cc, char *compiler_cflags, const char *compiler_cversion, const char *build_install, const char *build_src, const char *build_dir,
                       const char *build_bin, const char *makefile_out);
char *get_cwd(char *buf, size_t size);
char *get_full_path(const char *path);
void assign_value(char *line, char *key, char *value);

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
    char compiler_cflags[MAX_VALUE_LENGTH] = "";
    char compiler_cversion[MAX_VALUE_LENGTH] = "c11";

    char build_src[MAX_VALUE_LENGTH] = "src";
    char build_dir[MAX_VALUE_LENGTH] = "build";
    char build_bin[MAX_VALUE_LENGTH] = "bin";
    char build_install[MAX_VALUE_LENGTH] = "/usr/local/bin";

    char makefile_out[MAX_VALUE_LENGTH] = "Makefile";

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

        if (strcmp(section, "toml2make") == 0) {
            if (strstr(line, "version =") != NULL) {
                sscanf(line, "version = \"%[^\"]\"", schema_version);
                printf("Using toml2make schema version %s.\n", schema_version);
            }
            continue;
        }

        if (strcmp(schema_version, "") == 0) {
            fprintf(stderr, "No schema version found in %s. Assuming %s.\n", toml_file_path, LATEST_SCHEMA);
            strcpy(schema_version, LATEST_SCHEMA);
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
            assign_value(line, "cflags =", compiler_cflags);
            assign_value(line, "cversion =", compiler_cversion);

            continue;
        }

        if (strcmp(section, "build") == 0) {
            assign_value(line, "install =", build_install);
            assign_value(line, "src =", build_src);
            assign_value(line, "build =", build_dir);
            assign_value(line, "bin =", build_bin);

            continue;
        }

        if (strcmp(section, "makefile") == 0) {
            assign_value(line, "out =", makefile_out);
        }
    }

    fclose(fp);

    generate_makefile(project_name, project_version, project_license, project_description, project_author,
                      compiler_cc, compiler_cflags, compiler_cversion,
                      build_install, build_src, build_dir, build_bin,
                      makefile_out);

    free(toml_file_path);

    printf("Makefile generated at %s. Run `make` to build the C project, `make clean` to clean source files or `make install` to install the binary.\n", makefile_out);

    return EXIT_SUCCESS;
}

// Fricking hell that's a lot of parameters
void generate_makefile(const char *project_name, const char *project_version, const char *project_license, const char *project_description, const char *project_author,
                       const char *compiler_cc, char *compiler_cflags, const
                       char *compiler_cversion, const char *build_install, const
                       char *build_src, const char *build_dir, const char *build_bin, const char *makefile_out)
{
    FILE *fExists = fopen(makefile_out, "r");
    if (fExists) {
        fclose(fExists);
        fprintf(stderr, "Makefile already exists at %s. Overwriting.\n", makefile_out);
    }

    FILE *fp = fopen(makefile_out, "w");
    if (!fp) {
        fprintf(stderr, "Failed to create Makefile at %s.\n", makefile_out);
        return;
    }

    time_t t = time(NULL);
    struct tm *tm = localtime(&t);

    // I can't help but feel like there's a better way to do this

    fprintf(fp, "# Makefile generated by TOML To Makefile.\n");
    fprintf(fp, "# Generated on %d-%d-%d at %d:%d:%d UTC.\n", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
    fprintf(fp, "# For more information, see <https://github.com/AeriaVelocity/toml2make>\n");
    fprintf(fp, "\n");

    fprintf(fp, "NAME = %s\n", project_name);
    fprintf(fp, "VERSION = %s\n", project_version);
    fprintf(fp, "LICENSE = %s\n", project_license);
    fprintf(fp, "DESCRIPTION = %s\n", project_description);
    fprintf(fp, "AUTHOR = %s\n", project_author);
    fprintf(fp, "\n");

    fprintf(fp, "CC = %s\n", compiler_cc);
    fprintf(fp, "CFLAGS = %s --std=%s\n", compiler_cflags, compiler_cversion);
    fprintf(fp, "CVERSION = %s\n", compiler_cversion);
    fprintf(fp, "\n");

    fprintf(fp, "SRC_DIR = %s\n", build_src);
    fprintf(fp, "BUILD_DIR = %s\n", build_dir);
    fprintf(fp, "INSTALL_DIR = %s\n", build_install);
    fprintf(fp, "BIN_DIR = %s\n", build_bin);
    fprintf(fp, "TARGET = $(BIN_DIR)/$(NAME)\n");
    fprintf(fp, "\n");

    fprintf(fp, "SRC = $(wildcard $(SRC_DIR)/*.c)\n");
    fprintf(fp, "OBJ = $(SRC:$(SRC_DIR)/%%.c=$(BUILD_DIR)/%%.o)\n");
    fprintf(fp, "\n");

    fprintf(fp, "all: $(TARGET)\n");
    fprintf(fp, "\n");

    fprintf(fp, "$(TARGET): $(OBJ)\n");
    fprintf(fp, "	@mkdir -p $(BIN_DIR)\n");
    fprintf(fp, "	$(CC) -o $@ $^\n");
    fprintf(fp, "\n");

    fprintf(fp, "$(BUILD_DIR)/%%.o: $(SRC_DIR)/%%.c\n");
    fprintf(fp, "	@mkdir -p $(BUILD_DIR)\n");
    fprintf(fp, "	$(CC) $(CFLAGS) -c $< -o $@\n");
    fprintf(fp, "\n");

    fprintf(fp, "run: all\n");
    fprintf(fp, "	./$(TARGET)\n");
    fprintf(fp, "\n");

    fprintf(fp, "install:\n");
    fprintf(fp, "	mkdir -p $(INSTALL_DIR)\n");
    fprintf(fp, "	cp $(TARGET) $(INSTALL_DIR)\n");
    fprintf(fp, "\n");

    fprintf(fp, "clean:\n");
    fprintf(fp, "	rm -rf $(BUILD_DIR) $(BIN_DIR)\n");
    fprintf(fp, "\n");

    fprintf(fp, ".PHONY: all run clean\n");

    fclose(fp);
}
