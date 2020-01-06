#include "exa-hmholtz.h"

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

  exaHmholtzDestroy(hmhz);

  exaFinalize(h);

  MPI_Finalize();

  return 0;
}
