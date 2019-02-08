/*  Copyright (c) 2015-2018, Drew Schmidt
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


#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "filesampler.h"
#include "utils.h"


#define HEADER_ALL 0
#define HEADER_FIRST 1
#define HEADER_NONE 2


int fs_rebalance(const uint32_t num_outfiles, const uint32_t num_infiles, const char **infiles, const int inheader, const int outheader, const char *outdir)
{
  int ret = 0;
  FILE *fp_read, *fp_write;
  uint64_t total_lines;
  uint64_t outfile_lines;
  uint64_t rem;
  
  if (num_infiles == num_outfiles)
    return ret;
  
  
  buf = malloc(BUFLEN * sizeof(char));
  if (buf == NULL)
    return MALLOC_FAIL;
  
  for (uint32_t i=0; i<num_infiles; i++)
  {
    uint64_t nlines;
    int wcret = fs_wc(infiles[i], false, NULL, false, NULL, true, &nlines)
    total_lines += nlines;
  }
  
  
  outfile_lines = total_lines / num_outfiles;
  rem = total_lines - outfile_lines*num_outfiles;
  
  
  
  
  for (uint32_t i=0; i<num_infiles; i++)
  {
    fp_read = fopen(infiles[i], "r");
    if (!fp_read)
    {
      ret = READ_FAIL;
      goto cleanup;
    }
    
    fp_write = fopen(output, "w");
    if (!fp_write)
    {
      fclose(fp_read);
      ret = WRITE_FAIL;
      goto cleanup;
    }
  }
  
  
cleanup:
  free(buf);
  return ret;
}
