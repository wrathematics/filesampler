#ifndef LINESAMPLER_H_
#define LINESAMPLER_H_


#include <stdbool.h>
#include <stdint.h>

#define BUFLEN 8192

#define INVALID_PROB    -1
#define INVALID_NSKIP   -2
#define READ_FAIL       -3
#define WRITE_FAIL      -4
#define MALLOC_FAIL     -5
#define USER_INTERRUPT  -6


// file_sampler.c
int LS_sample_prob(const bool verbose, const bool header, uint32_t nskip, uint32_t nmax, const double p, const char *input, const char *output);
int LS_sample_exact(const bool header, uint64_t nlines_in, uint64_t nlines_out, const uint32_t nskip, const char *input, const char *output);

// wc.c
int LS_wc(const char *file, const bool chars, uint64_t *nchars, const bool words, uint64_t *nwords, const bool lines, uint64_t *nlines);

#endif
