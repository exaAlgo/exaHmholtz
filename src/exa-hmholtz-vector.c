#include "exa-hmholtz-impl.h"

int exaVectorScaledAdd(exaScalar alpha,exaVector vec1,exaScalar beta,
  exaVector vec2,exaHmholtz hz)
{
  exaUInt size=exaVectorGetSize(vec1);
  assert(size==(exaUInt)exaVectorGetSize(vec2));

  exaKernelRun(hz->vectorScaledAdd,getExaUInt(size),
    getExaScalar(alpha),vec1,getExaScalar(beta),vec2);

  return 0;
}

exaScalar exaVectorWeightedNorm2(exaVector weights,exaVector vec,
  exaHmholtz hz)
{
  exaUInt size=exaVectorGetSize(vec);
  assert(size==(exaUInt)exaVectorGetSize(weights));

  exaHandle h; exaHmholtzGetHandle(hz,&h);

  exaUInt blockSize;
  exaSettingsGetNative(&blockSize,"defines::p_blockSize",hz->s);
  exaUInt nBlocks=(size+blockSize-1)/blockSize;

  exaVector out; exaVectorCreate(h,nBlocks,&out);

  exaKernelRun(hz->vectorWeightedNorm2,getExaUInt(size),weights,vec,
    out);

  exaScalar *result; exaCalloc(nBlocks,&result);
  exaVectorRead(out,result);

  exaScalar t=0;
  for(int i=0;i<nBlocks;i++)
    t+=result[i];

  exaGop(h,&t,1,exaScalar_t,exaAddOp);

  exaFree(result);
  exaDestroy(out);

  return t;
}
