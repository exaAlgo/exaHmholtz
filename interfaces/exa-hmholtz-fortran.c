// Fortran interface
#include <exa-fortran-name.h>
#include <exa-hmholtz-impl.h>
#include <stdlib.h>
#include <string.h>

static exaHandle *handleDict=NULL;
static int handleCurrent=0;
static int handleActive=0;
static int handleMax=0;

#define fExaInit EXA_FORTRAN_NAME(exainit,EXAINIT)
void fExaInit(const char *backend,MPI_Fint *fcomm,int *exa,int *err,
  fortran_charlen_t backend_len)
{
  FIX_STRING(backend);
  if(handleCurrent==handleMax)
    handleMax+=handleMax/2+1,exaRealloc(handleMax,&handleDict);

  MPI_Comm comm=MPI_Comm_f2c(*fcomm);
  *err=exaInit(&handleDict[handleCurrent],comm,backend_c);

  if(*err==0)
    *exa=handleCurrent++,handleActive++;
}

#define fExaFinalize EXA_FORTRAN_NAME(exafinalize,EXAFINALIZE)
void fExaFinalize(int *exa, int *err){
  // TODO: Validate handle
  *err=exaFinalize(handleDict[*exa]);

  if(*err==0){
    handleActive--;
    if(handleActive==0)
      exaFree(handleDict),handleCurrent=0,handleMax=0;
  }
}

