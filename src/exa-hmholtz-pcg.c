#include "exa-hmholtz-impl.h"

int exaHmholtzCG(exaVector x,
  int (*getAx)(exaVector,exaVector,exaHmholtz),exaVector b,
  exaVector weights,exaScalar tol,int maxit,exaHmholtz hz)
{
  exaInt size=exaVectorGetSize(x);
  assert(size==exaVectorGetSize(b));
  assert(size==exaVectorGetSize(weights));

  exaVector r =hz->tmp_r;
  exaVector p =hz->tmp_p;
  exaVector Ap=hz->tmp_Ap;
  exaVector Ax=hz->tmp_Ax;

  exaHandle h; exaHmholtzGetHandle(hz,&h);
  exaVectorCreate(h,size,&r );
  exaVectorCreate(h,size,&p );
  exaVectorCreate(h,size,&Ap);
  exaVectorCreate(h,size,&Ax);

  exaScalar normB=exaVectorWeightedNorm2(weights,b,hz);
  exaScalar TOL=max(tol*tol*normB,tol*tol);

  exaVectorScaledAdd(1.0,b,0.0,r,hz);
  exaVectorScaledAdd(1.0,b,0.0,p,hz);
  exaScalar rdotr=normB;

  exaScalar alpha,beta,rdotr0;

  int nIter=0;
  while(nIter<maxit && rdotr>TOL){
    //Calculate Ap=A*p
    getAx(p,Ap,hz);

    exaScalar pAp=exaVectorWeightedInnerProduct2(weights,p,Ap,hz);
    alpha=rdotr/pAp;

    exaVectorScaledAdd(alpha     ,p ,1.0,x,hz);
    exaVectorScaledAdd(-1.0*alpha,Ap,1.0,r,hz);

    rdotr0=rdotr;
    rdotr=exaVectorWeightedNorm2(weights,r,hz);
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
