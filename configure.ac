AC_PREREQ([2.69])
AC_INIT(DESCRIPTION)
AC_CONFIG_MACRO_DIRS([tools/])

# Get C compiler from R
: ${R_HOME=`R RHOME`}
if test -z "${R_HOME}"; then
  echo "could not determine R_HOME"
  exit 1
fi
CC=`"${R_HOME}/bin/R" CMD config CC`
CFLAGS=`"${R_HOME}/bin/R" CMD config CFLAGS`
CPPFLAGS=`"${R_HOME}/bin/R" CMD config CPPFLAGS`


AC_PROG_CC_C99
AC_OPENMP
if test -n "${OPENMP_CFLAGS}"; then
  have_omp="yes"
  OMP_FLAGS="\$(SHLIB_OPENMP_CFLAGS)"
else
  have_omp="no"
  OMP_FLAGS=""
fi



AX_CHECK_COMPILE_FLAG("-mavx2", [have_avx2="yes"], [have_avx2="no"])
if test "X${have_avx2}" = Xyes; then
  AVX2_FLAGS="-mavx2"
else
  AVX2_FLAGS=""
fi


echo " "
echo "**************** Results of filesampler package configure ****************"
echo " "
echo "* OpenMP Report"
echo "*   >> Compiler support: ${have_omp}"
echo "*   >> CFLAGS = ${OMP_FLAGS}"
echo "* avx2 Report"
echo "*   >> Compiler support: ${have_avx2}"
echo "*   >> CPPFLAGS = ${AVX2_FLAGS}"
echo "**************************************************************************"
echo " "

AC_SUBST(OMP_FLAGS)
AC_SUBST(AVX2_FLAGS)
AC_OUTPUT(src/Makevars)
