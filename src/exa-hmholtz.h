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
int exaHmholtzCreate(exaHandle h,exaSettings s,exaHmholtz *solver);
int exaHmholtzGetHandle(exaHmholtz solver,exaHandle *h);
int exaHmholtzSetup (exaSettings s,exaHmholtz solver);
int exaHmholtzDestroy(exaHmholtz solver);
//
// vector operations
//
int exaVectorScaledAdd(exaScalar alpha,exaVector x,exaScalar beta,
  exaVector y,exaHmholtz hz);
exaScalar exaVectorWeightedInnerProduct2(exaVector weights,
  exaVector x,exaVector y,exaHmholtz hz);
exaScalar exaVectorWeightedNorm2(exaVector weights,exaVector vec,
  exaHmholtz hz);
//
// PCG
//
int exaHmholtzCG(exaVector x,exaVector f,exaVector h1,
  exaScalar lambda,exaScalar tol,int maxit,exaHmholtz hz);
#endif
