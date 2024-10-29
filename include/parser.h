#ifndef PARSER_H

#define PARSER_H

#include "config.h"

int parse_toml_config(char* file_path, ProjectConfig *config);
void trim_whitespace(char *str);
void assign_value(char *line, char *key, char *value);

#endif