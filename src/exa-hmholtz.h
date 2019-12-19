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
int exaHmholtzOperator(exaVector p,exaVector Ap,exaMesh mesh,
  exaHmholtz hz);
int exaHmholtzDestroy(exaHmholtz solver);
//
// exaMesh
//
exaInt exaMeshGetElements(exaMesh mesh);
int exaMeshGet1DDofs(exaMesh mesh);
int exaMeshGetElementDofs(exaMesh mesh);
int exaMeshGetLocalDofs(exaMesh mesh);
int exaMeshSetup(exaMesh mesh,exaHmholtz hmhz);
int exaMeshFinalize(exaMesh mesh);
//
// mask
//
int exaApplyMask(exaVector vec,exaVector maskIds,exaHmholtz hz);
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
int exaHmholtzCG(exaVector x,exaVector b,exaMesh mesh,
  exaScalar tol,int maxit,int verbose,exaHmholtz hz);

#endif
