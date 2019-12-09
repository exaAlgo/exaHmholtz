#ifndef _EXA_HMHOLTZ_IMPL_H_
#define _EXA_HMHOLTZ_IMPL_H_

#include "exa-hmholtz.h"

struct exaHmholtz_private{
  exaHandle h;
  exaProgram prog;
  // kernels
  exaKernel Ax;
  exaKernel weightedNorm2;
  // device vectors
  exaVector d_x,d_b,d_r;
};

#endif
