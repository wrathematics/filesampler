#ifndef CSVSAMPLER_H_
#define CSVSAMPLER_H_


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define BUFLEN 4096

#define INVALID_PROB  -1
#define READ_FAIL     -2
#define WRITE_FAIL    -3


int wc(const char *file, uint64_t *nletters, uint64_t *nwords, uint64_t *nlines);
int file_sampler(bool header, const double p, const char *input, const char *output);


#endif
