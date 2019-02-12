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
#include "filesampler/filesampler.h"

#define CHARPT(x,i) ((char*)CHAR(STRING_ELT(x,i)))
#define INT(x) INTEGER(x)[0]
#define DBL(x) REAL(x)[0]


// -----------------------------------------------------------------------------
// file samplers
// -----------------------------------------------------------------------------

SEXP R_fs_sample_prob(SEXP verbose, SEXP header, SEXP nskip_, SEXP nmax_, SEXP p, SEXP input, SEXP output)
{
  int ret;
  uint32_t nskip = (uint32_t) INT(nskip_);
  uint32_t nmax = (uint32_t) INT(nmax_);
  
  ret = fs_sample_prob(INT(verbose), INT(header), nskip, nmax, DBL(p), CHARPT(input, 0), CHARPT(output, 0));
  fs_checkret(ret);
  
  return R_NilValue;
}



SEXP R_fs_sample_exact(SEXP verbose, SEXP header, SEXP nskip_, SEXP nlines_out_, SEXP input, SEXP output)
{
  int ret;
  uint32_t nskip = (uint32_t) INT(nskip_);
  uint32_t nlines_out = (uint32_t) INT(nlines_out_);
  
  ret = fs_sample_exact(INT(verbose), INT(header), nskip, nlines_out, CHARPT(input, 0), CHARPT(output, 0));
  fs_checkret(ret);
  
  return R_NilValue;
}



// -----------------------------------------------------------------------------
// counters
// -----------------------------------------------------------------------------

#define COUNTS(n) REAL(counts)[n]
#define NCHARS  0
#define NWORDS  1
#define NLINES  2

SEXP R_fs_wc(SEXP input, SEXP chars_, SEXP words_, SEXP lines_)
{
  int ret;
  uint64_t nchars, nwords, nlines;
  const bool chars = INT(chars_);
  const bool words = INT(words_);
  const bool lines = INT(lines_);
  SEXP counts;
  // REALSXP because R is too stupid to have 64-bit ints already
  PROTECT(counts = allocVector(REALSXP, 3));
  
  ret = fs_wc(CHARPT(input, 0), chars, &nchars, words, &nwords, lines, &nlines);
  fs_checkret(ret);
  
  COUNTS(NCHARS) = chars ? (double) nchars : -1.0;
  COUNTS(NWORDS) = words ? (double) nwords : -1.0;
  COUNTS(NLINES) = lines ? (double) nlines : -1.0;
  
  UNPROTECT(1);
  return counts;
}
