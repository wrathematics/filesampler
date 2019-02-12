#ifndef FILESAMPLER_CHECK_AVX_H
#define FILESAMPLER_CHECK_AVX_H


#if defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 1300)
  #include <immintrin.h>
  #define CHECK_FOR_AVX2 _may_i_use_cpu_feature(_FEATURE_AVX2)
#elif (defined(__GNUC__) || defined(__clang__))
  #define CHECK_FOR_AVX2 __builtin_cpu_supports("avx2")
#else
  #define CHECK_FOR_AVX2 0;
#endif


static inline int has_avx2()
{
  static int check = -1;
  if (check < 0 )
    check = CHECK_FOR_AVX2;
  
  return (check > 0);
}


#endif
