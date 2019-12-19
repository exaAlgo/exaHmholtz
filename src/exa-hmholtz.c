#include "exa-hmholtz-impl.h"
//
// exaHmholtz
//
static const char *kernelDir="/kernels";
static const char *interfaceDir="/interfaces";

int exaHmholtzSetup(exaSettings s,exaHmholtz solver){
  solver->s=s;

  exaHandle h; exaHmholtzGetHandle(solver,&h);

  char installDir[BUFSIZ];
  char *ret=getenv("EXA_HMHOLTZ_DIR");
  if(ret==NULL){
    const char *home=getenv("HOME");
    const char *suffix="/local/exaHmholtz/";
    strcpy(installDir,home);
    strcpy(installDir+strlen(home),suffix);
  } else
    strcpy(installDir,ret);
  exaDebug(h,"Hmholtz install dir=%s\n",installDir);
  exaSettingsSet("hmholtz::install_dir",getExaStr(installDir),s);

  char fname[BUFSIZ]; strcpy(fname,installDir);
  strcpy(fname+strlen(installDir),interfaceDir);
  exaSettingsSet("hmholtz::interface_dir",getExaStr(fname),s);

  strcpy(fname,installDir);
  strcpy(fname+strlen(installDir),kernelDir);
  exaSettingsSet("hmholtz::kernel_dir",getExaStr(fname),s);

  strcpy(fname+strlen(installDir)+strlen(kernelDir),"/vector");
  exaDebug(h,"Hmholtz vector kernels=%s\n",fname);
  exaProgramCreate(h,fname,s,&solver->p);

  exaKernelCreate(solver->p,"scaledAdd",&solver->vectorScaledAdd);
  exaKernelCreate(solver->p,"weightedInnerProduct2",
    &solver->vectorWeightedInnerProduct2);
  exaKernelCreate(solver->p,"innerProduct2",
    &solver->vectorInnerProduct2);
  exaKernelCreate(solver->p,"weightedNorm2",
    &solver->vectorWeightedNorm2);
  exaProgramFree(solver->p);

  strcpy(fname+strlen(installDir)+strlen(kernelDir),"/mask");
  exaDebug(h,"Hmholtz mask kernels=%s\n",fname);
  exaProgramCreate(h,fname,s,&solver->p);
  exaKernelCreate(solver->p,"mask",&solver->mask);
  exaProgramFree(solver->p);

  ret=getenv("EXA_NEK5000_DIR");
  if(ret==NULL){
    const char *home=getenv("HOME");
    const char *suffix="/local/nek5000/";
    strcpy(installDir,home);
    strcpy(installDir+strlen(home),suffix);
  } else
    strcpy(installDir,ret);
  exaDebug(h,"Nek5000 install dir=%s\n",installDir);
  exaSettingsSet("nek5000::install_dir",getExaStr(installDir),s);

  return 0;
}

int exaHmholtzCreate(exaHandle h,exaSettings s,exaHmholtz *solver_){
  exaMalloc(1,solver_);

  exaHmholtz solver=*solver_;
  solver->h=h;

  exaHmholtzSetup(s,solver);

  return 0;
}

int exaHmholtzGetSettings(exaHmholtz solver,exaSettings *s){
  *s=solver->s;
  return 0;
}

int exaHmholtzGetHandle(exaHmholtz solver,exaHandle *h){
  *h=solver->h;
  return 0;
}

int exaHmholtzDestroy(exaHmholtz solver){
  exaDestroy(solver->mask);
  exaDestroy(solver->vectorScaledAdd);
  exaDestroy(solver->vectorWeightedInnerProduct2);
  exaDestroy(solver->vectorInnerProduct2);
  exaDestroy(solver->vectorWeightedNorm2);

  exaFree(solver);

  return 0;
}
