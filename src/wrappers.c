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


#include <R.h>
#include <Rinternals.h>
#include "lineSampler.h"

#define CHARPT(x,i) ((char*)CHAR(STRING_ELT(x,i)))
#define INT(x) INTEGER(x)[0]
#define DBL(x) REAL(x)[0]

SEXP R_file_sampler(SEXP verbose, SEXP header, SEXP nskip, SEXP nmax, SEXP p, SEXP input, SEXP output)
{
  SEXP ret;
  PROTECT(ret = allocVector(INTSXP, 1));
  
  INT(ret) = file_sampler(INT(verbose), INT(header), (uint32_t)INT(nskip), (uint32_t)INT(nmax), DBL(p), CHARPT(input, 0), CHARPT(output, 0));
  
  UNPROTECT(1);
  return ret;
}



SEXP R_file_sampler_exact(SEXP header, SEXP nskip, SEXP nlines_out, SEXP input, SEXP output)
{
  SEXP ret;
  PROTECT(ret = allocVector(INTSXP, 1));
  uint64_t nlines_in;
  
  INT(ret) = file_sampler_wc(CHARPT(input, 0), false, NULL, false, NULL, true, &nlines_in);
  if (INT(ret)) goto cleanup;
  
  INT(ret) = file_sampler_exact(INT(header), nlines_in, INT(nlines_out), (uint32_t)INT(nskip), CHARPT(input, 0), CHARPT(output, 0));
  
  cleanup:
    UNPROTECT(1);
  
  return ret;
}



#define COUNTS(n) REAL(counts)[n]
#define NCHARS  0
#define NWORDS  1
#define NLINES  2
SEXP R_wc(SEXP input, SEXP chars_, SEXP words_, SEXP lines_)
{
  int retval;
  uint64_t nchars, nwords, nlines;
  const bool chars = INT(chars_);
  const bool words = INT(words_);
  const bool lines = INT(lines_);
  SEXP counts;
  // REALSXP because R is too stupid to have 64-bit ints already
  PROTECT(counts = allocVector(REALSXP, 3));
  
  retval = file_sampler_wc(CHARPT(input, 0), chars, &nchars, words, &nwords, lines, &nlines);
  
  if (!retval)
  {
    if (retval == MALLOC_FAIL)
      error("Out of memory");
    else if (retval == READ_FAIL)
      error("Could not read file; perhaps it doesn't exist?");
  }
  
  COUNTS(NCHARS) = chars ? (double) nchars : -1.;
  COUNTS(NWORDS) = words ? (double) nwords : -1.;
  COUNTS(NLINES) = lines ? (double) nlines : -1.;
  
  UNPROTECT(1);
  return counts;
}
