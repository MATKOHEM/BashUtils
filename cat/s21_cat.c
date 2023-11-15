#include "s21_cat.h"

int main(int argc, char **argv) {
  int index = 1;
  parser(argc, argv, &index);
  for (int i = index; i < argc; i++) {
    print_file(argv[i]);
  }
  return 0;
}

void print_file(char *name) {
  FILE *f = fopen(name, "r");
  if (f) {
    char *line_buf = NULL;
    size_t line_buf_size = 0;
    int line_count = 1;
    ssize_t line_size = 0;
    int blank_line_count = 0;
    line_size = getline(&line_buf, &line_buf_size, f);
    while (line_size > 0) {
      if (flag_s.value && line_size == 1) {
        blank_line_count += 1;
      } else {
        blank_line_count = 0;
      }
      if (blank_line_count < 2) {
        if (flag_b.value || flag_n.value) {
          bn_flag(&line_count, line_size, flag_b.value, flag_e.value);
        }
        if (flag_e.value || flag_t.value || flag_v.value) {
          etv_flag(line_buf, line_size, flag_e.value, flag_t.value,
                   flag_v.value);
        } else {
          for (ssize_t i = 0; i < line_size; i++) {
            printf("%c", line_buf[i]);
          }
        }
      }
      line_size = getline(&line_buf, &line_buf_size, f);
    }

    free(line_buf);
    line_buf = NULL;
    fclose(f);
  } else {
    fprintf(stderr, "cat: %s No such file or directory\n", name);
  }
}

void bn_flag(int *line_count, int length, bool b, bool e) {
  if (b) {
    if (length > 1) {
      printf("%6d\t", *line_count);
      *line_count += 1;
    } else if (e) {
      printf("%6s\t", "");
    }
  } else {
    printf("%6d\t", *line_count);
    *line_count += 1;
  }
}

void etv_flag(char *line_buf, ssize_t len, bool e, bool t, bool v) {
  for (ssize_t i = 0; i < len; i++) {
    if (t && line_buf[i] == '\t') {
      printf("^");
      printf("%c", line_buf[i] + 64);
    } else if (e && line_buf[i] == '\n') {
      printf("$");
      printf("\n");
    } else if (v && line_buf[i] <= 31 && line_buf[i] != '\n' &&
               line_buf[i] != '\t') {
      printf("^");
      printf("%c", line_buf[i] + 64);
    } else if (v && line_buf[i] == 127) {
      printf("^");
      printf("%c", line_buf[i] - 64);
    } else {
      printf("%c", line_buf[i]);
    }
  }
}

void parser(int argc, char **argv, int *index) {
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      *index = i + 1;
      bool ext = false;
      for (size_t j = 1; j < strlen(argv[i]); j++) {
        switch (argv[i][j]) {
          case '-':
            if (strcmp(argv[i], "--number") == 0) {
              ext = true;
              flag_n.value = true;
              break;
            }
            if (strcmp(argv[i], "--number-nonblank") == 0) {
              ext = true;
              flag_b.value = true;
              break;
            }
            if (strcmp(argv[i], "--squeeze-blank") == 0) {
              ext = true;
              flag_s.value = true;
              break;
            }
            ext = true;
            break;

          case 'b':
            flag_b.value = true;
            break;
          case 'e':
            flag_e.value = true;
            flag_v.value = true;
            break;
          case 'v':
            flag_v.value = true;
            break;
          case 'n':
            flag_n.value = true;
            break;
          case 's':
            flag_s.value = true;
            break;
          case 't':
            flag_t.value = true;
            flag_v.value = true;
            break;
          case 'E':
            flag_e.value = true;
            break;
          case 'T':
            flag_t.value = true;
            break;
          default:
            if (ext) {
              break;
            } else {
              fprintf(stderr,
                      "cat: illegal option -- %c\nusage: cat [-benstvTE] [file "
                      "...]\n",
                      argv[i][j]);
              exit(1);
            }
        }
        if (ext) {
          break;
        }
      }

    } else {
      break;
    }
  }
}
