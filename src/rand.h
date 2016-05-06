#ifndef LINESAMPLER_RAND_H_
#define LINESAMPLER_RAND_H_

// To make this useable outside of R, you need to:
//   1. remove the Get/PutRNGstate() calls
//   2. change RUNIF def to random uniform(0,1) rng of your choice.
//   3. set PRINTFUN to (probably) printf(). */

#include <R.h>
#include <Rmath.h>


#define RUNIF unif_rand
#define PRINTFUN Rprintf

#define STARTRNG GetRNGstate
#define ENDRNG PutRNGstate


#endif
