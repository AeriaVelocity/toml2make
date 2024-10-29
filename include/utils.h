#ifndef UTILS_H

#define UTILS_H

#include "config.h"

char *get_toml_file_path(int argc, char **argv);
void assign_value(char *line, char *key, char *value);
int parse_toml_config(char* file_path, ProjectConfig *config);

#endif