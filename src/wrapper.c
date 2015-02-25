#include <R.h>
#include <Rinternals.h>
#include "lineSampler.h"

#define CHARPT(x,i)     ((char*)CHAR(STRING_ELT(x,i)))


SEXP R_file_sampler(SEXP header, SEXP p, SEXP input, SEXP output)
{
  SEXP ret;
  PROTECT(ret = allocVector(INTSXP, 1));
  
  INTEGER(ret)[0] = file_sampler(INTEGER(header)[0], REAL(p)[0], CHARPT(input, 0), CHARPT(output, 0));
  
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

