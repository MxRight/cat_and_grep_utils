#ifndef FLAGS_H
#define FLAGS_H

typedef struct {
  int number_all;
  int number_nonblank;
  int squeeze_blank;
  int show_ends;
  int show_tabs;
  int show_nonprint;
} cat_flags;

int cat_args_reader(char argv[], cat_flags *flags);
int cat_long_args_reader(char *argv, cat_flags *flags);

#endif