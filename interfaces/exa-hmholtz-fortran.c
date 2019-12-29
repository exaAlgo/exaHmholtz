// Fortran interface
#include <exa-hmholtz-impl.h>
#include <exa-hmholtz-fortran.h>
#include <stdlib.h>
#include <string.h>

static exaHmholtz *hmholtzDict=NULL;
static int hmholtzCurrent=0;
static int hmholtzActive=0;
static int hmholtzMax=0;

#define fExaHmholtzCreate\
  EXA_FORTRAN_NAME(exahmholtzcreate,EXAHMHOLTZCREATE)
void fExaHmholtzCreate(exaFortranHandle *exa,exaFortranSettings *s,
  exaFortranHmholtz *hmhz,int *err)
{
  if(hmholtzCurrent==hmholtzMax)
    hmholtzMax+=hmholtzMax/2+1,exaRealloc(hmholtzMax,&hmholtzDict);

  *err=exaHmholtzCreate(exaHandleF2C(*exa),exaSettingsF2C(*s),
    &hmholtzDict[hmholtzCurrent]);

  if(*err==0)
    *exa=hmholtzCurrent++,hmholtzActive++;
}

#define fExaHmholtzDestroy\
  EXA_FORTRAN_NAME(exahmholtzdestroy,EXAHMHOLTZDESTROY)
void fExaHmholtzDestroy(exaFortranHmholtz *hmhz,int *err){
  *err=exaHmholtzDestroy(exaHmholtzF2C(*hmhz));

  if(*err==0){
    hmholtzActive--;
    if(hmholtzActive==0)
      exaFree(hmholtzDict),hmholtzCurrent=0,hmholtzMax=0;
  }
}

static exaMesh *meshDict=NULL;
static int meshCurrent=0;
static int meshActive=0;
static int meshMax=0;

#define fExaMeshCreate\
  EXA_FORTRAN_NAME(exameshcreate,EXAMESHCREATE)
void fExaMeshCreate(exaFortranMesh *mesh,exaFortranSettings *s,
  int *err)
{
  if(meshCurrent==meshMax)
    meshMax+=meshMax/2+1,exaRealloc(meshMax,&meshDict);

  *err=exaMeshCreate(&meshDict[meshCurrent],exaSettingsF2C(*s));

  if(*err==0)
    *mesh=meshCurrent++,meshActive++;
}

#define fExaMeshGetElements\
  EXA_FORTRAN_NAME(exameshgetelements,EXAMESHGETELEMENTS)
void fExaMeshGetElements(exaFortranMesh *mesh,int *nelem,int *err){
  *nelem=exaMeshGetElements(exaMeshF2C(*mesh));
  *err=0;
}

#define fExaMeshSetElements\
  EXA_FORTRAN_NAME(exameshsetelements,EXAMESHSETELEMENTS)
void fExaMeshSetElements(exaFortranMesh *mesh,int nelem,int *err){
  *err=exaMeshSetElements(exaMeshF2C(*mesh),nelem);
}

#define fExaMeshGet1DDofs\
  EXA_FORTRAN_NAME(exameshget1ddofs,EXAMESHGET1DDOFS)
void fExaMeshGet1DDofs(exaFortranMesh *mesh,int *nx1,int *err){
  *nx1=exaMeshGet1DDofs(exaMeshF2C(*mesh));
  *err=0;
}

#define fExaMeshSet1DDofs\
  EXA_FORTRAN_NAME(exameshset1ddofs,EXAMESHSET1DDOFS)
void fExaMeshSet1DDofs(exaFortranMesh *mesh,int nx1,int *err){
  *err=exaMeshSet1DDofs(exaMeshF2C(*mesh),nx1);
}

#define fExaMeshGetDim\
  EXA_FORTRAN_NAME(exameshgetdim,EXAMESHGETDIM)
void fExaMeshGetDim(exaFortranMesh *mesh,int *ndim,int *err){
  *ndim=exaMeshGetDim(exaMeshF2C(*mesh));
  *err=0;
}

#define fExaMeshSetDim\
  EXA_FORTRAN_NAME(exameshsetdim,EXAMESHSETDIM)
void fExaMeshSetDim(exaFortranMesh *mesh,int ndim,int *err){
  *err=exaMeshSetDim(exaMeshF2C(*mesh),ndim);
}

#define fExaMeshGetDofsPerElement\
  EXA_FORTRAN_NAME(exameshgetdofsperelement,EXAMESHGETDOFSPERELEMENT)
void fExaMeshGetDofsPerElement(exaFortranMesh *mesh,int *ndofs,
  int *err)
{
  *ndofs=exaMeshGetDofsPerElement(exaMeshF2C(*mesh));
  *err=0;
}

#define fExaMeshGetLocalDofs\
  EXA_FORTRAN_NAME(exameshgetlocaldofs,EXAMESHGETLOCALDOFS)
void fExaMeshGetLocalDofs(exaFortranMesh *mesh,int *ndofs,int *err){
  *ndofs=exaMeshGetLocalDofs(exaMeshF2C(*mesh));
  *err=0;
}

#define fExaMeshGetNGeom\
  EXA_FORTRAN_NAME(exameshgetngeom,EXAMESHGETNGEOM)
void fExaMeshGetNGeom(exaFortranMesh *mesh,int *ngeom,int *err){
  *ngeom=exaMeshGetNGeom(exaMeshF2C(*mesh));
  *err=0;
}

#define fExaMeshDestroy\
  EXA_FORTRAN_NAME(exameshdestroy,EXAMESHDESTROY)
void fExaMeshDestroy(exaFortranMesh *mesh,int *err){
  *err=exaMeshDestroy(exaMeshF2C(*mesh));

  if(*err==0){
    meshActive--;
    if(meshActive==0)
      exaFree(meshDict),meshCurrent=0,meshMax=0;
  }
}

exaMesh exaMeshF2C(exaFortranMesh mesh){
  if(mesh<0||mesh>=meshCurrent||meshActive<=0){
    fprintf(stderr,"Invalid exaFortranMesh.");
    return NULL;
  }
  return meshDict[mesh];
}

exaHmholtz exaHmholtzF2C(exaFortranHmholtz hmholtz){
  if(hmholtz<0||hmholtz>=hmholtzCurrent||hmholtzActive<=0){
    fprintf(stderr,"Invalid exaFortranHmholtz.");
    return NULL;
  }
  return hmholtzDict[hmholtz];
}
