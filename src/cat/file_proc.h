#ifndef FILE_PROC_H
#define FILE_PROC_H

#define _GNU_SOURCE
#include "flags.h"
#define UTILITY_NAME "s21_cat"

int read_file(char *filename, cat_flags flags);
void update_line(const char *line, cat_flags flags);

#endif