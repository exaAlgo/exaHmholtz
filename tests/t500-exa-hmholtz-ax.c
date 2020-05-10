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
  exaSettingsSet("general::order",getExaUInt(1),s);

  /* Create helmholtz solver */
  exaHmholtz hmhz; exaHmholtzCreate(&hmhz,h);
  exaHmholtzSetup(hmhz,s);

  /* Create a hex mesh of 2x1x1 */
  exaMesh mesh; exaMeshCreate(&mesh,NULL,h);
  exaMeshSetNElements(mesh,2);
  exaMeshSetDim(mesh,3);

  exaScalar xc[16]={-1.0,0.0,-1.0,0.0,-1.0,0.0,-1.0,0.0,
                     0.0,1.0, 0.0,1.0, 0.0,1.0, 0.0,1.0};
  exaScalar yc[16]={0.0,0.0,1.0,1.0,0.0,0.0,1.0,1.0,
                    0.0,0.0,1.0,1.0,0.0,0.0,1.0,1.0};
  exaScalar zc[16]={0.0,0.0,0.0,0.0,1.0,1.0,1.0,1.0,
                    0.0,0.0,0.0,0.0,1.0,1.0,1.0,1.0};

  exaMeshSetElemX(mesh,xc);
  exaMeshSetElemY(mesh,yc);
  exaMeshSetElemZ(mesh,zc);
  exaMeshSetMeshX(mesh,xc);
  exaMeshSetMeshY(mesh,yc);
  exaMeshSetMeshZ(mesh,zc);

  exaScalar mask[16]={1.0,0.0,1.0,0.0,1.0,0.0,1.0,0.0,
                      0.0,1.0,0.0,1.0,0.0,1.0,0.0,1.0};
  exaMeshSetMask(mesh,mask);

  exaLong globalIds[16]={ 1, 2, 3, 4, 5, 6, 7, 8,
                          2, 9, 4,10, 6,11, 8,12};
  exaMeshSetGlobalIds(mesh,globalIds);

  exaScalar D[4]={-0.5,0.5,-0.5,0.5};
  exaMeshSetDerivativeMatrix(mesh,D);

  exaScalar geom[2*8*6];
  for(int i=0;i<16;i++){
    geom[6*i+0]=0.5; geom[6*i+1]=0.0; geom[6*i+2]=0.0;
                     geom[6*i+3]=0.5; geom[6*i+4]=0.0;
                                      geom[6*i+5]=0.5;
  }
  exaMeshSetGeometricFactors(mesh,geom);

  exaMeshSetup(mesh,s);

  // create x with zero initial guess and rhs with sin function
  exaVector x,b; exaUInt size=exaMeshGetLocalDofs(mesh);
  exaVectorCreate(h,size,exaScalar_t,&x);
  exaVectorCreate(h,size,exaScalar_t,&b);

  exaScalar *in,*sol; exaCalloc(size,&in); exaCalloc(size,&sol);
  exaVectorWrite(x,in);

  exaUInt ndofs=exaMeshGetDofsPerElement(mesh);
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

  exaHmholtzCG(x,b,mesh,1e-8,1000,0,hmhz);

  exaScalar *out; exaVectorRead(x,(void**)&out);
  for(i=0;i<size;i++)
    if(fabs(sol[i]-out[i])>1e-7)
      fprintf(stderr,"error: %lf != %lf\n",sol[i],out[i]);

  exaFree(in); exaFree(sol);

  exaDestroy(b); exaDestroy(x);

  exaMeshDestroy(mesh);
  exaHmholtzDestroy(hmhz);
  exaDestroy(s);
  exaFinalize(h);

  MPI_Finalize();

  return 0;
}
