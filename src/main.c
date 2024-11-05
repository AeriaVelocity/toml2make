#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "generator.h"
#include "utils.h"

int main(int argc, char **argv) {
    int run_after_compile = 0;
    if (argc > 1) {
        if (strcmp(argv[1], "--init") == 0) {
            create_new_cproject();
            return EXIT_SUCCESS;
        }
        else if (strcmp(argv[1], "--run") == 0) {
            run_after_compile = 1;
        }
    }

    char *toml_file_path = get_toml_file_path();
    if (!toml_file_path) {
        fprintf(stderr, "No cproject.toml found in the current working directory.\n\
Run `toml2make --init` to initialise a new project.\n");
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

    if (run_after_compile) {
        printf("Running the program...\n");
        int status = system("make run");
        if (status != 0) {
            fprintf(stderr, "Failed to run the program. Removing Makefile.\n");
            remove(config.paths_makefile);
            return EXIT_FAILURE;
        }
    }

    printf("Removing Makefile.\n");
    remove(config.paths_makefile);

    printf("Done.\n");
    return EXIT_SUCCESS;
}
