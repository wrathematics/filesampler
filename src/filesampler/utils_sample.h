// This file is free and unencumbered software released into the public domain.
// You may modify it for any purpose with or without attribution.
// See the Unlicense specification for full details http://unlicense.org/

// A sample, modified version of utils.h

#ifndef FILESAMPLER_UTILS_H_
#define FILESAMPLER_UTILS_H_

// ----------------------------------------------------------------------------
// RNG
// ----------------------------------------------------------------------------

#include <rand.h>

#define STARTRNG
#define ENDRNG

#define RUNIF ((double) rand() / (RAND_MAX+1))


// ----------------------------------------------------------------------------
// Printing
// ----------------------------------------------------------------------------

#define PRINTFUN printf

// ----------------------------------------------------------------------------
// Interrupt checker
// ----------------------------------------------------------------------------

static inline bool check_interrupt()
{
  return false;
}


// ----------------------------------------------------------------------------
// Error handler
// ----------------------------------------------------------------------------


#include <stdio.h>

static inline void fs_error_fun(const int err_num, const char *err_msg)
{
  fputs(stderr, err_msg);
  exit(err_num)
}


#endif
