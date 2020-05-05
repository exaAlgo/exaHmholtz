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
int exaHmholtzCreate(exaHmholtz *solver,exaHandle h);
int exaHmholtzGetHandle(exaHmholtz solver,exaHandle *h);
int exaHmholtzSetHandle(exaHmholtz solver,exaHandle *h);
int exaHmholtzGetSettings(exaHmholtz solver,exaSettings *s);
int exaHmholtzSetup(exaHmholtz solver,exaSettings s);
int exaHmholtzDestroy(exaHmholtz solver);
//
// exaMesh
//
int exaMeshCreate(exaMesh *mesh,const char *meshFile,exaHandle h);
int exaMeshGetHandle(exaMesh mesh,exaHandle *h);
int exaMeshSetHandle(exaMesh mesh,exaHandle *h);

exaInt exaMeshGetNElements(exaMesh mesh);
int exaMeshSetNElements(exaMesh mesh,exaInt nelem);

int exaMeshGet1DDofs(exaMesh mesh);
int exaMeshSet1DDofs(exaMesh mesh,int nx1);

int exaMeshGetDim(exaMesh mesh);
int exaMeshSetDim(exaMesh mesh,int dim);

int exaMeshSetXcoords(exaMesh mesh,exaScalar *xc);
exaScalar *exaMeshGetXcoords(exaMesh mesh);
int exaMeshSetYcoords(exaMesh mesh,exaScalar *yc);
exaScalar *exaMeshGetYcoords(exaMesh mesh);
int exaMeshSetZcoords(exaMesh mesh,exaScalar *zc);
exaScalar *exaMeshGetZcoords(exaMesh mesh);

int exaMeshSetGlobalNumbering(exaMesh mesh,exaLong *gloNum);
exaLong *exaMeshGetGlobalNumbering(exaMesh mesh);

int exaMeshSetMask(exaMesh mesh,exaScalar *mask);
exaScalar *exaMeshGetMask(exaMesh mesh);

int exaMeshSetGeometricFactors(exaMesh mesh,exaScalar *geom);
exaScalar *exaMeshGetGeometricFactors(exaMesh mesh);

int exaMeshSetDerivativeMatrix(exaMesh mesh,exaScalar *D);
exaScalar *exaMeshGetDerivativeMatrix(exaMesh mesh);

int exaMeshGetDofsPerElement(exaMesh mesh);
int exaMeshGetLocalDofs(exaMesh mesh);
int exaMeshGetNGeom(exaMesh mesh);

int exaMeshSetup(exaMesh mesh,exaSettings s);

int exaMeshDestroy(exaMesh mesh);
//
// Hmholtz Operator
//
int exaHmholtzOperator(exaVector p,exaVector Ap,exaMesh mesh);
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
