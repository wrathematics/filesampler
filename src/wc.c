// Copyright 2015 Drew Schmidt.  All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file.

#include "lineSampler.h"
#include <R_ext/Utils.h>


int wc(const char *file, uint64_t *nletters, uint64_t *nwords, uint64_t *nlines)
{
  FILE *fp;
  char c;
  
  *nletters = 0;
  *nwords = 0;
  *nlines = 0;
  
  fp = fopen(file, "r");
  if (!fp) return READ_FAIL;
  
  while ((c = getc(fp)) != EOF)
  {
    R_CheckUserInterrupt();
    
    if (c == '\n')
    {
      (*nwords)++;
      (*nlines)++;
    }
    else
    {
      if (c == ' ')
        (*nwords)++;
      
      (*nletters)++;
    }
  }
  
  
  return 0;
}

