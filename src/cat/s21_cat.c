#include <stdio.h>
#include <string.h>

#include "file_proc.h"
#include "flags.h"
#include "open_file.h"

int main(int argc, char *argv[]) {
  cat_flags flags = {0};
  int error_code = 0;
  if (argc == 1) {
    puts("s21_cat [OPTION] [FILE]...");
  } else if (argc == 2)
    error_code = read_file(argv[1], flags);
  else {
    for (int i = 1; i < argc; ++i) {
      if (argv[i][0] == '-' && argv[i][1] == '-') {
        error_code = cat_long_args_reader(argv[i], &flags);
        continue;
      } else if (argv[i][0] == '-') {
        error_code = cat_args_reader(argv[i], &flags);
      } else {
        error_code = read_file(argv[i], flags);
      }
      if (error_code == 1) break;
    }
  }

  return error_code;
}