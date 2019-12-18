#include "exa-hmholtz.h"
#include "exa-memory.h"

#include <math.h>

#define M 10

int main(int argc,char *argv[])
{
  MPI_Init(NULL,NULL);

  if(argc!=2){
    printf("Usage: %s <backend>\n",argv[0]);
    exit(0);
  }

  exaHandle h;
  exaInit(&h,MPI_COMM_WORLD,argv[1]);

  exaSettings s; exaSettingsInit(h,NULL,&s);

  exaHmholtz hmhz;
  exaHmholtzCreate(h,s,&hmhz);

  exaVector vec1,vec2;
  exaVectorCreate(h,M,exaScalar_t,&vec1);
  exaVectorCreate(h,M,exaScalar_t,&vec2);

  exaScalar *in,*out;
  exaMalloc(M,&in);
  exaCalloc(M,&out);
  exaInt i;
  for(i=0;i<M;i++) in[i]=1.0;

  exaVectorWrite(vec1,in);
  exaVectorWrite(vec2,in);

  exaScalar alpha=0.1,beta=0.3;

  exaVectorScaledAdd(alpha,vec1,beta,vec2,hmhz);

  exaVectorRead(vec2,out);
  for(i=0;i<M;i++)
    if(fabs(out[i]-0.4)>EXA_TOL)
      fprintf(stderr,"%lf != 0.4\n",out[i]);

  exaFree(in);
  exaDestroy(vec1);
  exaDestroy(vec2);

  exaHmholtzDestroy(hmhz);
  exaDestroy(s);

  exaFinalize(h);

  MPI_Finalize();

  return 0;
}
