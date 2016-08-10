// This file is free and unencumbered software released into the public domain.
// You may modify it for any purpose with or without attribution.
// See the Unlicense specification for full details http://unlicense.org/

// To make the lineSampler library useable outside of R, you need to modify
// every definition in this file appropriately, and delete the R header files.
// Comments in this file are to help someone porting this to another (non-R)
// system.

// See utils_sample.h for an example.


#ifndef LINESAMPLER_UTILS_H_
#define LINESAMPLER_UTILS_H_

// ----------------------------------------------------------------------------
// RNG
// ----------------------------------------------------------------------------

#include <R.h>      // delete me
#include <Rmath.h>  // delete me

// rng initializer and ender; probably sufficient to set the second def to a blank
#define STARTRNG GetRNGstate()
#define ENDRNG PutRNGstate()

// uniform random number generator
#define RUNIF unif_rand


// ----------------------------------------------------------------------------
// Printing
// ----------------------------------------------------------------------------

// replace with regular printf() (probably)
#define PRINTFUN Rprintf

// ----------------------------------------------------------------------------
// Interrupt checker
// ----------------------------------------------------------------------------

// Write an appropriate check_interrupt() function (or just set to always return
// false if you're lazy).

#include <Rinternals.h>   // delete me
#include <R_ext/Utils.h>  // delete me

#include <stdbool.h>

static void check_interrupt_fun(void *ignored)
{
  R_CheckUserInterrupt();
}

// returns `true` if user interrupts with C-c, and `false` if not
static inline bool check_interrupt()
{
  return (R_ToplevelExec(check_interrupt_fun, NULL) == FALSE);
}


// ----------------------------------------------------------------------------
// Error handler
// ----------------------------------------------------------------------------

// Print to stderr and exit
static inline void LS_error_fun(const int err_num, const char *err_msg)
{
  error(err_msg);
}


#endif
