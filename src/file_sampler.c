// Copyright 2015 Drew Schmidt.  All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file.

#include "lineSampler.h"


#include <R.h>
#define RUNIF unif_rand // change this to the rng of your choice if not using R
double unif_rand();       // also remove the Get/PutRNGstate()'s'
#define PRINTFUN Rprintf



#define HAS_NEWLINE ((readlen > 0) && (buf[readlen-1] == '\n'))


static inline void read_header(char *buf, FILE *fp_read, FILE *fp_write, uint64_t *nlines_in, uint64_t *nlines_out)
{
  int readlen;
  
  while (fgets(buf, BUFLEN, fp_read) != NULL)
  {
    fprintf(fp_write, "%s", buf);
    
    readlen = strnlen(buf, BUFLEN);
    
    if (HAS_NEWLINE)
    {
      (*nlines_in)++;
      (*nlines_out)++;
      break;
    }
  }
}


// very un-thread safe
int file_sampler(bool verbose, bool header, int nskip, const double p, const char *input, const char *output)
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
    read_header(buf, fp_read, fp_write, &nlines_in, &nlines_out);
  
  if (p == 0.)
  {
    if (verbose)
      PRINTFUN("Read 0 lines (p == 0) of unknown length file.\n");
    
    goto cleanup;
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
    
    if (nskip)
      nskip--;
    else if (should_write)
    {
      nlines_out++;
      fprintf(fp_write, "%s", buf);
    }
    
    readlen = strnlen(buf, BUFLEN);
    
    if (HAS_NEWLINE)
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
  
  cleanup:
    fclose(fp_read);
    fclose(fp_write);
    free(buf);
  
  return 0;
}




static inline int unif_rand_int(const int low, const int high)
{
  return (int) low + (high + 1 - low)*unif_rand() ;
}


static int res_sampler(const int nlines_in, const int nlines_out, uint64_t **samp)
{
  int i, j;
  *samp = malloc(nlines_out * sizeof(**samp));
  
  for (i=0; i<nlines_out; i++)
    (*samp)[i] = i+1;
  
  GetRNGstate();
  
  for (i=0; i<nlines_out; i++)
  {
    j = unif_rand_int(1, i);
    if (j <= nlines_out)
      (*samp)[j] = i+1;
  }
  
  PutRNGstate();
  
  return 0;
}


