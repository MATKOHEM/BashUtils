#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <getopt.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void parser(int argc, char **argv, char **files, char *pattern);
int check_file(char *string);
void form_pattern(regex_t *reg, char *pattern);
void print_file(char **files, regex_t pattern);
void other_flags(char *line_buf, int line_count, int finded, int files_amount,
                 char *file_name);
void read_f(char *filename, char *pattern);
void form_string(char *string, char *new);

struct flags {
  bool value;
} flag_e = {false}, flag_i = {false}, flag_v = {false}, flag_c = {false},
  flag_l = {false}, flag_n = {false}, flag_h = {false}, flag_s = {false},
  flag_f = {false}, flag_o = {false};
