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


#include "Rfilesampler.h"
#include "filesampler/filesampler.h"

#define COUNTS(n) REAL(counts)[n]
#define NCHARS  0
#define NWORDS  1
#define NLINES  2

#define BADVAL -1.0


SEXP R_fs_wc(SEXP input, SEXP chars_, SEXP words_, SEXP lines_)
{
  SEXP counts;
  int ret;
  uint64_t nchars, nwords, nlines;
  
  const bool chars = INT(chars_);
  const bool words = INT(words_);
  const bool lines = INT(lines_);
  
  PROTECT(counts = allocVector(REALSXP, 3));
  
  ret = fs_wc(CHARPT(input, 0), chars, &nchars, words, &nwords, lines, &nlines);
  fs_checkret(ret);
  
  COUNTS(NCHARS) = chars ? (double) nchars : BADVAL;
  COUNTS(NWORDS) = words ? (double) nwords : BADVAL;
  COUNTS(NLINES) = lines ? (double) nlines : BADVAL;
  
  UNPROTECT(1);
  return counts;
}
