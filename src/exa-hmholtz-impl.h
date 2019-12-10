#ifndef _EXA_HMHOLTZ_IMPL_H_
#define _EXA_HMHOLTZ_IMPL_H_

#include "exa-hmholtz.h"
#include "exa-memory.h"

struct exaHmholtz_private{
  exaHandle h;
  exaSettings s;
  // program
  exaProgram p;
  // kernels
  exaKernel Ax;
  exaKernel vectorWeightedNorm2;
  exaKernel vectorScaledAdd;
  // device vectors
  exaVector tmp;
};

#endif
