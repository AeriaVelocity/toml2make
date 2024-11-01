#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "generator.h"

void generate_makefile(const ProjectConfig *config) {
    FILE *fExists = fopen(config->paths_makefile, "r");
    if (fExists) {
        fclose(fExists);
        fprintf(stderr, "Makefile already exists at %s. Overwriting.\n", config->paths_makefile);
    }

    FILE *fp = fopen(config->paths_makefile, "w");
    if (!fp) {
        fprintf(stderr, "Failed to create Makefile at %s.\n", config->paths_makefile);
        return;
    }

    time_t t = time(NULL);
    struct tm *tm = localtime(&t);

    fprintf(fp, "# Makefile generated by TOML To Makefile.\n");
    fprintf(fp, "# Generated on %d-%d-%d at %d:%d:%d UTC.\n", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
    fprintf(fp, "# For more information, see <https://github.com/AeriaVelocity/toml2make>\n");
    fprintf(fp, "\n");

    fprintf(fp, "NAME = %s\n", config->project_name);
    fprintf(fp, "VERSION = %s\n", config->project_version);
    fprintf(fp, "LICENSE = %s\n", config->project_license);
    fprintf(fp, "DESCRIPTION = %s\n", config->project_description);
    fprintf(fp, "AUTHOR = %s\n", config->project_author);
    fprintf(fp, "\n");

    fprintf(fp, "CC = %s\n", config->compiler_cc);
    fprintf(fp, "CFLAGS = %s --std=%s\n", config->compiler_cflags, config->compiler_cversion);
    fprintf(fp, "CVERSION = %s\n", config->compiler_cversion);
    fprintf(fp, "\n");

    fprintf(fp, "SRC_DIR = %s\n", config->paths_src);
    fprintf(fp, "BUILD_DIR = %s\n", config->paths_build);
    fprintf(fp, "INSTALL_DIR = %s\n", config->paths_install);
    fprintf(fp, "INCLUDE_DIR = %s\n", config->paths_include);
    fprintf(fp, "BIN_DIR = %s\n", config->paths_bin);
    fprintf(fp, "TARGET = $(BIN_DIR)/$(NAME)\n");
    fprintf(fp, "\n");

    fprintf(fp, "SRC = $(wildcard $(SRC_DIR)/*.%s)\n", config->options_lang);
    fprintf(fp, "OBJ = $(SRC:$(SRC_DIR)/%%.%s=$(BUILD_DIR)/%%.o)\n", config->options_lang);
    fprintf(fp, "\n");

    fprintf(fp, "all: $(TARGET)\n");
    fprintf(fp, "\n");

    char* cc = "$(CC)";

    if (config->options_include == 1) {
        cc = "$(CC) -I $(INCLUDE_DIR)";
    }

    fprintf(fp, "$(TARGET): $(OBJ)\n");
    fprintf(fp, "	@mkdir -p $(BIN_DIR)\n");
    fprintf(fp, "	%s -o $@ $^\n", cc);
    fprintf(fp, "\n");

    fprintf(fp, "$(BUILD_DIR)/%%.o: $(SRC_DIR)/%%.%s\n", config->options_lang);
    fprintf(fp, "	@mkdir -p $(BUILD_DIR)\n");
    fprintf(fp, "	%s $(CFLAGS) -c $< -o $@\n", cc);
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