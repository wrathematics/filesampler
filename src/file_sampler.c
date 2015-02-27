// Copyright 2015 Drew Schmidt.  All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file.

#include "lineSampler.h"


#include <R.h>
#define RUNIF unif_rand // change this to the rng of your choice if not using R
double unif_rand();       // also remove the Get/PutRNGstate()'s'
#define PRINTFUN Rprintf


// very un-thread safe
int file_sampler(bool verbose, bool header, const double p, const char *input, const char *output)
{
  FILE *fp_read, *fp_write;
  char *buf;
  int readlen;
  // Have to track cases where buffer is too small for fgets()
  bool should_write = false;
  bool firstread = true;
  uint64_t nlines_in = 0, nlines_out = 0;
  
  if (p < 0. || p > 1.) return INVALID_PROB;
  
  fp_read = fopen(input, "r");
  if (!fp_read) return READ_FAIL;
  fp_write = fopen(output, "w");
  
  buf = malloc(BUFLEN * sizeof(char));
  
  
  if (header)
  {
    while (fgets(buf, BUFLEN, fp_read) != NULL)
    {
      fprintf(fp_write, "%s", buf);
      
      readlen = strnlen(buf, BUFLEN);
      
      if ((readlen > 0) && (buf[readlen-1] == '\n')) // has newline
      {
        nlines_in++;
        nlines_out++;
        break;
      }
    }
  }
  
  
  GetRNGstate();
  
  while (fgets(buf, BUFLEN, fp_read) != NULL)
  {
    if (firstread)
    {
      if (RUNIF() < p)
        should_write = true;
      else
        should_write = false;
    }
    
    if (should_write)
    {
      nlines_out++;
      fprintf(fp_write, "%s", buf);
    }
    
    readlen = strnlen(buf, BUFLEN);
    
    if ((readlen > 0) && (buf[readlen-1] == '\n')) // has newline
    {
      nlines_in++;
      should_write = false;
      firstread = true;
    }
    else
      firstread = false;
    
  }
  
  PutRNGstate();
  
  if (verbose)
    PRINTFUN("Read %llu lines (%.3f%%) of %llu line file.\n", nlines_out, (double) nlines_out/nlines_in, nlines_in);
  
  fclose(fp_read);
  fclose(fp_write);
  free(buf);
  
  return 0;
}

