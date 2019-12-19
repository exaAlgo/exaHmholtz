#include "exa-hmholtz-impl.h"

int exaApplyMask(exaVector vec,exaVector maskIds,exaHmholtz hz){
  exaUInt size=exaVectorGetSize(maskIds);

  exaKernelRun(hz->mask,getExaUInt(size),maskIds,vec);

  return 0;
}
