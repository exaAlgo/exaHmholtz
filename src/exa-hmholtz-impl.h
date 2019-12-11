#ifndef _EXA_HMHOLTZ_IMPL_H_
#define _EXA_HMHOLTZ_IMPL_H_

#include "exa-hmholtz.h"
#include "exa-memory.h"

#define max(a,b) ((a)>(b)) ? (a) : (b)

struct exaHmholtz_private{
  exaHandle h;
  exaSettings s;
  // program
  exaProgram p;
  // kernels
  exaKernel Ax;
  exaKernel vectorWeightedNorm2;
  exaKernel vectorWeightedInnerProduct2;
  exaKernel vectorScaledAdd;
  // device vectors
  exaVector tmp_r,tmp_p,tmp_z,tmp_Ap,tmp_Ax;
};

#endif
