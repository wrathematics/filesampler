/*  Copyright (c) 2015-2016, Schmidt
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


#ifndef LINESAMPLER_ERROR_H_
#define LINESAMPLER_ERROR_H_


#include "utils.h"

// Parameter checks
#define INVALID_PROB    -1
#define INVALID_NSKIP   -2

#define INVALID_PROB_MSG  "Invalid `p` specified. Must be between 0 and 1."
#define INVALID_NSKIP_MSG "Invalid `nskip` specified; "


// Things that went wrong
#define READ_FAIL       -3
#define WRITE_FAIL      -4
#define MALLOC_FAIL     -5
#define USER_INTERRUPT  -6

#define READ_FAIL_MSG       "Could not read infile; perhaps it doesn't exist?"
#define WRITE_FAIL_MSG      "Could not generate tempfile for writing for some reason?"
#define MALLOC_FAIL_MSG     "Out of memory."
#define USER_INTERRUPT_MSG  "Process killed by user interrupt."


static inline void LS_checkret(const int ret)
{
  switch (ret)
  {
    case 0:
      break;
    case INVALID_PROB:
      LS_error_fun(ret, INVALID_PROB_MSG);
      break;
    case INVALID_NSKIP:
      LS_error_fun(ret, INVALID_NSKIP_MSG);
      break;
    case READ_FAIL:
      LS_error_fun(ret, READ_FAIL_MSG);
      break;
    case WRITE_FAIL:
      LS_error_fun(ret, WRITE_FAIL_MSG);
      break;
    case MALLOC_FAIL:
      LS_error_fun(ret, MALLOC_FAIL_MSG);
      break;
    case USER_INTERRUPT:
      LS_error_fun(ret, USER_INTERRUPT_MSG);
      break;
    default:
      LS_error_fun(ret, "Unknown error code; please report this to the developers.");
      break;
  }
  
  return;
}


#endif
