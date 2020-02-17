#include "exa-hmholtz.h"
#include "exa-memory.h"

#include <math.h>

#define M 128

int main(int argc,char *argv[])
{
  MPI_Init(NULL,NULL);

  if(argc!=2){
    printf("Usage: %s <backend>\n",argv[0]);
    exit(0);
  }

  exaHandle h;
  exaInit(&h,MPI_COMM_WORLD,argv[1]);

  exaSettings s; exaSettingsCreate(h,NULL,&s);
  exaSettingsSet("general::order",getExaInt(7),s);

  exaMesh mesh; exaMeshCreate(&mesh,NULL,h);
  exaMeshSetup(mesh,s);

  exaHmholtz hmhz; exaHmholtzCreate(&hmhz,h);
  exaHmholtzSetup(hmhz,s,mesh);

  exaVector vec,weights;
  exaVectorCreate(h,M,exaScalar_t,&vec);
  exaVectorCreate(h,M,exaScalar_t,&weights);

  exaScalar *in;
  exaMalloc(M,&in);
  exaInt i;
  for(i=0;i<M;i++) in[i]=0.5;
  exaVectorWrite(vec,in);
  exaVectorWrite(weights,in);
  exaFree(in);

  exaULong size=M;
  exaGop(h,&size,1,exaULong_t,exaAddOp);

  exaScalar norm=exaVectorWeightedNorm2(weights,vec,hmhz);
  exaScalar ans=0.5*0.5*0.5*size;
  if(fabs(norm-ans)>EXA_TOL)
    fprintf(stderr,"norm: %lf != %lf\n",norm,ans);

  exaDestroy(vec);
  exaDestroy(weights);

  exaHmholtzDestroy(hmhz);
  exaMeshDestroy(mesh);
  exaDestroy(s);

  exaFinalize(h);

  MPI_Finalize();

  return 0;
}
