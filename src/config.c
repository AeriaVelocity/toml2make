#include <string.h>
#include "config.h"

void init_default_config(ProjectConfig *config) {
    strncpy(config->schema_version, "0.5.1", MAX_VALUE_LENGTH);

    strncpy(config->project_name, "my-c-project", MAX_VALUE_LENGTH);
    strncpy(config->project_version, "0.1.0", MAX_VALUE_LENGTH);
    strncpy(config->project_license, "ISC", MAX_VALUE_LENGTH);
    strncpy(config->project_description, "A sample C project", MAX_VALUE_LENGTH);
    strncpy(config->project_author, "Your Name", MAX_VALUE_LENGTH);

    strncpy(config->compiler_cc, "gcc", MAX_VALUE_LENGTH);
    strncpy(config->compiler_cflags, "", MAX_VALUE_LENGTH);
    strncpy(config->compiler_cversion, "c11", MAX_VALUE_LENGTH);

    strncpy(config->options_lang, "c", MAX_VALUE_LENGTH);
    config->options_include = 1;

    strncpy(config->paths_src, "src", MAX_VALUE_LENGTH);
    strncpy(config->paths_build, "build", MAX_VALUE_LENGTH);
    strncpy(config->paths_bin, "bin", MAX_VALUE_LENGTH);
    strncpy(config->paths_include, "include", MAX_VALUE_LENGTH);
    strncpy(config->paths_install, "/usr/local/bin", MAX_VALUE_LENGTH);
    strncpy(config->paths_makefile, "Makefile", MAX_VALUE_LENGTH);
}