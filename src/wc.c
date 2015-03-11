// Copyright 2015 Drew Schmidt.  All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file.

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
 * @param nletters
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
int wc(const char *file, uint64_t *nletters, uint64_t *nwords, uint64_t *nlines)
{
  int i;
  FILE *fp;
  char *buf;
  size_t readlen;
  
  *nletters = 0;
  *nwords = 0;
  *nlines = 0;
  
  fp = fopen(file, "r");
  if (!fp) return READ_FAIL;
  
  buf = malloc(BUFLEN * sizeof(char));
  
  while (true)
  {
    R_CheckUserInterrupt();
    
    readlen = fread(buf, sizeof(char), BUFLEN, fp);
    
    for (i=0; i<readlen; i++)
    {
      if (buf[i] == '\n')
      {
        (*nwords)++;
        (*nlines)++;
      }
      else
      {
        if (buf[i] == ' ')
          (*nwords)++;
        
        (*nletters)++;
      }
    }
    
    if (readlen < BUFLEN) break;
  }
  
  free(buf);
  
  return 0;
}

