#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void parser(int argc, char **argv, int *index);
void print_file(char *name);
void etv_flag(char *line_buf, ssize_t len, bool e, bool t, bool v);
void bn_flag(int *line_count, int length, bool b, bool e);

struct flags {
  char *key;
  bool value;
} flag_b = {"b", false}, flag_e = {"e", false}, flag_v = {"v", false},
  flag_n = {"n", false}, flag_s = {"s", false}, flag_t = {"t", false},
  flag_E = {"E", false}, flag_T = {"T", false};
