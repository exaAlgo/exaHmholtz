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
  exaKernel vectorInnerProduct2;
  exaKernel vectorScaledAdd;
  // device vectors
  exaVector tmp_1,tmp_2,tmp_3,tmp_4,tmp_5;
};

struct exaMesh_private{
  int ndim,nx1;
  exaInt nelt;
  exaInt nelv;
  exaInt lelt;

  exaScalar *xc ,*yc ,*zc;
  exaScalar *xm1,*ym1,*zm1;

  exaLong *glo_num;

  char *cbc;
  int *boundaryID;

  exaScalar *geom;
  exaScalar *jacobians;
  exaScalar *D,*Dt;
};

#endif
