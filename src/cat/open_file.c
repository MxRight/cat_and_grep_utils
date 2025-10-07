#include <stdio.h>

FILE *open_file(char *filename, char *utility_name) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    fprintf(stderr, "%s: %s: ", utility_name, filename);
    perror("");
  }
  return fp;
}