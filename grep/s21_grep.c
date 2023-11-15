#include "s21_grep.h"

int main(int argc, char **argv) {
  int err = 0;
  regex_t reegex;
  char **files = (char **)malloc(argc * sizeof(char *));
  if (NULL != files) {
    for (int i = 0; i < argc; i++) {
      files[i] = calloc(1024, sizeof(char));
      if (NULL == files[i]) {
        err = 1;
        for (int j = 0; j <= i; j++) {
          if (files[j]) free(files[j]);
        }
        if (files) free(files);
        break;
      }
    }
  }
  if (err == 0) {
    char *string_arr = (char *)calloc(1024, sizeof(char));
    if (NULL == string_arr) {
      err = 1;
    } else {
      parser(argc, argv, files, string_arr);
      form_pattern(&reegex, string_arr);
      print_file(files, reegex);
      free(string_arr);
      for (int i = 0; i < argc; i++) {
        if (files[i]) free(files[i]);
      }
      if (files) free(files);
      regfree(&reegex);
    }
  } else {
    for (int i = 0; i < argc; i++) {
      if (files[i]) free(files[i]);
    }
    if (files) free(files);
  }
  return err;
}

void read_f(char *filename, char *pattern) {
  if (filename != NULL && pattern != NULL) {
    FILE *f = fopen(filename, "r");
    if (f != NULL) {
      char *line_buf = NULL;  // строка из файла
      size_t line_buf_size = 0;
      ssize_t line_size = getline(&line_buf, &line_buf_size, f);
      while (line_size > 0) {
        size_t ln = strlen(line_buf) - 1;
        if (line_buf[0] != '\n' && (*line_buf && line_buf[ln] == '\n'))
          line_buf[ln] = '\0';
        form_string(pattern, line_buf);
        line_size = getline(&line_buf, &line_buf_size, f);
      }
      if (line_buf) free(line_buf);
      if (f) fclose(f);
    }
  }
}

void print_file(char **files, regex_t pattern) {
  if (files != NULL) {
    int files_amount = 0;
    for (size_t i = 0; files[i][0] > 0; i++) {
      files_amount += 1;
    }
    for (size_t i = 0; files[i][0] > 0; i++) {
      FILE *f = fopen(files[i], "r");
      int coincidence = 0;  // если ли запрос в этом файле
      int line_count = 0;   // кол-во строк
      char *line_buf = NULL;  // строка из файла
      size_t line_buf_size = 0;
      int value;
      int eflag;
      while (true) {
        ssize_t line_size = getline(&line_buf, &line_buf_size, f);
        if (line_size < 1) {
          break;
        }
        line_count++;
        if (flag_o.value == true) {
          char *ps = line_buf;
          regmatch_t pmatch[100];
          eflag = 0;
          char res[100] = {'\0'};
          while (true) {
            value = regexec(&pattern, ps, 1, pmatch, eflag);
            if (value == 0) {
              int start = pmatch[0].rm_so;
              int finish = pmatch[0].rm_eo;
              sprintf(res, "%.*s\n", (finish - start), ps + start);
              if ((flag_v.value == false) || (flag_v.value == true)) {
                coincidence++;
              }
              if (flag_c.value == true || flag_l.value == true) {
                break;
              } else {
                other_flags(res, line_count, 0, files_amount, files[i]);
              }
              ps += pmatch[0].rm_eo;
              eflag = REG_NOTBOL;
            } else {
              break;
            }
          }
          continue;
        }
        value = regexec(&pattern, line_buf, 0, NULL, 0);

        if ((value == 0 && flag_v.value == false) ||
            (value != 0 && flag_v.value == true)) {
          coincidence++;
        }
        if (flag_c.value == true || flag_l.value == true) {
          continue;
        } else {
          other_flags(line_buf, line_count, value, files_amount, files[i]);
        }
      }
      if (flag_c.value == true) {
        if (files_amount > 1 && flag_h.value == false) {
          printf("%s:", files[i]);
        }
        if (flag_l.value == true && coincidence > 0) {
          printf("1\n");
        } else {
          printf("%d\n", coincidence);
        }
      }
      if (flag_l.value == true && coincidence > 0) {
        printf("%s\n", files[i]);
      }
      free(line_buf);
      if (f) fclose(f);
    }
  }
}

