#ifndef LINESAMPLER_H_
#define LINESAMPLER_H_


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define BUFLEN 4096

#define INVALID_PROB  -1
#define INVALID_NSKIP -2
#define READ_FAIL     -3
#define WRITE_FAIL    -4


// file_sampler.c
int file_sampler(bool verbose, bool header, uint32_t nskip, uint32_t nmax, const double p, const char *input, const char *output);
int file_sampler_exact(bool header, uint64_t nlines_in, uint64_t nlines_out, const uint32_t nskip, const char *input, const char *output);

// wc.c
int file_sampler_wc(const char *file, uint64_t *nchars, uint64_t *nwords, uint64_t *nlines);

#endif
