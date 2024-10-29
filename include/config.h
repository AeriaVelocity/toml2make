#ifndef CONFIG_H

#define CONFIG_H

#define MAX_VALUE_LENGTH 128
#define MAX_LINE_LENGTH 1024

typedef struct {
    char schema_version[MAX_VALUE_LENGTH];

    char project_name[MAX_VALUE_LENGTH];
    char project_version[MAX_VALUE_LENGTH];
    char project_license[MAX_VALUE_LENGTH];
    char project_description[MAX_VALUE_LENGTH];
    char project_author[MAX_VALUE_LENGTH];

    char compiler_cc[MAX_VALUE_LENGTH];
    char compiler_cflags[MAX_VALUE_LENGTH];
    char compiler_cversion[MAX_VALUE_LENGTH];

    char options_lang[MAX_VALUE_LENGTH];
    int options_include;

    char paths_src[MAX_VALUE_LENGTH];
    char paths_build[MAX_VALUE_LENGTH];
    char paths_bin[MAX_VALUE_LENGTH];
    char paths_include[MAX_VALUE_LENGTH];
    char paths_install[MAX_VALUE_LENGTH];
    char paths_makefile[MAX_VALUE_LENGTH];
} ProjectConfig;

void init_default_config(ProjectConfig *config);

#endif