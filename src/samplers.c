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


SEXP R_fs_sample_prop(SEXP verbose, SEXP header, SEXP nskip_, SEXP nmax_, SEXP p, SEXP input, SEXP output)
{
  int ret;
  
  const uint32_t nskip = (uint32_t) INT(nskip_);
  const uint32_t nmax = (uint32_t) INT(nmax_);
  
  ret = fs_sample_prop(INT(verbose), INT(header), nskip, nmax, DBL(p), CHARPT(input, 0), CHARPT(output, 0));
  fs_checkret(ret);
  
  return R_NilValue;
}



SEXP R_fs_sample_exact(SEXP verbose, SEXP header, SEXP nskip_, SEXP nlines_out_, SEXP input, SEXP output)
{
  int ret;
  
  const uint32_t nskip = (uint32_t) INT(nskip_);
  const uint32_t nlines_out = (uint32_t) INT(nlines_out_);
  
  ret = fs_sample_exact(INT(verbose), INT(header), nskip, nlines_out, CHARPT(input, 0), CHARPT(output, 0));
  fs_checkret(ret);
  
  return R_NilValue;
}
