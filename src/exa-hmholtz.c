#include "exa-hmholtz-impl.h"
//
// exaHmholtz
//
int exaHmholtzCreate(exaHmholtz *solver_,exaHandle h){
  exaDebug(h,"[hmholtzCreate]\n");
  exaMalloc(1,solver_); exaHmholtz solver=*solver_;

  exaHmholtzSetHandle(solver,&h);

  solver->Ax=NULL;
  solver->vectorWeightedNorm2=NULL;
  solver->vectorWeightedInnerProduct2=NULL;
  solver->vectorInnerProduct2=NULL;
  solver->vectorScaledAdd=NULL;
  solver->mask=NULL;

  return 0;
}

int exaHmholtzSetSettings(exaHmholtz solver,exaSettings *s){
  solver->s=*s;
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
int exaHmholtzSetHandle(exaHmholtz solver,exaHandle *h){
  solver->h=*h;
  return 0;
}

static const char *kernelDir="/kernels";
static const char *interfaceDir="/interfaces";

static int setupSettings(exaSettings s,exaHmholtz solver){
  exaHmholtzSetSettings(solver,&s);

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

static int buildKernels(exaSettings s,exaHmholtz hmhz){
  exaHandle h; exaHmholtzGetHandle(hmhz,&h);

  const char *kernelDir;
  exaSettingsGet(&kernelDir,"hmholtz::kernel_dir",s);

  char fname[BUFSIZ];
  strcpy(fname,kernelDir);
  int pathLength=strlen(kernelDir);

  strcpy(fname+pathLength,"/vector");
  exaDebug(h,"Hmholtz vector kernels=%s\n",fname);
  exaProgramCreate(h,fname,s,&hmhz->p);

  exaKernelCreate(hmhz->p,"scaledAdd",&hmhz->vectorScaledAdd);
  exaKernelCreate(hmhz->p,"weightedInnerProduct2",
    &hmhz->vectorWeightedInnerProduct2);
  exaKernelCreate(hmhz->p,"innerProduct2",
    &hmhz->vectorInnerProduct2);
  exaKernelCreate(hmhz->p,"weightedNorm2",
    &hmhz->vectorWeightedNorm2);
  exaProgramFree(hmhz->p);

  strcpy(fname+pathLength,"/mask");
  exaDebug(h,"Hmholtz mask kernels=%s\n",fname);
  exaProgramCreate(h,fname,s,&hmhz->p);
  exaKernelCreate(hmhz->p,"mask",&hmhz->mask);
  exaProgramFree(hmhz->p);

  return 0;
}


int exaHmholtzSetup(exaHmholtz hmhz,exaSettings s){
  exaHandle h; exaHmholtzGetHandle(hmhz,&h);
  exaDebug(h,"[hmholtzSetup]\n");

  setupSettings(s,hmhz);
  buildKernels(s,hmhz);

  exaDebug(h,"[/hmholtzSetup]\n");
  return 0;
}

int exaHmholtzDestroy(exaHmholtz hmhz){
  exaHandle h; exaHmholtzGetHandle(hmhz,&h);
  exaDebug(h,"[hmholtzDestroy]\n");

  exaDestroy(hmhz->mask);
  exaDestroy(hmhz->vectorScaledAdd);
  exaDestroy(hmhz->vectorWeightedInnerProduct2);
  exaDestroy(hmhz->vectorInnerProduct2);
  exaDestroy(hmhz->vectorWeightedNorm2);

  exaFree(hmhz);

  exaDebug(h,"[/hmholtzDestroy]\n");
  return 0;
}
