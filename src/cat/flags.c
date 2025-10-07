#include "flags.h"

#include <stdio.h>
#include <string.h>

int cat_args_reader(char argv[], cat_flags *flags) {
  for (int i = 1; argv[i] != '\0'; ++i) {
    switch (argv[i]) {
      case 'n':
        if (!flags->number_nonblank) flags->number_all = 1;
        break;
      case 'e':
        flags->show_ends = 1;
        flags->show_nonprint = 1;
        break;
      case 'E':
        flags->show_ends = 1;
        break;
      case 's':
        flags->squeeze_blank = 1;
        break;
      case 't':
        flags->show_tabs = 1;
        flags->show_nonprint = 1;
        break;
      case 'T':
        flags->show_tabs = 1;
        break;
      case 'b':
        flags->number_nonblank = 1;
        flags->number_all = 0;
        break;
      case 'v':
        flags->show_nonprint = 1;
        break;
      default:
        fprintf(stderr, "s21_cat: illegal option -- %c\n", argv[i]);
        fprintf(stderr, "usage: s21_cat [-benstv] [file ...]\n");
        return 1;
    }
  }

  return 0;
}

int cat_long_args_reader(char *argv, cat_flags *flags) {
  if (strcmp(argv, "--number") == 0) {
    if (!flags->number_nonblank) flags->number_all = 1;
  } else if (strcmp(argv, "--number-nonblank") == 0) {
    flags->number_nonblank = 1;
    flags->number_all = 0;
  } else if (strcmp(argv, "--squeeze-blank") == 0) {
    flags->squeeze_blank = 1;
  } else {
    fprintf(stderr, "s21_cat: illegal option -- %s\n", argv);
    fprintf(stderr, "usage: s21_cat [-benstv] [file ...]\n");
    return 1;
  }

  return 0;
}
