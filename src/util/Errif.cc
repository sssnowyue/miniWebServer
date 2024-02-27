#include "Errif.h"
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void errif(bool condition, const char* errmsg) {
  if (condition) {
    fprintf(stderr, "%s: %s\n", errmsg, strerror(errno));
    exit(EXIT_FAILURE);
  }
}