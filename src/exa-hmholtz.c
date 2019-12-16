#include "exa-hmholtz-impl.h"
//
// exaHmholtz
//
static const char *kernelDir="/kernels";
static char installDir[BUFSIZ];

int exaHmholtzSetup(exaSettings s,exaHmholtz solver){
  solver->s=s;

  exaHandle h; exaHmholtzGetHandle(solver,&h);

  char *ret=getenv("EXA_HMHOLTZ_INSTALL_DIR");
  if(ret==NULL){
    const char *home=getenv("HOME");
    const char *suffix="/local/exaHmholtz/";
    strcpy(installDir,home);
    strcpy(installDir+strlen(home),suffix);
  } else
    strcpy(installDir,ret);

  exaDebug(h,"Hmholtz install dir=%s\n",installDir);

  char fname[BUFSIZ]; strcpy(fname,installDir);
  strcpy(fname+strlen(installDir),"/kernels/vector");
  exaDebug(h,"Hmholtz vector kernel=%s\n",fname);

  exaProgramCreate(h,fname,s,&solver->p);
  exaKernelCreate(solver->p,"scaledAdd",&solver->vectorScaledAdd);
  exaKernelCreate(solver->p,"weightedInnerProduct2",
    &solver->vectorWeightedInnerProduct2);
  exaKernelCreate(solver->p,"innerProduct2",
    &solver->vectorInnerProduct2);
  exaKernelCreate(solver->p,"weightedNorm2",
    &solver->vectorWeightedNorm2);
  exaProgramFree(solver->p);

  return 0;
}

int exaHmholtzCreate(exaHandle h,exaSettings s,exaHmholtz *solver_){
  exaMalloc(1,solver_);

  exaHmholtz solver=*solver_;
  solver->h=h;

  exaHmholtzSetup(s,solver);

  return 0;
}

int exaHmholtzGetHandle(exaHmholtz solver,exaHandle *h){
  *h=solver->h;
}

int exaHmholtzDestroy(exaHmholtz solver){
  exaDestroy(solver->vectorScaledAdd);
  exaDestroy(solver->vectorWeightedInnerProduct2);
  exaDestroy(solver->vectorInnerProduct2);
  exaDestroy(solver->vectorWeightedNorm2);

  exaFree(solver);

  return 0;
}
