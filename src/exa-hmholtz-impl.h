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
  exaKernel gatherScatter;
  exaKernel gather;
  exaKernel scatter;
  // device vectors
  exaVector tmp_1,tmp_2,tmp_3,tmp_4,tmp_5;
};

struct exaMesh_private{
  exaHandle h;

  /* offsets for geometric factors */
  exaUInt G00ID,G01ID,G02ID,G11ID,G12ID,G22ID,GWJID;

  int ndim,nx1;
  exaInt nelt;
  exaInt nelv;

  /* x,y and z co-ordinates of elements */
  exaScalar *xc,*yc,*zc;
  /* x,y and z co-ordinates of mesh 1 */
  exaScalar *xm1,*ym1,*zm1;

  /* global numbering of dofs */
  exaLong *globalIds;
  exaVector d_globalStart;
  exaVector d_globalIds;

  /* boundary data */
  exaScalar *mask;
  exaArray maskIds;
  exaVector d_maskIds;

  exaScalar *geom;
  exaVector d_geom;

  exaScalar *D,*Dt;
  exaVector d_D,d_Dt;

  /* gather scatter setup */
  exaGS gs; exaBuffer buf;
  exaGS dGs;

  /* inv degree of a dof */
  exaScalar *rmult;
  exaVector d_rmult;

  exaKernel hmholtzAx;
};

#endif
