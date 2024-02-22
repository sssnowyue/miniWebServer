#include "Errif.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

void errif(bool condition, const char *errmsg) {
  if (condition) {
    fprintf(stderr, "%s: %s\n", errmsg, strerror(errno));
    exit(EXIT_FAILURE);
  }
}