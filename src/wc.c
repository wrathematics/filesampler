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
#include <R_ext/Utils.h>


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
int file_sampler_wc(const char *file, uint64_t *nchars, uint64_t *nwords, uint64_t *nlines)
{
  int i;
  FILE *fp;
  char *buf;
  size_t readlen = BUFLEN;
  
  *nchars = 0;
  *nwords = 0;
  *nlines = 0;
  
  fp = fopen(file, "r");
  if (!fp) return READ_FAIL;
  
  buf = malloc(BUFLEN * sizeof(char));
  
  while (readlen == BUFLEN)
  {
    R_CheckUserInterrupt();
    
    readlen = fread(buf, sizeof(char), BUFLEN, fp);
    
    for (i=0; i<readlen; i++)
    {
      if (buf[i] == '\n')
      {
        (*nwords)++;
        (*nlines)++;
        (*nchars)++;
      }
      else
      {
        if (buf[i] == ' ')
          (*nwords)++;
        
        (*nchars)++;
      }
    }
  }
  
  free(buf);
  
  return 0;
}
