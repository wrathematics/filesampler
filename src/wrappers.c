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
  uint64_t nletters, nwords, nlines_in;
  
  INT(ret) = wc(CHARPT(input, 0), &nletters, &nwords, &nlines_in);
  if (INT(ret)) goto cleanup;
  
  INT(ret) = file_sampler_exact(INT(header), nlines_in, INT(nlines_out), (uint32_t)INT(nskip), CHARPT(input, 0), CHARPT(output, 0));
  
  cleanup:
    UNPROTECT(1);
  
  return ret;
}



#define COUNTS(n) REAL(counts)[n]
#define RETVAL    0
#define NLETTERS  1
#define NWORDS    2
#define NLINES    3
SEXP R_wc(SEXP input)
{
  int retval;
  uint64_t nletters, nwords, nlines;
  // Putting everything in an int vec because FUCK lists
  SEXP counts;
  // REALSXP because R is too stupid to have 64-bit ints already
  PROTECT(counts = allocVector(REALSXP, 4));
  
  retval = wc(CHARPT(input, 0), &nletters, &nwords, &nlines);
  
  COUNTS(RETVAL) = (double) retval;
  COUNTS(NLETTERS) = (double) nletters;
  COUNTS(NWORDS) = (double) nwords;
  COUNTS(NLINES) = (double) nlines;
  
  UNPROTECT(1);
  return counts;
}

