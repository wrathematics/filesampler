// Copyright 2015 Drew Schmidt.  All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file.

#include "lineSampler.h"
#include <R_ext/Utils.h>


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

