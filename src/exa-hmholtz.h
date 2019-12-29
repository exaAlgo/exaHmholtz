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
int exaHmholtzSetup(exaHmholtz solver);
int exaHmholtzDestroy(exaHmholtz solver);
//
// exaMesh
//
int exaMeshRead(exaMesh *mesh,const char *meshName,
  const char *interface,exaSettings s);
int exaMeshCreate(exaMesh *mesh,exaSettings s);

int exaMeshGetHandle(exaMesh mesh,exaHandle *h);
int exaMeshSetHandle(exaMesh mesh,exaHandle *h);

exaInt exaMeshGetElements(exaMesh mesh);
int exaMeshSetElements(exaMesh mesh,exaInt nelem);

int exaMeshGet1DDofs(exaMesh mesh);
int exaMeshSet1DDofs(exaMesh mesh,int nx1);

int exaMeshGetDim(exaMesh mesh);
int exaMeshSetDim(exaMesh mesh,int dim);

int exaMeshGetDofsPerElement(exaMesh mesh);
int exaMeshGetLocalDofs(exaMesh mesh);
int exaMeshGetNGeom(exaMesh mesh);

int exaMeshSetup(exaMesh mesh,exaSettings s);

int exaMeshDestroy(exaMesh mesh);
//
// Hmholtz Operator
//
int exaHmholtzOperator(exaVector p,exaVector Ap,exaMesh mesh,
  exaHmholtz hz);
//
// Mask
//
int exaApplyMask(exaVector vec,exaVector maskIds,exaHmholtz hz);
//
// Vector Operations
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
