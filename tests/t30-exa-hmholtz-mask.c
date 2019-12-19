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
  exaSettingsSet("general::order",getExaInt(2),s);

  exaHmholtz hmhz; exaHmholtzCreate(h,s,&hmhz);

  exaHmholtzSetup(hmhz);

  exaVector vec,maskIds;
  exaVectorCreate(h,M,exaScalar_t,&vec);
  exaUInt half=(M+1)/2;
  exaVectorCreate(h,half,exaInt_t,&maskIds);

  exaScalar *in; exaMalloc(M,&in);
  exaInt i;
  for(i=0;i<M;i++) in[i]=0.5;
  exaVectorWrite(vec,in);

  exaInt *maskIn; exaMalloc(half,&maskIn);
  exaInt cnt;
  // zero out every other point
  for(i=0,cnt=0;i<M;i+=2,cnt++){
    in[i]=0.;
    maskIn[cnt]=i;
  }
  exaVectorWrite(maskIds,maskIn);
  exaFree(maskIn);

  exaApplyMask(vec,maskIds,hmhz);

  exaScalar *out; exaMalloc(M,&out);
  exaVectorRead(vec,out);
  for(i=0;i<M;i++)
    if(fabs(in[i]-out[i])>EXA_TOL)
      fprintf(stderr,"Mask error: %lf != %lf\n",in[i],out[i]);
  exaFree(out);
  exaFree(in);

  exaDestroy(vec);
  exaDestroy(maskIds);

  exaHmholtzDestroy(hmhz);
  exaDestroy(s);
  exaFinalize(h);

  MPI_Finalize();

  return 0;
}
