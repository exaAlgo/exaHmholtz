#include "exa-hmholtz-impl.h"
//
// exaHmholtz Gather Scatter
//
int exaHmholtzGatherScatter(exaVector in,exaMesh mesh,exaHmholtz hmhz){
  exaGS dGs=mesh->dGs;

  exaVector indices; exaGSGetIndices(dGs,&indices);
  exaVector offsets; exaGSGetOffsets(dGs,&offsets);
  exaUInt size=exaVectorGetSize(offsets)-1;

  exaKernelRun(hmhz->gatherScatter,getExaUInt(size),offsets,indices,in);
  return 0;
}