void other_flags(char *line_buf, int line_count, int finded, int files_amount,
                 char *file_name) {
  if (line_buf != NULL && file_name != NULL) {
    if (finded == 0) {
      if (files_amount > 1 && flag_h.value == false && flag_v.value == false) {
        printf("%s:", file_name);
      }
      if (flag_n.value == true && flag_v.value == false) {
        printf("%d:", line_count);
      }
      if (flag_v.value == false) {
        printf("%s", line_buf);
        if (strstr(line_buf, "\n") == NULL) {
          printf("\n");
        }
      }
    } else {
      if (files_amount > 1 && flag_h.value == false && flag_v.value == true) {
        printf("%s:", file_name);
      }
      if (flag_n.value == true && flag_v.value == true) {
        printf("%d:", line_count);
      }
      if (flag_v.value == true) {
        printf("%s", line_buf);
        if (strstr(line_buf, "\n") == NULL) {
          printf("\n");
        }
      }
    }
  }
}

void form_string(char *string, char *new) {
  if (string != NULL && new != NULL) {
    if (strlen(string) > 0) {
      strcat(string, "|");
    }
    strcat(string, new);
  }
}

void form_pattern(regex_t *reg, char *pattern) {
  if (reg != NULL && pattern != NULL) {
    int check = 0;
    int regflag = REG_EXTENDED;
    regflag |= REG_NEWLINE;
    if (flag_i.value == true) {
      regflag |= REG_ICASE;
    }
    check = regcomp(reg, pattern, regflag);
    if (check != 0) {
      exit(1);
    }
  }
}

int check_file(char *string) {
  if (NULL != string) {
    int check = 0;
    FILE *f = fopen(string, "r");
    check = f ? 1 : 0;
    if (f) fclose(f);
    return check;
  } else {
    exit(1);
  }
}

void parser(int argc, char **argv, char **files, char *pattern) {
  if (argv != NULL && pattern != NULL && files != NULL) {
    int opt, count = 0;
    int option_index = 0;
    static struct option long_options[] = {{0, 0, 0, 0}};
    while ((opt = getopt_long(argc, argv, "f:e:ivclnhso", long_options,
                              &option_index)) != -1) {
      switch (opt) {
        case 'e':
          flag_e.value = true;
          form_string(pattern, optarg);
          break;
        case 'i':
          flag_i.value = true;
          break;
        case 'v':
          flag_v.value = true;
          break;
        case 'c':
          flag_c.value = true;
          break;
        case 'l':
          flag_l.value = true;
          break;
        case 'n':
          flag_n.value = true;
          break;
        case 'h':
          flag_h.value = true;
          break;
        case 's':
          flag_s.value = true;
          break;
        case 'f':
          flag_f.value = true;
          read_f(optarg, pattern);
          break;
        case 'o':
          flag_o.value = true;
          break;
        default:
          if (flag_s.value == false) {
            fprintf(
                stderr,
                "usage: grep [-abcdDEFGHhIiJLlMmnOopqRSsUVvwXxZz] [-A num] [-B "
                "num] [-C[num]]\n\t[-e pattern] [-f file] "
                "[--binary-files=value] "
                "[--color=when]\n\t[--context[=num]] [--directories=action] "
                "[--label] [--line-buffered]\n\t[--null] [pattern] [file ...]");
          }
      }
    }
    if (flag_v.value == true) {
      flag_o.value = false;
    }
    for (; optind < argc; optind++) {
      int check = check_file(argv[optind]);
      if (strlen(pattern) == 0 && check == 0) {
        form_string(pattern, argv[optind]);
      } else if (check == 1) {
        if (NULL != files[count]) {
          strcat(files[count], argv[optind]);
          count += 1;
        } else {
          exit(1);
        }
      } else {
        if (flag_s.value == false) {
          fprintf(stderr, "grep: %s: No such file or directory\n",
                  argv[optind]);
        }
      }
    }
  }
}
