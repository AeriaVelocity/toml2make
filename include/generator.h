#ifndef GENERATOR_H

#define GENERATOR_H

#include "config.h"

void generate_makefile(const ProjectConfig *config);
void generate_cproject(const ProjectConfig *config);
int create_new_cproject();

#endif