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

  /* x,y and z co-ordinates of mesh points */
  exaScalar *xm1 ,*ym1 ,*zm1;
  exaVector d_xm1,d_ym1,d_zm1;

  /* inv degree of a dof */
  exaScalar *rmult;
  exaVector d_rmult;

  /* global numbering of dofs */
  exaLong *glo_num;
  exaVector d_globalStart;
  exaVector d_globalIds;

  int *boundaryID;
  exaVector d_maskedIds;

  exaScalar *geom;
  exaScalar *jacobians;

  exaScalar *D,*Dt;
  exaVector d_D,d_Dt;
};

#endif
