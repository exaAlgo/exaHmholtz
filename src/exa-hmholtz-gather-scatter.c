#include "exa-hmholtz-impl.h"
//
// exaHmholtz Gather Scatter
//
int exaHmholtzGatherScatter(exaVector in,exaMesh mesh,exaHmholtz hmhz){
  exaGS dGs=mesh->dGs;
  exaVector indices; exaGSGetIndices(dGs,&indices);
  exaVector offsets; exaGSGetOffsets(dGs,&offsets);

  exaHandle h=hmhz->h; exaUInt size=exaVectorGetSize(offsets)-1;
  exaVector q; exaVectorCreate(h,size,exaScalar_t,&q);

  exaKernelRun(hmhz->gather,getExaUInt(size),offsets,indices,in,q);

  exaScalar *qh; exaVectorRead(q,(void**)&qh);
  exaBuffer buf; exaBufferCreate(&buf,1024);
  exaGSOp((void*)qh,exaScalar_t,exaAddOp,0,dGs,buf);
  exaVectorWrite(q,(void*)qh);

  exaKernelRun(hmhz->scatter,getExaUInt(size),offsets,indices,in,q);

  exaDestroy(q  );
  exaDestroy(buf);

  return 0;
}
