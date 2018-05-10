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


#include <ctype.h> // isspace()

#include "lineSampler.h"
#include "safeomp.h"
#include "utils.h"


static inline bool isnewline(const char c)
{
  return (c=='\n') ? true : false;
}



#ifdef __AVX2__
// we have AVX2 support

#ifndef _MSC_VER
/* Non-Microsoft C/C++-compatible compiler */
#include <x86intrin.h> // on some recent GCC, this will declare posix_memalign
#else
/* Microsoft C/C++-compatible compiler */
#include <intrin.h>
#endif

// http://lemire.me/blog/2017/02/14/how-fast-can-you-count-lines/
static size_t linefeedcount(char * buffer, size_t size) {
    size_t answer = 0;
    __m256i cnt = _mm256_setzero_si256();
    __m256i newline = _mm256_set1_epi8('\n');
    size_t i = 0;
    uint8_t tmpbuffer[sizeof(__m256i)];
    while( i + 32 <= size ) {
        size_t remaining = size - i;
        size_t howmanytimes =  remaining / 32;
        if(howmanytimes > 256) howmanytimes = 256;
        const __m256i * buf = (const __m256i *) (buffer + i);
        size_t j = 0;
        for (; j + 3 <  howmanytimes; j+= 4) {
            __m256i newdata1 = _mm256_lddqu_si256(buf + j);
            __m256i newdata2 = _mm256_lddqu_si256(buf + j + 1);
            __m256i newdata3 = _mm256_lddqu_si256(buf + j + 2);
            __m256i newdata4 = _mm256_lddqu_si256(buf + j + 3);
            __m256i cmp1 = _mm256_cmpeq_epi8(newline,newdata1);
            __m256i cmp2 = _mm256_cmpeq_epi8(newline,newdata2);
            __m256i cmp3 = _mm256_cmpeq_epi8(newline,newdata3);
            __m256i cmp4 = _mm256_cmpeq_epi8(newline,newdata4);
            __m256i cnt1 = _mm256_add_epi8(cmp1,cmp2);
            __m256i cnt2 = _mm256_add_epi8(cmp3,cmp4);
            cnt = _mm256_add_epi8(cnt,cnt1);
            cnt = _mm256_add_epi8(cnt,cnt2);
        }
        for (; j <  howmanytimes; j++) {
            __m256i newdata = _mm256_lddqu_si256(buf + j);
            __m256i cmp = _mm256_cmpeq_epi8(newline,newdata);
            cnt = _mm256_add_epi8(cnt,cmp);
        }
        i += howmanytimes * 32;
        cnt = _mm256_subs_epi8(_mm256_setzero_si256(),cnt);
        _mm256_storeu_si256((__m256i *) tmpbuffer,cnt);
        for(int k = 0; k < sizeof(__m256i); ++k) answer += tmpbuffer[k];
        cnt = _mm256_setzero_si256();
    }
    for(; i < size; i++)
        if(buffer[i] == '\n') answer ++;
    return answer;
}


#else

// we do not have AVX2 support
static size_t linefeedcount(char * buffer, size_t size) {
    size_t cnt = 0;
    char * ptr = buffer;
    char * last = buffer + size;
    while((ptr = memchr(ptr,'\n', last - ptr))) {
      cnt ++;
      ptr ++;
    }
    return cnt;
}
#endif

static int wc_charsonly(FILE *restrict fp, char *restrict buf, uint64_t *restrict nchars)
{
  size_t readlen = BUFLEN;
  uint64_t nc = 0;

  while (readlen == BUFLEN)
  {
    if (check_interrupt())
      return USER_INTERRUPT;

    readlen = fread(buf, sizeof(char), BUFLEN, fp);
    nc += readlen;
  }

  *nchars = nc;

  return 0;
}



static int wc_linesonly(FILE *restrict fp, char *restrict buf, uint64_t *restrict nlines)
{
  size_t readlen = BUFLEN;
  uint64_t nl = 0;

  while (readlen == BUFLEN)
  {
    if (check_interrupt())
      return USER_INTERRUPT;

    readlen = fread(buf, sizeof(*buf), BUFLEN, fp);
    nl += linefeedcount(buf, readlen);
  }

  *nlines = nl;

  return 0;
}



static int wc_nowords(FILE *restrict fp, char *restrict buf, uint64_t *restrict nchars, uint64_t *restrict nlines)
{
  size_t readlen = BUFLEN;
  uint64_t nc = 0;
  uint64_t nl = 0;

  while (readlen == BUFLEN)
  {
    if (check_interrupt())
      return USER_INTERRUPT;

    readlen = fread(buf, sizeof(*buf), BUFLEN, fp);
    nl += linefeedcount(buf, readlen);
    nc += readlen;
  }

  *nchars = nc;
  *nlines = nl;

  return 0;
}



static int wc_nolines(FILE *restrict fp, char *restrict buf, uint64_t *restrict nchars, uint64_t *restrict nwords)
{
  size_t readlen = BUFLEN;
  uint64_t nc = 0;
  uint64_t nw = 0;

  while (readlen == BUFLEN)
  {
    if (check_interrupt())
      return USER_INTERRUPT;

    readlen = fread(buf, sizeof(char), BUFLEN, fp);

    SAFE_FOR_SIMD
    for (size_t i=0; i<readlen; i++)
    {
      if (isspace(buf[i]))
        nw++;

      nc++;
    }
  }

  *nchars = nc;
  *nwords = nw;

  return 0;
}



static int wc_full(FILE *restrict fp, char *restrict buf, uint64_t *restrict nchars, uint64_t *restrict nwords, uint64_t *restrict nlines)
{
  uint64_t nc = 0;
  uint64_t nw = 0;
  uint64_t nl = 0;
  size_t readlen = BUFLEN;

  while (readlen == BUFLEN)
  {
    if (check_interrupt())
      return USER_INTERRUPT;

    readlen = fread(buf, sizeof(char), BUFLEN, fp);
    nc += readlen;

    SAFE_FOR_SIMD
    for (size_t i=0; i<readlen; i++)
    {
      if (isnewline(buf[i]))
      {
        nw++;
        nl++;
      }
      else
      {
        if (isspace(buf[i]))
          nw++;
      }
    }
  }

  *nchars = nc;
  *nwords = nw;
  *nlines = nl;

  return 0;
}



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
 * @param chars
 *
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
int LS_wc(const char *file, const bool chars, uint64_t *nchars,
  const bool words, uint64_t *nwords, const bool lines, uint64_t *nlines)
{
  int ret = 0;
  FILE *fp;
  char *buf;

  fp = fopen(file, "r");
  if (!fp)
    return READ_FAIL;

  buf = malloc(BUFLEN * sizeof(*buf));
  if (buf == NULL)
  {
    fclose(fp);
    return MALLOC_FAIL;
  }

  if (!chars && !words && lines)
    ret = wc_linesonly(fp, buf, nlines);
  else if (chars && !words && lines)
    ret = wc_nowords(fp, buf, nchars, nlines);
  else if (chars && words && !lines)
    ret = wc_nolines(fp, buf, nchars, nwords);
  else if (chars && !words && !lines)
    ret = wc_charsonly(fp, buf, nchars);
  else
    ret = wc_full(fp, buf, nchars, nwords, nlines);

  fclose(fp);
  free(buf);

  return ret;
}
