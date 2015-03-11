// Copyright 2015 Drew Schmidt.  All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file.

#include "lineSampler.h"

#define HAS_NEWLINE ((readlen > 0) && (buf[readlen-1] == '\n'))


/* To make this useable outside of R, you need to:
 *    1. remove the Get/PutRNGstate() calls
 *    2. change RUNIF def to random uniform(0,1) rng of your choice.
 *    3. set PRINTFUN to (probably) printf().
 */
#include <R.h>
#include <Rmath.h>
#define RUNIF unif_rand
#define PRINTFUN Rprintf



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



/**
 * @file
 * @brief 
 * File Sampler
 *
 * @details
 * This function takes an input file and randomly subsamples it at
 * the given proportion p line-by-line, with the randomly chosen lines
 * being placed in the given output file.  
 * 
 * If the file has many lines, then the size of the output file
 * should be roughly p*nlines(input).  If you want to specify
 * an exact number of lines, see file_sampler_exact().
 *
 * @param verbose
 * Input.  Indicates whether character/word/line counts of the input
 * file (discovered while filling the output) should be printed.
 * @param header
 * Input.  Indicates whether or not there is a header line (as in a
 * csv).
 * @param nskip
 * Input.  Number of lines to skip.  If header=true and nskip>0, then
 * the number of lines skipped applies to post-header lines only.
 * @param p
 * Input.  Proportion of lines from input file to (randomly) retain.
 * The proportion retained is not guaranteed to be exactly p, but 
 * will be very close for large files.
 * @param input
 * Input.  Absolute path to input file.
 * @param output
 * Input.  Absolute path to output file.
 *
 * @note
 * Due to R's RNG, this call (as written) is very un-threadsafe.
 * 
 * @return
 * The return value indicates the status of the function.
 */
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



// ------------------------------------------------------
// exact reader


static inline int unif_rand_int(const int low, const int high)
{
  return (int) low + (high + 1 - low)*RUNIF() ;
}



static int res_sampler(const int nskip, const uint64_t nlines_in, const uint64_t nlines_out, uint64_t **samp)
{
  int i, j;
  *samp = malloc(nlines_out * sizeof(**samp));
  
  for (i=0; i<nlines_out; i++)
    (*samp)[i] = nskip + i+1;
  
  GetRNGstate();
  
  for (i=nlines_out; i<nlines_in; i++)
  {
    j = unif_rand_int(0, i-1);
    if (j < nlines_out)
      (*samp)[j] = nskip + i+1;
  }
  
  PutRNGstate();
  
  return 0;
}



static int comp(const void *a, const void *b)
{
   return *(uint64_t*)a - *(uint64_t*)b;
}



/**
 * @file
 * @brief 
 * File Sampler (Exact)
 *
 * @details
 * This function takes an input file and randomly subsamples
 * line-by-line producing exactly n lines in the subsample, with 
 * the randomly chosen lines being placed in the given output file.  
 * 
 * If the file has many lines, it's probably just as good (and 
 * certainly much faster) to instead use file_sampler(), which
 * randomly subsamples at a proportion.
 *
 * @param header
 * Input.  Indicates whether or not there is a header line (as in a
 * csv).
 * @param nskip
 * Input.  Number of lines to skip.  If header=true and nskip>0, then
 * the number of lines skipped applies to post-header lines only.
 * @param nlines_in
 * Input.  The number of lines of the input file.  See wc().
 * @param nlines_out
 * Input.  The precise number of lines of input to (randomly) to 
 * retain.
 * @param input
 * Input.  Absolute path to input file.
 * @param output
 * Input.  Absolute path to output file.
 *
 * @note
 * Due to R's RNG, this call (as written) is very un-threadsafe.
 * 
 * @return
 * The return value indicates the status of the function.
 */
int file_sampler_exact(bool header, uint64_t nlines_in, uint64_t nlines_out, const int nskip, const char *input, const char *output)
{
  int ret = 0;
  FILE *fp_read, *fp_write;
  char *buf;
  int readlen;
  // Have to track cases where buffer is too small for fgets()
  bool should_write = false;
  bool firstread = true;
  uint64_t *samp;
  
  
  if (nskip > nlines_in)
    return INVALID_NSKIP;
  
  fp_read = fopen(input, "r");
  if (!fp_read) return READ_FAIL;
  fp_write = fopen(output, "w");
  
  buf = malloc(BUFLEN * sizeof(char));
  
  
  if (header)
    read_header(buf, fp_read, fp_write, &nlines_in, &nlines_out);
  
  ret = res_sampler(nskip, nlines_in, nlines_out, &samp);
  if (ret) goto cleanup;
  
  qsort(samp, nlines_out, sizeof(uint64_t), comp);
  
  GetRNGstate();
  
  
  nlines_in = 0;
  nlines_out = 0;
  while (fgets(buf, BUFLEN, fp_read) != NULL)
  {
    if (firstread)
    {
      if (samp[nlines_out] == nlines_in)
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
  
  free(samp);
  cleanup:
    fclose(fp_read);
    fclose(fp_write);
    free(buf);
  
  return ret;
}

