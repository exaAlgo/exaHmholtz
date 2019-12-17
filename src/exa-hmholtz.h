#ifndef _EXA_HMHOLTZ_H_
#define _EXA_HMHOLTZ_H_

#include <exa.h>
//
// Pointer types in exaHmholtz
//
typedef struct exaHmholtz_private *exaHmholtz;
typedef struct exaMesh_private *exaMesh;
//
// exaHmholtz
//
int exaHmholtzCreate(exaHandle h,exaSettings s,exaHmholtz *solver);
int exaHmholtzGetHandle(exaHmholtz solver,exaHandle *h);
int exaHmholtzGetSettings(exaHmholtz solver,exaSettings *s);
int exaHmholtzSetup (exaSettings s,exaHmholtz solver);
int exaHmholtzDestroy(exaHmholtz solver);
//
// vector operations
//
int exaVectorScaledAdd(exaScalar alpha,exaVector x,exaScalar beta,
  exaVector y,exaHmholtz hz);

exaScalar exaVectorInnerProduct2(exaVector x,exaVector y,
  exaHmholtz hz);
exaScalar exaVectorWeightedInnerProduct2(exaVector weights,
  exaVector x,exaVector y,exaHmholtz hz);

exaScalar exaVectorNorm2(exaVector vec,exaHmholtz hz);
exaScalar exaVectorWeightedNorm2(exaVector weights,exaVector vec,
  exaHmholtz hz);
//
// CG and PCG
//
int exaHmholtzCGGeneral(exaVector x,
  int (*getAx)(exaVector,exaVector,exaHmholtz),exaVector b,
  exaScalar tol,int maxit,int verbose,exaHmholtz hz);
#if 0
int exaHmholtzCG(exaVector x,
  int (*getAx)(exaVector,exaVector,exaHmholtz),exaVector b,
  exaVector weights,exaScalar tol,int maxit,exaHmholtz hz);
#endif

#endif
