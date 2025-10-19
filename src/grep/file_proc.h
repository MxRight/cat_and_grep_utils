#ifndef FILE_PROC_H
#define FILE_PROC_H

#include "flags.h"

int is_match(const char *line, const char *pattern, cat_flags *flags);
void print_match(const char *text, int len);
void print_line(const char *line, const char *pattern, cat_flags *flags);
void print_only_matching(const char *line, const char *pattern,
                         cat_flags *flags);
int print_filename(cat_flags *flags, int one_or_few_files);
void grep_file(const char *filename, cat_flags *flags, int one_or_few_files,
               pattern_list *plist);

#endif