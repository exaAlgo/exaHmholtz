#ifndef _EXA_HMHOLTZ_IMPL_H_
#define _EXA_HMHOLTZ_IMPL_H_

#include "exa-hmholtz.h"
#include "exa-memory.h"

#define max(a,b) ((a)>(b) ? (a) : (b))

struct exaHmholtz_private{
  exaHandle h;
  exaSettings s;
  // program
  exaProgram p;
  // kernels
  exaKernel Ax;
  exaKernel vectorWeightedNorm2;
  exaKernel vectorInnerProduct2;
  exaKernel vectorWeightedInnerProduct2;
  exaKernel vectorScaledAdd;
  // device vectors
  exaVector tmp_1,tmp_2,tmp_3,tmp_4,tmp_5;
};

#endif
