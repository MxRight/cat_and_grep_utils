#define _GNU_SOURCE
#include "file_proc.h"

#include <errno.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int is_match(const char *line, const char *pattern, cat_flags *flags) {
  if (pattern[0] == '\0') return 1;
  if (!flags->regexp) {
    return flags->ignore_case ? (strcasestr(line, pattern) != NULL)
                              : (strstr(line, pattern) != NULL);
  }

  regex_t regex;
  int cflags = REG_EXTENDED | REG_NEWLINE;
  if (flags->ignore_case) cflags |= REG_ICASE;

  if (regcomp(&regex, pattern, cflags) != 0) return 0;

  int res = regexec(&regex, line, 0, NULL, 0);
  regfree(&regex);
  return res == 0;
}

void print_match(const char *text, int len) { printf("%.*s", len, text); }

void print_line(const char *line, const char *pattern, cat_flags *flags) {
  if (pattern[0] == '\0') {
    printf("%s\n", line);
    return;
  }

  const char *p = line;
  if (flags->regexp) {
    regex_t regex;
    regmatch_t match;
    int cflags = REG_EXTENDED;
    if (flags->ignore_case) cflags |= REG_ICASE;
    if (regcomp(&regex, pattern, cflags) != 0) {
      printf("%s\n", line);
      regfree(&regex);
      return;
    }
    while (regexec(&regex, p, 1, &match, 0) == 0) {
      fwrite(p, 1, match.rm_so, stdout);
      print_match(p + match.rm_so, match.rm_eo - match.rm_so);
      p += match.rm_eo;
    }
    printf("%s\n", p);
    regfree(&regex);
  } else {
    size_t len = strlen(pattern);
    const char *match;
    while ((match = flags->ignore_case ? strcasestr(p, pattern)
                                       : strstr(p, pattern))) {
      fwrite(p, 1, match - p, stdout);
      print_match(match, len);
      p = match + (len > 0 ? len : 1);
    }
    printf("%s\n", p);
  }
}

void print_only_matching(const char *line, const char *pattern,
                         cat_flags *flags) {
  if (pattern[0] == '\0') {
    printf("%s\n", line);
    return;
  }

  if (flags->regexp) {
    regex_t regex;
    regmatch_t match;
    int cflags = REG_EXTENDED;
    if (flags->ignore_case) cflags |= REG_ICASE;
    if (regcomp(&regex, pattern, cflags) != 0) return;
    const char *p = line;
    while (regexec(&regex, p, 1, &match, 0) == 0) {
      print_match(p + match.rm_so, match.rm_eo - match.rm_so);
      putchar('\n');
      p += match.rm_eo;
    }
    regfree(&regex);
  } else {
    const char *found =
        flags->ignore_case ? strcasestr(line, pattern) : strstr(line, pattern);
    if (found) {
      print_match(found, strlen(pattern));
      putchar('\n');
    }
  }
}

int print_filename(cat_flags *flags, int multi_file) {
  return multi_file && !flags->no_filename && !flags->files_with_matches;
}

void grep_file(const char *filename, cat_flags *flags, int multi_file,
               pattern_list *plist) {
  FILE *fp = fopen(filename, "r");
  if (!fp) {
    if (!flags->no_messages)
      fprintf(stderr, "s21_grep: %s: %s\n", filename, strerror(errno));
    return;
  }
  char *line = NULL;
  size_t len = 0;
  int line_number = 0, match_count = 0;
  while (getline(&line, &len, fp) != -1) {
    if (ferror(fp)) {
      perror("getline");
      break;
    }
    line_number++;
    line[strcspn(line, "\n")] = '\0';

    int matched = 0;
    int matched_index = -1;
    for (int i = 0; i < plist->count; ++i) {
      if (is_match(line, plist->arr[i], flags)) {
        matched = 1;
        if (matched_index == -1) matched_index = i;
        break;
      }
    }

    int should_print = (flags->invert_match ? !matched : matched);
    if (should_print) {
      match_count++;
      if (flags->files_with_matches) {
        printf("%s\n", filename);
        break;
      }
      if (!flags->count) {
        if (print_filename(flags, multi_file)) printf("%s:", filename);
        if (flags->line_number) printf("%d:", line_number);
        if (flags->only_matching && !flags->invert_match) {
          for (int i = 0; i < plist->count; i++)
            if (is_match(line, plist->arr[i], flags))
              print_only_matching(line, plist->arr[i], flags);
        } else if (!flags->only_matching && !flags->invert_match &&
                   matched_index != -1) {
          print_line(line, plist->arr[matched_index], flags);
        } else if (flags->invert_match) {
          printf("%s\n", line);
        }
      }
    }
  }
  if (flags->count) {
    if (print_filename(flags, multi_file)) printf("%s:", filename);
    printf("%d\n", match_count);
  }
  free(line);
  fclose(fp);
}