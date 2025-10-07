#define _GNU_SOURCE
#include "flags.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int args_e_f_reader(int argc, char *argv[], cat_flags *flags,
                    pattern_list *plist, int *file_start_index) {
  char *pattern_arg = NULL;
  int i = 1;
  while (i < argc) {
    if (strcmp(argv[i], "-e") == 0) {
      if (i + 1 >= argc) {
        fprintf(stderr, "s21_grep: option requires an argument -- e\n");
        return 1;
      }
      flags->regexp = 1;
      add_pattern(plist, argv[++i]);
      i++;
    } else if (strcmp(argv[i], "-f") == 0) {
      if (i + 1 >= argc) {
        fprintf(stderr, "s21_grep: option requires an argument -- f\n");
        return 1;
      }
      flags->file = 1;
      load_patterns_from_file(argv[++i], flags->no_messages, plist);
      i++;
    } else if (argv[i][0] == '-' && argv[i][1] != '\0') {
      if (args_reader(argv[i], flags)) return 1;
      i++;
    } else {
      if (!pattern_arg && plist->count == 0) {
        pattern_arg = argv[i];
        add_pattern(plist, pattern_arg);
        i++;
      }
      *file_start_index = i;
      break;
    }
  }

  if (plist->count == 0 && pattern_arg) {
    add_pattern(plist, pattern_arg);
  }

  if (*file_start_index == 0) *file_start_index = i;
  return 0;
}

int args_reader(const char *arg, cat_flags *flags) {
  for (size_t j = 1; arg[j]; ++j) {
    switch (arg[j]) {
      case 'i':
        flags->ignore_case = 1;
        break;
      case 'v':
        flags->invert_match = 1;
        flags->only_matching = 0;
        break;
      case 'c':
        flags->count = 1;
        break;
      case 'l':
        flags->files_with_matches = 1;
        break;
      case 'n':
        flags->line_number = 1;
        break;
      case 'h':
        flags->no_filename = 1;
        break;
      case 's':
        flags->no_messages = 1;
        break;
      case 'o':
        flags->only_matching = 1;
        flags->invert_match = 0;
        break;
      default:
        fprintf(stderr, "s21_grep: invalid option -- '%c'\n", arg[j]);
        return 1;
    }
  }
  return 0;
}

void free_patterns(pattern_list *plist) {
  for (int i = 0; i < plist->count; ++i) free(plist->arr[i]);
}

void add_pattern(pattern_list *plist, const char *pat) {
  if (plist->count >= MAX_PATTERNS) {
    fprintf(stderr, "превышено число шаблонов\n");
    exit(EXIT_FAILURE);
  }
  char *copy = strdup(pat);
  if (!copy) {
    perror("ошибка выделения памяти");
    exit(EXIT_FAILURE);
  }
  plist->arr[plist->count++] = copy;
}

void load_patterns_from_file(const char *filename, int dont_show_filename,
                             pattern_list *plist) {
  FILE *fp = fopen(filename, "r");
  if (!fp) {
    if (!dont_show_filename)
      fprintf(stderr, "s21_grep: %s: %s\n", filename, strerror(errno));
    exit(EXIT_FAILURE);
  }
  char *line = NULL;
  size_t len = 0;
  while (getline(&line, &len, fp) != -1) {
    line[strcspn(line, "\n")] = '\0';
    add_pattern(plist, line);
  }
  free(line);
  fclose(fp);
}