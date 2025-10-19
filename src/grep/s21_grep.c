#include <stdio.h>
#include <string.h>

#include "file_proc.h"
#include "flags.h"

int main(int argc, char *argv[]) {
  cat_flags flags = {0};
  pattern_list plist = {.count = 0};
  int file_start_index = 0;
  if (args_e_f_reader(argc, argv, &flags, &plist, &file_start_index)) {
    free_patterns(&plist);
    return 1;
  }
  if (plist.count == 0) {
    fprintf(stderr, "s21_grep: no pattern supplied\n");
    return 1;
  }
  if (file_start_index == 0 || file_start_index >= argc) {
    fprintf(stderr, "s21_grep: no input files\n");
    return 1;
  }
  int one_or_few_files = (argc - file_start_index > 1);
  for (int i = file_start_index; i < argc; ++i)
    grep_file(argv[i], &flags, one_or_few_files, &plist);
  free_patterns(&plist);
  return 0;
}
