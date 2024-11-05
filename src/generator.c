#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "generator.h"
#include "utils.h"
#include "platform.h"

#ifdef _WIN32
    #include <direct.h>
    #include <windows.h>
    #include <fileapi.h>
    int create_directory(const char *path) {
        return CreateDirectoryA(path, NULL);
    }
#else
    #include <unistd.h>
    #include <sys/stat.h>
    int create_directory(const char *path) {
        return mkdir(path, 0777);
    }
#endif

char **ask_project_details() {
    char *project_name = malloc(MAX_VALUE_LENGTH);
    char *project_license = malloc(MAX_VALUE_LENGTH);
    char *project_description = malloc(MAX_VALUE_LENGTH);
    char *project_author = malloc(MAX_VALUE_LENGTH);
    int is_cpp = -1;

    // Get project name
    printf("What's the name of your project?\n(default: my-c-project): ");
    if (!fgets(project_name, MAX_VALUE_LENGTH, stdin)) {
        fprintf(stderr, "Failed to read project name.\n");
    }
    project_name[strcspn(project_name, "\n")] = '\0';  // Remove newline
    if (strlen(project_name) == 0) {
        strncpy(project_name, "my-c-project", MAX_VALUE_LENGTH);
    }

    // Get project license
    printf("What license are you using for your project?\n(default: ISC): ");
    if (!fgets(project_license, MAX_VALUE_LENGTH, stdin)) {
        fprintf(stderr, "Failed to read project license.\n");
    }
    project_license[strcspn(project_license, "\n")] = '\0';  // Remove newline
    if (strlen(project_license) == 0) {
        strncpy(project_license, "ISC", MAX_VALUE_LENGTH);
    }

    // Get project description
    printf("Give a short description of your project.\n(default: A sample C project): ");
    if (!fgets(project_description, MAX_VALUE_LENGTH, stdin)) {
        fprintf(stderr, "Failed to read project description.\n");
    }
    project_description[strcspn(project_description, "\n")] = '\0';  // Remove newline
    if (strlen(project_description) == 0) {
        strncpy(project_description, "A sample C project", MAX_VALUE_LENGTH);
    }

    // Get project author
    printf("Who's the author of this project?\n(default: Your Name): ");
    if (!fgets(project_author, MAX_VALUE_LENGTH, stdin)) {
        fprintf(stderr, "Failed to read project author.\n");
    }
    project_author[strcspn(project_author, "\n")] = '\0';  // Remove newline
    if (strlen(project_author) == 0) {
        strncpy(project_author, "Your Name", MAX_VALUE_LENGTH);
    }

    // Get project type (C or C++)
    char temp_buffer[MAX_VALUE_LENGTH];
    while (is_cpp == -1) {
        printf("Is this a C or C++ project?\n(default: C): ");
        if (!fgets(temp_buffer, MAX_VALUE_LENGTH, stdin)) {
            fprintf(stderr, "Failed to read project type.\n");
        }
        temp_buffer[strcspn(temp_buffer, "\n")] = '\0';  // Remove newline

        if (strcasecmp(temp_buffer, "C") == 0) {
            is_cpp = 0;
        } else if (strcasecmp(temp_buffer, "C++") == 0 || strcasecmp(temp_buffer, "CPP") == 0) {
            is_cpp = 1;
        } else if (strlen(temp_buffer) == 0) {
            is_cpp = 0;  // Default to C if input is empty
        } else {
            printf("Invalid input. Please specify C or C++.\n");
        }
    }

    // Store project details
    char **details = malloc(7 * sizeof(char*));
    if (!details) {
        fprintf(stderr, "Failed to allocate memory for project details.\n");
        return NULL;       
    }

    details[0] = project_name;
    details[1] = project_license;
    details[2] = project_description;
    details[3] = project_author;
    details[4] = strdup(is_cpp == 0 ? "c" : "cpp");
    details[5] = strdup(is_cpp == 0 ? "gcc" : "g++");
    details[6] = strdup(is_cpp == 0 ? "c11" : "c++11");
    return details;
}


int create_new_cproject() {
    char *current_dir = get_cwd(NULL, 0);
    if (!current_dir) {
        fprintf(stderr, "Failed to get current directory.\n");
        return -1;
    }

    if (!directory_is_empty(current_dir)) {
        fprintf(stderr, "Will not initialise in a non-empty directory.\n");
        free(current_dir);
        return -1;
    }

    char **details = ask_project_details();
    if (!details) {
        free(current_dir);
        return -1;
    }
    for (int i = 0; i < 4; i++) {
        if (!details[i]) {
            fprintf(stderr, "Failed to read project details.\n");
            free(current_dir);
            free(details);
            return -1;
        }
    }

    char *name = details[0];
    char *license = details[1];
    char *description = details[2];
    char *author = details[3];
    char *language = details[4];
    char *compiler = details[5];
    char *cversion = details[6];
    free(details);

    ProjectConfig config;
    init_default_config(&config);

    strncpy(config.project_name, name, MAX_VALUE_LENGTH);
    strncpy(config.project_license, license, MAX_VALUE_LENGTH);
    strncpy(config.project_description, description, MAX_VALUE_LENGTH);
    strncpy(config.project_author, author, MAX_VALUE_LENGTH);

    strncpy(config.compiler_cc, compiler, MAX_VALUE_LENGTH);
    strncpy(config.compiler_cversion, cversion, MAX_VALUE_LENGTH);
    strncpy(config.options_lang, language, MAX_VALUE_LENGTH);

    generate_cproject(&config);

    printf("Created project %s.\n", name);
    printf("Run `make` to build the project.\n");
    printf("If `cproject.toml` is changed, run `toml2make` again.\n");

    free(current_dir);
    free(name);
    free(license);
    free(description);
    free(author);
    free(language);
    free(compiler);
    free(cversion);

    return 1;
}

