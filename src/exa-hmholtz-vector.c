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

exaScalar exaVectorInnerProduct2(exaVector x,exaVector y,
  exaHmholtz hz)
{
  exaUInt size=exaVectorGetSize(x);
  assert(size==(exaUInt)exaVectorGetSize(y));

  exaHandle h; exaHmholtzGetHandle(hz,&h);

  exaUInt blockSize;
  int err=exaSettingsGet(&blockSize,"defines::p_blockSize",hz->s);
  if(err!=0){
    fprintf(stderr,"Could not find blockSize.\n");
    exit(0);
  }
  exaUInt nBlocks=(size+blockSize-1)/blockSize;
  exaVector out; exaVectorCreate(h,nBlocks,exaScalar_t,&out);

  exaKernelRun(hz->vectorInnerProduct2,getExaUInt(size),x,y,out);

  exaScalar *result; exaVectorRead(out,(void**)&result);

  exaScalar t=0;
  for(int i=0;i<nBlocks;i++)
    t+=result[i];

  exaGop(h,&t,1,exaScalar_t,exaAddOp);

  exaDestroy(out);

  return t;
}

exaScalar exaVectorWeightedInnerProduct2(exaVector weights,
  exaVector x,exaVector y,exaHmholtz hz)
{
  exaHandle h; exaHmholtzGetHandle(hz,&h);
  if(exaRank(h)==0)
    exaDebug(h,"[exaVectorWeightedInnerProduct2]\n");

  exaUInt size=exaVectorGetSize(x);
  assert(size==(exaUInt)exaVectorGetSize(y));
  assert(size==(exaUInt)exaVectorGetSize(weights));

  exaUInt blockSize;
  int err=exaSettingsGet(&blockSize,"defines::p_blockSize",hz->s);
  if(err!=0){
    fprintf(stderr,"Could not find blockSize.\n");
    exit(0);
  }
  exaUInt nBlocks=(size+blockSize-1)/blockSize;
  exaVector out; exaVectorCreate(h,nBlocks,exaScalar_t,&out);

  if(exaRank(h)==0) exaDebug(h,"size=%u nBlocks=%u\n",size,nBlocks);

  exaKernelRun(hz->vectorWeightedInnerProduct2,getExaUInt(size),
    weights,x,y,out);

  exaScalar *result; exaVectorRead(out,(void**)&result);

  exaScalar t=0;
  for(int i=0;i<nBlocks;i++){
    t+=result[i];
  }
  exaDestroy(out);

  exaGop(h,&t,1,exaScalar_t,exaAddOp);

  if(exaRank(h)==0)
    exaDebug(h,"[/exaVectorWeightedInnerProduct2]\n");

  return t;
}

exaScalar exaVectorNorm2(exaVector vec,exaHmholtz hz){
  exaScalar inner=exaVectorInnerProduct2(vec,vec,hz);
  return inner;
}

exaScalar exaVectorWeightedNorm2(exaVector weights,exaVector vec,
  exaHmholtz hz)
{
  exaScalar wInner=exaVectorWeightedInnerProduct2(weights,vec,vec,hz);
  return wInner;
}
