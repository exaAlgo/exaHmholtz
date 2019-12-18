#include "exa-hmholtz.h"
#include "nek-setup.h"

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
  exaSettingsSet("general::order",getExaInt(7),s);

  exaHmholtz hmhz;
  exaHmholtzCreate(h,s,&hmhz);

  exaMesh mesh;
  nekSetup(&mesh,"/home/thilina/Repos/NekExamples/b_0001/bb",hmhz);
  exaMeshSetup(mesh,hmhz);
  printf("nel:%d 1d dofs:%d local dofs:%d\n",
    exaMeshGetElements(mesh),exaMeshGet1DDofs(mesh),
    exaMeshGetLocalDofs(mesh));

  exaHmholtzDestroy(hmhz);
  exaDestroy(s);

  exaFinalize(h);

  MPI_Finalize();

  return 0;
}
