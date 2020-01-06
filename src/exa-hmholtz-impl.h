#ifndef _EXA_HMHOLTZ_IMPL_H_
#define _EXA_HMHOLTZ_IMPL_H_

#include "exa-hmholtz.h"
#include "exa-memory.h"

#include <math.h>

#define max(a,b) ((a)>(b)) ? (a) : (b)

struct exaHmholtz_private{
  // exaHandle
  exaHandle h;
  // exaSettings
  exaSettings s;
  // program
  exaProgram p;
  // kernels
  exaKernel Ax;
  exaKernel vectorWeightedNorm2;
  exaKernel vectorWeightedInnerProduct2;
  exaKernel vectorInnerProduct2;
  exaKernel vectorScaledAdd;
  exaKernel mask;
  exaKernel hmholtzAx;
  // device vectors
  exaVector tmp_1,tmp_2,tmp_3,tmp_4,tmp_5;
};

struct exaMesh_private{
  exaHandle h;
  int isSetup;

  int ndim,nx1;
  exaInt nelt;
  exaInt nelv;

  /* x,y and z co-ordinates of mesh points */
  exaScalar *xm1 ,*ym1 ,*zm1;

  /* inv degree of a dof */
  exaScalar *rmult;
  exaVector d_rmult;

  /* offsets for geometric factors */
  exaUInt G00ID,G01ID,G02ID,G11ID,G12ID,G22ID,GWJID;

  /* global numbering of dofs */
  exaLong *glo_num;
  exaVector d_globalStart;
  exaVector d_globalIds;

  /* gather scatter setup */
  exaGS gs; exaBuffer buf;

  /* boundary data */
  int *boundaryID;
  exaScalar *mask;
  exaArray maskIds;
  exaVector d_maskIds;

  exaScalar *geom;
  exaVector d_geom;

  exaScalar *D;
  exaVector d_D;
};

#endif
