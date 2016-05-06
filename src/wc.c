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


#include <R_ext/Utils.h>
#include <ctype.h>
#include "lineSampler.h"


static inline bool isnewline(const char c)
{
  return (c=='\n') ? true : false;
}


static int wc_linesonly(FILE *restrict fp, char *restrict buf, uint64_t *restrict nlines)
{
  size_t readlen = BUFLEN;
  uint64_t nl = 0;
  
  while (readlen == BUFLEN)
  {
    R_CheckUserInterrupt();
    
    readlen = fread(buf, sizeof(char), BUFLEN, fp);
    
    #pragma omp for simd
    for (int i=0; i<readlen; i++)
    {
      if (isnewline(buf[i]))
        nl++;
    }
  }
  
  *nlines = nl;
  
  return 0;
}


static int wc_nolines(FILE *restrict fp, char *restrict buf, uint64_t *restrict nchars, uint64_t *restrict nwords)
{
  uint64_t nc = 0;
  uint64_t nw = 0;
  size_t readlen = BUFLEN;
  
  while (readlen == BUFLEN)
  {
    R_CheckUserInterrupt();
    
    readlen = fread(buf, sizeof(char), BUFLEN, fp);
    
    #pragma omp for simd
    for (int i=0; i<readlen; i++)
    {
      if (isspace(buf[i]))
        nw++;
      
      nc++;
    }
  }
  
  *nchars = nc;
  *nwords = nw;
  
  return 0;
}


static int wc_full(FILE *restrict fp, char *restrict buf, uint64_t *restrict nchars, uint64_t *restrict nwords, uint64_t *restrict nlines)
{
  uint64_t nc = 0;
  uint64_t nw = 0;
  uint64_t nl = 0;
  size_t readlen = BUFLEN;
  
  while (readlen == BUFLEN)
  {
    R_CheckUserInterrupt();
    
    readlen = fread(buf, sizeof(char), BUFLEN, fp);
    
    #pragma omp for simd
    for (int i=0; i<readlen; i++)
    {
      if (isnewline(buf[i]))
      {
        nw++;
        nl++;
      }
      else
      {
        if (isspace(buf[i]))
          nw++;
      }
      
      nc++;
    }
  }
  
  *nchars = nc;
  *nwords = nw;
  *nlines = nl;
  
  return 0;
}


/**
 * @file
 * @brief 
 * Wordcounts
 *
 * @details
 * This function emulates the standard unix wc command, to generate
 * letter, word, and line counts from a text file.
 *
 * @param file
 * Input.  Absolute path to output file.
 * @param chars
 * 
 * @param nchars
 * Output, passed by reference.  On successful return, the value
 * is set to the number of "letters" (character) in the file.
 * @param nwords
 * Output, passed by reference.  On successful return, the value
 * is set to the number of "words" (space separated tokens) in the
 * file.
 * @param nlines
 * Output, passed by reference.  On successful return, the value
 * is set to the number of lines in the file.
 * 
 * @return
 * The return value indicates the status of the function.
 */
int file_sampler_wc(const char *file, const bool chars, uint64_t *nchars, 
  const bool words, uint64_t *nwords, const bool lines, uint64_t *nlines)
{
  int ret = 0;
  FILE *fp;
  char *buf;
  
  *nchars = 0;
  *nwords = 0;
  *nlines = 0;
  
  fp = fopen(file, "r");
  if (!fp)
    return READ_FAIL;
  
  buf = malloc(BUFLEN * sizeof(char));
  // TODO
  // if ()
  //   return MALLOC_FAIL;
  
  if (!chars && !words && lines)
    ret = wc_linesonly(fp, buf, nlines);
  else
    ret = wc_full(fp, buf, nchars, nwords, nlines);
  
  
  fclose(fp);
  free(buf);
  
  return ret;
}
