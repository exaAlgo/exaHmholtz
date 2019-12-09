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

int exaVectorWeightedNorm2(exaVector v,exaHmholtz hz){
  return 0;
}
