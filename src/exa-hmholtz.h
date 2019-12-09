#ifndef _EXA_HMHOLTZ_H_
#define _EXA_HMHOLTZ_H_

#include <exa.h>
//
// Pointer types in exaHmholtz
//
typedef struct exaHmholtz_private *exaHmholtz;
//
// exaHmholtz
//
int exaHmholtzCreate(exaHandle h,exaHmholtz *solver);
int exaHmholtzSetup (exaHmholtz solver);
int exaHmholtzDestroy(exaHmholtz solver);
//
// vector operations
//
int exaVectorWeightedNorm2(exaVector v,exaHmholtz hz);
//
// PCG
//
int exaHmholtzCG(exaVector x,exaVector f,exaScalar lambda,exaScalar tol,
  int maxit,exaHmholtz hz);
#endif