void create_directory_structure(const ProjectConfig *config) {
    // Create .gitignore
    FILE *fGitignore = fopen(".gitignore", "w");
    if (!fGitignore) {
        fprintf(stderr, "Failed to create .gitignore.\n");
        return;
    }
    fprintf(fGitignore, "Makefile\n");
    fclose(fGitignore);
    printf("Created .gitignore.\n");

    // Create README.md
    FILE *fReadme = fopen("README.md", "w");
    if (!fReadme) {
        fprintf(stderr, "Failed to create README.md.\n");
        return;
    }
    fprintf(fReadme, "# %s\n\n", config->project_name);
    fprintf(fReadme, "%s\n\n", config->project_description);
    fprintf(fReadme, "## License\n\n%s\n", config->project_license);
    fclose(fReadme);
    printf("Created README.md.\n");

    // Create directories
    /// src
    if (create_directory("src") != 0) {
        fprintf(stderr, "Failed to create src directory.\n");
        return;
    }
    printf("Created src directory.\n");

    /// include
    if (config->options_include) {
        if (create_directory("include") != 0) {
            fprintf(stderr, "Failed to create include directory.\n");
            return;
        }
        printf("Created include directory.\n");
    }

    // Create main.c or main.cpp
    if (strcmp(config->options_lang, "c") == 0) {
        FILE *fMain = fopen("src/main.c", "w");
        if (!fMain) {
            fprintf(stderr, "Failed to create main.c.\n");
            return;
        }
        fprintf(fMain, "#include <stdio.h>\n\n");
        fprintf(fMain, "int main(int argc, char **argv) {\n");
        fprintf(fMain, "    printf(\"Hello, World!\\n\");\n");
        fprintf(fMain, "    return 0;\n");
        fprintf(fMain, "}\n");
        fclose(fMain);
        printf("Created main.c.\n");
    }
    else if (strcmp(config->options_lang, "cpp") == 0) {
        FILE *fMain = fopen("src/main.cpp", "w");
        if (!fMain) {
            fprintf(stderr, "Failed to create main.cpp.\n");
            return;
        }
        fprintf(fMain, "#include <iostream>\n\n");
        fprintf(fMain, "int main(int argc, char **argv) {\n");
        fprintf(fMain, "    std::cout << \"Hello, World!\\n\";\n");
        fprintf(fMain, "    return 0;\n");
        fprintf(fMain, "}\n");
        fclose(fMain);
        printf("Created main.cpp.\n");
    }
    else {
        fprintf(stderr, "Invalid language '%s'.\n", config->options_lang);
        fprintf(stderr, "You should never see this message.\n");
        return;
    }

    // Create Makefile
    generate_makefile(config);
    printf("Generated Makefile.\n");
}

void generate_cproject(const ProjectConfig *config) {
    FILE *fP = fopen("cproject.toml", "w");
    if (!fP) {
        fprintf(stderr, "Failed to create cproject.toml.\n");
        return;
    }

    fprintf(fP, "# This cproject.toml file was automatically generated by TOML To Makefile.\n");
    fprintf(fP, "\n");
    fprintf(fP, "[toml2make.schema]\n");
    fprintf(fP, "version = \"%s\"\n", config->schema_version);
    fprintf(fP, "\n");
    fprintf(fP, "[project]\n");
    fprintf(fP, "name = \"%s\"\n", config->project_name);
    fprintf(fP, "version = \"%s\"\n", config->project_version);
    fprintf(fP, "license = \"%s\"\n", config->project_license);
    fprintf(fP, "description = \"%s\"\n", config->project_description);
    fprintf(fP, "author = \"%s\"\n", config->project_author);
    fprintf(fP, "\n");
    fprintf(fP, "[compiler]\n");
    fprintf(fP, "cc = \"%s\"\n", config->compiler_cc);
    fprintf(fP, "cflags = \"%s\"\n", config->compiler_cflags);
    fprintf(fP, "cversion = \"%s\"\n", config->compiler_cversion);
    fprintf(fP, "\n");
    fprintf(fP, "[options]\n");
    fprintf(fP, "file_extension = \"%s\"\n", config->options_lang);
    fprintf(fP, "use_include = \"%s\"\n", config->options_include ? "true" : "false");
    fprintf(fP, "\n");
    fprintf(fP, "[paths]\n");
    fprintf(fP, "build = \"%s\"\n", config->paths_build);
    fprintf(fP, "src = \"%s\"\n", config->paths_src);
    fprintf(fP, "bin = \"%s\"\n", config->paths_bin);
    fprintf(fP, "include = \"%s\"\n", config->paths_include);
    fprintf(fP, "install = \"%s\"\n", config->paths_install);
    fprintf(fP, "makefile = \"%s\"\n", config->paths_makefile);

    fclose(fP);

    printf("cproject.toml generated at %s.\n", config->paths_src);
    printf("Creating project structure.\n");

    char *cwd = get_cwd(NULL, 0);
    if (cwd) {
        create_directory_structure(config);
        free(cwd);
    }
}

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