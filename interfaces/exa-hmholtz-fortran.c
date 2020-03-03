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
void fExaHmholtzCreate(exaFortranHmholtz *hmhz,
  exaFortranHandle *h,int *err)
{
  if(hmholtzCurrent==hmholtzMax)
    hmholtzMax+=hmholtzMax/2+1,exaRealloc(hmholtzMax,&hmholtzDict);

  *err=exaHmholtzCreate(&hmholtzDict[hmholtzCurrent],
    exaHandleF2C(*h));

  if(*err==0)
    *hmhz=hmholtzCurrent++,hmholtzActive++;
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
void fExaMeshCreate(exaFortranMesh *mesh,const char *meshFile,
  exaFortranHandle *exa,int *err,fortran_charlen_t meshFile_len)
{
  EXA_FIX_STRING(meshFile);

  if(meshCurrent==meshMax)
    meshMax+=meshMax/2+1,exaRealloc(meshMax,&meshDict);

  exaDebug(exaHandleF2C(*exa),"[fExaMeshCreate] %s:%d %s %d\n",
    __FILE__,__LINE__,meshFile_c,meshFile_len);

  //TODO: Null value passed from Fortran will be of length 1.
  //Need to figure out a better way to do this.
  *err=exaMeshCreate(&meshDict[meshCurrent],
    strlen(meshFile_c)>1?meshFile_c:NULL,exaHandleF2C(*exa));

  if(*err==0)
    *mesh=meshCurrent++,meshActive++;
}

#define fExaMeshSetup\
  EXA_FORTRAN_NAME(exameshsetup,EXAMESHSETUP)
void fExaMeshSetup(exaFortranMesh *mesh,exaFortranSettings *s,
  int *err)
{
  *err=exaMeshSetup(exaMeshF2C(*mesh),exaSettingsF2C(*s));
}

#define fExaMeshGetNElements\
  EXA_FORTRAN_NAME(exameshgetnelements,EXAMESHGETNELEMENTS)
void fExaMeshGetNElements(int *nelem,exaFortranMesh *mesh,int *err){
  *nelem=exaMeshGetNElements(exaMeshF2C(*mesh));
  *err=0;
}

#define fExaMeshSetNElements\
  EXA_FORTRAN_NAME(exameshsetnelements,EXAMESHSETNELEMENTS)
void fExaMeshSetNElements(exaFortranMesh *mesh,int *nelem,int *err){
  *err=exaMeshSetNElements(exaMeshF2C(*mesh),*nelem);
}

#define fExaMeshGet1DDofs\
  EXA_FORTRAN_NAME(exameshget1ddofs,EXAMESHGET1DDOFS)
void fExaMeshGet1DDofs(int *nx1,exaFortranMesh *mesh,int *err){
  *nx1=exaMeshGet1DDofs(exaMeshF2C(*mesh));
  *err=0;
}

#define fExaMeshSet1DDofs\
  EXA_FORTRAN_NAME(exameshset1ddofs,EXAMESHSET1DDOFS)
void fExaMeshSet1DDofs(exaFortranMesh *mesh,int *nx1,int *err){
  *err=exaMeshSet1DDofs(exaMeshF2C(*mesh),*nx1);
}

#define fExaMeshGetDim\
  EXA_FORTRAN_NAME(exameshgetdim,EXAMESHGETDIM)
void fExaMeshGetDim(int *ndim,exaFortranMesh *mesh,int *err){
  *ndim=exaMeshGetDim(exaMeshF2C(*mesh));
  *err=0;
}

#define fExaMeshSetDim\
  EXA_FORTRAN_NAME(exameshsetdim,EXAMESHSETDIM)
void fExaMeshSetDim(exaFortranMesh *mesh,int *ndim,int *err){
  *err=exaMeshSetDim(exaMeshF2C(*mesh),*ndim);
}

#define fExaMeshSetXcoords\
  EXA_FORTRAN_NAME(exameshsetxcoords,EXAMESHSETXCOORDS)
void fExaMeshSetXcoords(exaFortranMesh *mesh,exaScalar *xc,int *err){
  *err=exaMeshSetXcoords(exaMeshF2C(*mesh),xc);
}

#define fExaMeshSetYcoords\
  EXA_FORTRAN_NAME(exameshsetycoords,EXAMESHSETYCOORDS)
void fExaMeshSetYcoords(exaFortranMesh *mesh,exaScalar *yc,int *err){
  *err=exaMeshSetYcoords(exaMeshF2C(*mesh),yc);
}

#define fExaMeshSetZcoords\
  EXA_FORTRAN_NAME(exameshsetzcoords,EXAMESHSETZCOORDS)
void fExaMeshSetZcoords(exaFortranMesh *mesh,exaScalar *zc,int *err){
  *err=exaMeshSetZcoords(exaMeshF2C(*mesh),zc);
}

#define fExaMeshSetGlobalNumbering\
  EXA_FORTRAN_NAME(exameshsetglobalnumbering,\
  EXAMESHSETGLOBALNUMBERING)
void fExaMeshSetGlobalNumbering(exaFortranMesh *mesh,exaLong *gloNum,
  int *err)
{
  *err=exaMeshSetGlobalNumbering(exaMeshF2C(*mesh),gloNum);
}

#define fExaMeshSetMask\
  EXA_FORTRAN_NAME(exameshsetmask,EXAMESHSETMASK)
void fExaMeshSetMask(exaFortranMesh *mesh,exaScalar *mask,int *err)
{
  *err=exaMeshSetMask(exaMeshF2C(*mesh),mask);
}

#define fExaMeshSetGeometricFactors\
  EXA_FORTRAN_NAME(exameshsetgeometricfactors,\
  EXAMESHSETGEOMETRICFACTORS)
void fExaMeshSetGeometricFactors(exaFortranMesh *mesh,exaScalar *geom,
  int *err)
{
  *err=exaMeshSetGeometricFactors(exaMeshF2C(*mesh),geom);
}

#define fExaMeshSetDerivativeMatrix\
  EXA_FORTRAN_NAME(exameshsetderivativematrix,\
  EXAMESHSETDERIVATIVEMATRIX)
void fExaMeshSetDerivativeMatrix(exaFortranMesh *mesh,exaScalar *D,
  int *err)
{
  *err=exaMeshSetDerivativeMatrix(exaMeshF2C(*mesh),D);
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
