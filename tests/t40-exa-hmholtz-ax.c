#include "exa-hmholtz-impl.h"
#include "exa-memory.h"

#include <math.h>

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
  exaSettingsSet("general::order",getExaInt(10),s);

  exaHmholtz hmhz; exaHmholtzCreate(h,s,&hmhz);

  exaMesh mesh;
  exaMeshRead(&mesh,"/home/thilina/Repos/NekExamples/b_0001/bb",
    "nek",s);

  exaHmholtzSetup(hmhz);

  // create x with zero initial guess and rhs with sin function
  exaVector x,b; exaUInt size=exaMeshGetLocalDofs(mesh);
  exaVectorCreate(h,size,exaScalar_t,&x);
  exaVectorCreate(h,size,exaScalar_t,&b);

  exaScalar *in,*sol; exaCalloc(size,&in); exaCalloc(size,&sol);
  exaVectorWrite(x,in);

  exaUInt ndofs=exaMeshGetElementDofs(mesh);
  exaInt i;
  for(i=0;i<size;i++){
    exaScalar xi =mesh->xm1[i];
    exaScalar yi =mesh->ym1[i];
    exaScalar zi =mesh->zm1[i];
    exaScalar bmi=mesh->geom[mesh->GWJID*ndofs+i];
    in[i]=3*M_PI*M_PI*sin(M_PI*xi)*sin(M_PI*yi)*sin(M_PI*zi)*bmi;
    sol[i]=sin(M_PI*xi)*sin(M_PI*yi)*sin(M_PI*zi);
  }
  exaVectorWrite(b,in);
  exaApplyMask(b,mesh->d_maskIds,hmhz);
  //TODO: dssum

  exaHmholtzCG(x,b,mesh,1e-8,1000,1,hmhz);

  exaScalar *out; exaVectorRead(x,(void**)&out);
  for(i=0;i<size;i++)
    if(fabs(sol[i]-out[i])>1e-7)
      fprintf(stderr,"error: %lf != %lf\n",sol[i],out[i]);

  exaFree(in); exaFree(sol);

  exaDestroy(b); exaDestroy(x);

  exaHmholtzDestroy(hmhz);
  exaDestroy(s);
  exaFinalize(h);

  MPI_Finalize();

  return 0;
}
