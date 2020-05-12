#include "exa-hmholtz-impl.h"

int exaHmholtzCG(exaVector x,exaVector b,exaMesh mesh,exaScalar tol,
  int maxit,int verbose,exaHmholtz hz)
{
  exaInt size=exaVectorGetSize(x);
  assert(size==exaVectorGetSize(b));
  exaScalar *out; exaCalloc(size,&out);
  exaVectorWrite(x,out);

  exaVector r =hz->tmp_1;
  exaVector p =hz->tmp_2;
  exaVector Ap=hz->tmp_3;
  exaVector Ax=hz->tmp_4;

  exaHandle h; exaHmholtzGetHandle(hz,&h);
  exaVectorCreate(h,size,exaScalar_t,&r );
  exaVectorCreate(h,size,exaScalar_t,&p );
  exaVectorCreate(h,size,exaScalar_t,&Ap);
  exaVectorCreate(h,size,exaScalar_t,&Ax);

  exaVectorScaledAdd(1.0,b,0.0,r,hz);
  exaVectorScaledAdd(1.0,b,0.0,p,hz);

  exaScalar rdotr=exaVectorWeightedNorm2(mesh->d_rmult,r,hz);
  if(verbose)
    printf("cg 0: %lf\n",rdotr);

  exaScalar TOL=max(tol*tol*rdotr,tol*tol);

  exaScalar pAp,alpha,beta,rdotr0;

  int nIter=0;
  while(nIter<maxit && rdotr>TOL){
    exaHmholtzOperator(p,Ap,mesh);

    pAp=exaVectorInnerProduct2(p,Ap,hz);

    //TODO: dssum for multi-element cases
    //exaDsSum(Ap,mesh,hmhz);
    exaApplyMask(Ap,mesh->d_maskIds,hz);

    alpha=rdotr/pAp;

    exaVectorScaledAdd(alpha     ,p ,1.0,x,hz);
    exaVectorScaledAdd(-1.0*alpha,Ap,1.0,r,hz);

    rdotr0=rdotr;
    rdotr=exaVectorWeightedNorm2(mesh->d_rmult,r,hz);
    beta=rdotr/rdotr0;

    if(verbose)
      printf("niter=%d r0=%.5e r1=%.2e alpha=%.5e pap=%.5e\n",
        nIter,rdotr0,rdotr,alpha,pAp);

    exaVectorScaledAdd(1.0,r,beta,p,hz);

    nIter++;
  }

  exaDestroy(r );
  exaDestroy(p );
  exaDestroy(Ap);
  exaDestroy(Ax);

  exaFree(out);

  return nIter;
}

int exaHmholtzCGGeneral(exaVector x,
  int (*getAx)(exaVector,exaVector,exaHmholtz),exaVector b,
  exaScalar tol,int maxit,int verbose,exaHmholtz hz)
{
  exaInt size=exaVectorGetSize(x);
  assert(size==exaVectorGetSize(b));

  exaVector r =hz->tmp_1;
  exaVector p =hz->tmp_2;
  exaVector Ap=hz->tmp_3;
  exaVector Ax=hz->tmp_4;

  exaHandle h; exaHmholtzGetHandle(hz,&h);
  exaVectorCreate(h,size,exaScalar_t,&r );
  exaVectorCreate(h,size,exaScalar_t,&p );
  exaVectorCreate(h,size,exaScalar_t,&Ap);
  exaVectorCreate(h,size,exaScalar_t,&Ax);

  exaScalar normB=exaVectorNorm2(b,hz);
  exaScalar TOL=max(tol*tol*normB,tol*tol);

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
    rdotr=exaVectorNorm2(r,hz);
    beta=rdotr/rdotr0;

    if(verbose)
      printf("niter=%d r0=%.2e r1=%.2e alpha=%.2e beta=%.2e"
        " pap=%.2e\n",nIter,rdotr0,rdotr,alpha,beta,pAp);

    exaVectorScaledAdd(1.0,r,beta,p,hz);

    nIter++;
  }

  exaDestroy(r );
  exaDestroy(p );
  exaDestroy(Ap);
  exaDestroy(Ax);

  return nIter;
}
