#include "exa-hmholtz-impl.h"

int exaHmholtzGeneralCG(exaVector x,
  int (*getAx)(exaVector,exaVector,exaHmholtz),exaVector b,
  exaScalar tol,int maxit,exaHmholtz hz)
{
  exaInt size=exaVectorGetSize(x);
  assert(size==exaVectorGetSize(b));

  exaVector r =hz->tmp_1;
  exaVector p =hz->tmp_2;
  exaVector Ap=hz->tmp_3;
  exaVector Ax=hz->tmp_4;

  exaHandle h; exaHmholtzGetHandle(hz,&h);
  exaVectorCreate(h,size,&r );
  exaVectorCreate(h,size,&p );
  exaVectorCreate(h,size,&Ap);
  exaVectorCreate(h,size,&Ax);

  exaScalar normB=exaVectorInnerProduct2(b,b,hz);
  exaScalar TOL=max(tol*tol*normB,tol*tol);
  //printf("normB=%.2e TOL=%.2e\n",normB,TOL);

  exaVectorScaledAdd(1.0,b,0.0,r,hz);
  exaVectorScaledAdd(1.0,b,0.0,p,hz);
  exaScalar rdotr=normB;

  exaScalar alpha,beta,rdotr0;

  int nIter=0;
  while(nIter<maxit && rdotr>TOL){
    //Calculate Ap=A*p
    getAx(p,Ap,hz);

    exaScalar pAp=exaVectorInnerProduct2(p,Ap,hz);
    alpha=rdotr/pAp;

    exaVectorScaledAdd(alpha     ,p ,1.0,x,hz);
    exaVectorScaledAdd(-1.0*alpha,Ap,1.0,r,hz);

    rdotr0=rdotr;
    rdotr=exaVectorInnerProduct2(r,r,hz);
    beta=rdotr/rdotr0;

    exaVectorScaledAdd(1.0,r,beta,p,hz);

    nIter++;
  }

  exaDestroy(r );
  exaDestroy(p );
  exaDestroy(Ap);
  exaDestroy(Ax);

  return nIter;
}

int exaHmholtzCG(exaVector x,exaVector b,exaScalar tol,int maxit,
  exaHmholtz hz)
{
  exaInt size=exaVectorGetSize(x);
  assert(size==exaVectorGetSize(b));

  exaVector r =hz->tmp_1;
  exaVector p =hz->tmp_2;
  exaVector Ap=hz->tmp_3;
  exaVector Ax=hz->tmp_4;

  exaHandle h; exaHmholtzGetHandle(hz,&h);
  exaVectorCreate(h,size,&r );
  exaVectorCreate(h,size,&p );
  exaVectorCreate(h,size,&Ap);
  exaVectorCreate(h,size,&Ax);

  //TODO set from mesh
  exaVector rmult;
  exaScalar normB=exaVectorWeightedNorm2(rmult,b,hz);
  exaScalar TOL=max(tol*tol*normB,tol*tol);

  exaVectorScaledAdd(1.0,b,0.0,r,hz);
  exaVectorScaledAdd(1.0,b,0.0,p,hz);
  exaScalar rdotr=normB;

  exaScalar alpha,beta,rdotr0;

  int nIter=0;
  while(nIter<maxit && rdotr>TOL){
    //TODO: Calculate Ap=A*p

    exaScalar pAp=exaVectorInnerProduct2(p,Ap,hz);
    alpha=rdotr/pAp;

    //dssum Ap

    exaVectorScaledAdd(alpha ,p ,1.0,x,hz);
    exaVectorScaledAdd(-alpha,Ap,1.0,r,hz);

    rdotr0=rdotr;
    rdotr=exaVectorWeightedNorm2(rmult,r,hz);
    beta=rdotr/rdotr0;

    exaVectorScaledAdd(1.0,r,beta,p,hz);

    nIter++;
  }

  exaDestroy(r );
  exaDestroy(p );
  exaDestroy(Ap);
  exaDestroy(Ax);

  return nIter;
}
