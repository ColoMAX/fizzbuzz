#include <argp.h>
#include <stdio.h>
#include <stdlib.h>

#include "fizzbuzz.h"

static int endPoint = 100;
static int parse_opt(int key, char *arg, struct argp_state *state) {
  switch (key) {
    case 'n':
      endPoint = strtol(arg, NULL, 10);
      break;
  }
  return 0;
}

int main(int argc, char *argv[]) {
  struct argp_option options[] = {
      {"end", 'n', "NUM", 0, "element number on which to end (exclusive)"},
      {0}};
  struct argp argp = {options, parse_opt};

  argp_parse(&argp, argc, argv, 0, 0, 0);
  int vbufs = 4 * sizeof(char) * endPoint;
  vbufs = vbufs > 1024 * 32 ? 1024 * 32 : (vbufs <= 0 ? 0 : vbufs);
  setvbuf(stdout, NULL, _IOFBF, vbufs);

  struct fizzbuzz_obj fb;

  FzBz_init(&fb, NULL);

  for (size_t i = 1; i < endPoint; i++) {
    FzBz_check(&fb, i);
    FzBz_print(&fb);
  }
  FzBz_flush(&fb);
  // qputs(fb.printBuffer);
  FzBz_destroy(&fb);
}