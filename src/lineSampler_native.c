/* Automatically generated. Do not edit by hand. */

#include <R.h>
#include <Rinternals.h>
#include <R_ext/Rdynload.h>
#include <stdlib.h>

extern SEXP R_LS_sample_exact(SEXP verbose, SEXP header, SEXP nskip_, SEXP nlines_out_, SEXP input, SEXP output);
extern SEXP R_LS_sample_prob(SEXP verbose, SEXP header, SEXP nskip_, SEXP nmax_, SEXP p, SEXP input, SEXP output);
extern SEXP R_LS_wc(SEXP input, SEXP chars_, SEXP words_, SEXP lines_);

static const R_CallMethodDef CallEntries[] = {
  {"R_LS_sample_exact", (DL_FUNC) &R_LS_sample_exact, 6},
  {"R_LS_sample_prob", (DL_FUNC) &R_LS_sample_prob, 7},
  {"R_LS_wc", (DL_FUNC) &R_LS_wc, 4},
  {NULL, NULL, 0}
};
void R_init_lineSampler(DllInfo *dll)
{
  R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
  R_useDynamicSymbols(dll, FALSE);
}
