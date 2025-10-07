#ifndef FLAGS_H
#define FLAGS_H

#define MAX_PATTERNS 100

typedef struct {
  int regexp;              // -e
  int ignore_case;         // -i
  int invert_match;        // -v
  int count;               // -c
  int files_with_matches;  // -l
  int line_number;         // -n
  int no_filename;         // -h
  int no_messages;         // -s
  int file;                // -f
  int only_matching;       // -o
} cat_flags;

typedef struct {
  char *arr[MAX_PATTERNS];
  int count;
} pattern_list;

int args_reader(const char *arg, cat_flags *flags);
int args_e_f_reader(int argc, char *argv[], cat_flags *flags,
                    pattern_list *plist, int *file_start_index);
void free_patterns(pattern_list *plist);
void add_pattern(pattern_list *plist, const char *pat);
void load_patterns_from_file(const char *filename, int dont_show_filename,
                             pattern_list *plist);

#endif
