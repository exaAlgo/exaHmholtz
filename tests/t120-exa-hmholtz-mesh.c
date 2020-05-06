#include "exa-hmholtz.h"

exaScalar x[8]={-1.0,1.0,
                -1.0,1.0,
                -1.0,1.0,
                -1.0,1.0};
exaScalar y[8]={-1.0,-1.0,
                 1.0, 1.0,
                -1.0,-1.0,
                 1.0, 1.0};
exaScalar z[8]={-1.0,-1.0,
                -1.0,-1.0,
                 1.0, 1.0,
                 1.0, 1.0};

exaLong globalIds[8]={1,2,3,4,5,6,7,8};

exaScalar geom[8*6]={0};
exaScalar mask[8]={0};
exaScalar D[2*2]={0};

int main(int argc,char *argv[])
{
  MPI_Init(NULL,NULL);

  if(argc!=2){
    printf("Usage: %s <backend>\n",argv[0]);
    exit(0);
  }

  exaHandle h;
  exaInit(&h,MPI_COMM_WORLD,argv[1]);

  exaHmholtz hmhz; exaHmholtzCreate(&hmhz,h);

  exaMesh mesh; exaMeshCreate(&mesh,NULL,h);

  exaMeshSetDim(mesh,3);
  exaMeshSetNElements(mesh,1);
  exaMeshSet1DDofs(mesh,2);

  exaMeshSetXcoords(mesh,x);
  exaMeshSetYcoords(mesh,y);
  exaMeshSetZcoords(mesh,z);

  exaMeshSetGlobalIds(mesh,globalIds);

  exaMeshSetGeometricFactors(mesh,geom);
  exaMeshSetMask(mesh,mask);
  exaMeshSetDerivativeMatrix(mesh,D);

  if(exaMeshGetDim(mesh)!=3)
    fprintf(stderr,"exaMeshGetDim failed.\n");
  if(exaMeshGetNElements(mesh)!=1)
    fprintf(stderr,"exaMeshGetNElements failed.\n");
  if(exaMeshGet1DDofs(mesh)!=2)
    fprintf(stderr,"exaMeshGet1DDofs failed.\n");

  if(exaMeshGetXcoords(mesh)!=x || exaMeshGetYcoords(mesh)!=y
    || exaMeshGetZcoords(mesh)!=z)
    fprintf(stderr,"exaMeshGet(X,Y,Z)coords failed.\n");

  if(exaMeshGetGlobalIds(mesh)!=globalIds)
    fprintf(stderr,"exaMeshGetGlobalIds failed.\n");

  if(exaMeshGetGeometricFactors(mesh)!=geom)
    fprintf(stderr,"exaMeshGetGeometricFactors failed.\n");
  if(exaMeshGetMask(mesh)!=mask)
    fprintf(stderr,"exaMeshGetMask failed.\n");
  if(exaMeshGetDerivativeMatrix(mesh)!=D)
    fprintf(stderr,"exaMeshGetDerivativeMatrix failed.\n");

  exaMeshDestroy(mesh);
  exaHmholtzDestroy(hmhz);
  exaFinalize(h);

  MPI_Finalize();

  return 0;
}
