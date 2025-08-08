#ifndef HELPER_H
#define HELPER_H

#include <stdio.h>
#include <stdlib.h>

static int read_line(char *buf, size_t n);
static int read_int(const char *prompt, int *out);
static int read_float(const char *prompt, float *out);
int fpeek(FILE *f);
#endif