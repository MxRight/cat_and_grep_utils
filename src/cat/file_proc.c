#include "file_proc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "open_file.h"

int read_file(char *filename, cat_flags flags) {
  char *buffer = NULL;
  size_t bufsize = 0;
  int line_len;
  int count_of_lines = 0;
  int count_of_nonblank_lines = 0;
  int blank_line_flag = 0;
  int error_code = 0;

  FILE *fp = open_file(filename, UTILITY_NAME);
  if (fp != NULL) {
    while ((line_len = getline(&buffer, &bufsize, fp)) != -1) {
      if (ferror(fp)) {
        perror("Ошибка чтения файла");
        error_code = 1;
        break;
      }

      int is_blank = (strcmp(buffer, "\n") == 0);

      if (flags.squeeze_blank) {
        if (is_blank) {
          if (blank_line_flag) {
            continue;
          }
          blank_line_flag = 1;
        } else {
          blank_line_flag = 0;
        }
      }

      if (flags.number_nonblank && !is_blank) {
        printf("%6d\t", ++count_of_nonblank_lines);
      } else if (flags.number_all) {
        printf("%6d\t", ++count_of_lines);
      }

      update_line(buffer, flags);
    }

    free(buffer);
    fclose(fp);
  }

  return error_code;
}

void update_line(const char *line, cat_flags flags) {
  for (size_t i = 0; line[i] != '\0'; ++i) {
    unsigned char c = (unsigned char)line[i];

    if (c == '\t') {
      if (flags.show_tabs) {
        printf("^I");
      } else {
        putchar('\t');
      }
    } else if (c == '\n') {
      if (flags.show_ends) {
        printf("$\n");
      } else {
        putchar('\n');
      }
    } else if (flags.show_nonprint) {
      if (c >= 128) {
        printf("M-");
        c -= 128;
      }

      if (c < 32 && c != '\n' && c != '\t') {
        printf("^%c", c + 64);
      } else if (c == 127) {
        printf("^?");
      } else {
        putchar(c);
      }
    } else {
      putchar(c);
    }
  }
}