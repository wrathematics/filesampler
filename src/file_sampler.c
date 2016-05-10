/*  Copyright (c) 2015-2016, Drew Schmidt
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
    TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
    PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
    CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
    EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
    PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
    PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
    LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
    NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#include "lineSampler.h"
#include "omputils.h"
#include "rand.h"

#define HAS_NEWLINE ((readlen > 0) && (buf[readlen-1] == '\n'))


static inline void read_header(char *buf, FILE *fp_read, FILE *fp_write, uint64_t *nlines_in, uint64_t *nlines_out)
{
  size_t readlen;
  
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
 * @param nmax
 * Input.  Max number of lines to read.  If nmax==0 then there is no max.
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
int file_sampler(const bool verbose, const bool header, uint32_t nskip, uint32_t nmax, const double p, const char *input, const char *output)
{
  int ret = 0;
  FILE *fp_read, *fp_write;
  char *buf;
  size_t readlen;
  // Have to track cases where buffer is too small for fgets()
  bool should_write = false;
  bool firstread = true;
  bool checkmax = nmax ? true : false;
  uint64_t nlines_in = 0, nlines_out = 0;
  
  if (p < 0. || p > 1.)
    return INVALID_PROB;
  else if (p == 0.)
  {
    if (verbose)
      PRINTFUN("Read 0 lines of unknown length file (p == 0).\n");
    
    return 0;
  }
  
  
  fp_read = fopen(input, "r");
  if (!fp_read)
    return READ_FAIL;
  
  fp_write = fopen(output, "w");
  if (!fp_write)
  {
    fclose(fp_read);
    return WRITE_FAIL;
  }
  
  buf = malloc(BUFLEN * sizeof(char));
  if (buf == NULL)
  {
    fclose(fp_read);
    fclose(fp_write);
    return MALLOC_FAIL;
  }
  
  
  if (header)
    read_header(buf, fp_read, fp_write, &nlines_in, &nlines_out);
  
  
  STARTRNG();
  
  while (fgets(buf, BUFLEN, fp_read) != NULL)
  {
    if ((nlines_out % 1000 == 0) && check_interrupt())
    {
      ret = USER_INTERRUPT;
      goto cleanup;
    }
    
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
      
      if (checkmax)
      {
        nmax--;
        if (!nmax) break;
      }
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
  
  if (verbose)
  {
    if (checkmax && !nmax)
      PRINTFUN("Read nmax=%llu lines.\n", nmax);
    else
      PRINTFUN("Read %llu lines (%.5f%%) of %llu line file.\n", nlines_out, (double) nlines_out/nlines_in, nlines_in);
  }
  
  
  cleanup:
    ENDRNG();
    fclose(fp_read);
    fclose(fp_write);
    free(buf);
  
  return ret;
}





// ------------------------------------------------------
// exact reader
// ------------------------------------------------------

static inline int unif_rand_int(const int low, const int high)
{
  return (int) low + (high + 1 - low)*RUNIF() ;
}



static int res_sampler(const uint32_t nskip, const uint64_t nlines_in, const uint64_t nlines_out, uint64_t **samp)
{
  int i, j;
  *samp = malloc(nlines_out * sizeof(**samp));
  if (samp == NULL)
    return MALLOC_FAIL;
  
  SAFE_FOR_SIMD
  for (i=0; i<nlines_out; i++)
    (*samp)[i] = nskip + i+1;
  
  STARTRNG();
  
  SAFE_FOR_SIMD
  for (i=nlines_out; i<nlines_in; i++)
  {
    j = unif_rand_int(0, i-1);
    if (j < nlines_out)
      (*samp)[j] = nskip + i+1;
  }
  
  ENDRNG();
  
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
int file_sampler_exact(const bool header, uint64_t nlines_in, uint64_t nlines_out, const uint32_t nskip, const char *input, const char *output)
{
  int ret = 0;
  FILE *fp_read, *fp_write;
  char *buf;
  size_t readlen;
  // Have to track cases where buffer is too small for fgets()
  bool should_write = false;
  bool firstread = true;
  uint64_t *samp;
  
  
  if (nskip > nlines_in)
    return INVALID_NSKIP;
  
  fp_read = fopen(input, "r");
  if (!fp_read) 
    return READ_FAIL;
  
  fp_write = fopen(output, "w");
  if (!fp_write)
  {
    fclose(fp_read);
    return WRITE_FAIL;
  }
  
  buf = malloc(BUFLEN * sizeof(char));
  if (buf == NULL)
  {
    fclose(fp_read);
    fclose(fp_write);
    return MALLOC_FAIL;
  }
  
  
  if (header)
    read_header(buf, fp_read, fp_write, &nlines_in, &nlines_out);
  
  ret = res_sampler(nskip, nlines_in, nlines_out, &samp);
  if (ret) 
    goto cleanup;
  
  qsort(samp, nlines_out, sizeof(uint64_t), comp);
  
  STARTRNG();
  
  
  nlines_in = 0;
  nlines_out = 0;
  while (fgets(buf, BUFLEN, fp_read) != NULL)
  {
    if ((nlines_out % 1000 == 0) && check_interrupt())
    {
      ret = USER_INTERRUPT;
      goto fullcleanup;
    }
    
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
  
  
  fullcleanup:
    ENDRNG();
    free(samp);
  
  cleanup:
    fclose(fp_read);
    fclose(fp_write);
    free(buf);
  
  return ret;
}
