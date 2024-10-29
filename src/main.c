#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "generator.h"
#include "utils.h"

int main(int argc, char **argv) {
    char *toml_file_path = get_toml_file_path(argc, argv);
    if (!toml_file_path) {
        fprintf(stderr, "No cproject.toml found in the current working directory.\n\
If it exists somewhere else, try `%s path/to/cproject.toml`.\n", argv[0]);
        return EXIT_FAILURE;
    }

    printf("Using TOML file at %s.\n", toml_file_path);

    ProjectConfig config;

    init_default_config(&config);
    if (parse_toml_config(toml_file_path, &config) == -1) {
        fprintf(stderr, "Failed to open %s.\n", toml_file_path);
        free(toml_file_path);
        return EXIT_FAILURE;
    }

    generate_makefile(&config);

    free(toml_file_path);
    printf("Makefile generated at %s.\n", config.paths_makefile);
    return EXIT_SUCCESS;
}
