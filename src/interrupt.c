// Idea taken from http://permalink.gmane.org/gmane.comp.lang.r.devel/27627
// To use outside of R, write appropriate check_interrupt() function
// or just set to always false if you're lazy.

#include <Rinternals.h>
#include <R_ext/Utils.h>
#include <stdbool.h>


static void chkIntFn(void *dummy)
{
  R_CheckUserInterrupt();
}

bool check_interrupt()
{
  return (R_ToplevelExec(chkIntFn, NULL) == FALSE);
}
