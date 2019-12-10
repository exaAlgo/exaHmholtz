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
  exaVector out; exaVectorCreate(h,size,&out);

  exaKernelRun(hz->vectorWeightedNorm2,getExaUInt(size),weights,vec,
    out);

  exaDestroy(out);

  return 0;
}
