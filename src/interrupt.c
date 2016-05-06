// Idea taken from http://permalink.gmane.org/gmane.comp.lang.r.devel/27627
// to use outside of R, write appropriate check_interrupt() function
// or just set to always false if you're lazy.

#include <Rinternals.h>
#include <R_ext/Utils.h>
#include <stdbool.h>

static void chkIntFn(void *dummy)
{
  R_CheckUserInterrupt();
}

// this will call the above in a top-level context so it won't longjmp-out of your context
bool check_interrupt()
{
  return (R_ToplevelExec(chkIntFn, NULL) == FALSE);
}
