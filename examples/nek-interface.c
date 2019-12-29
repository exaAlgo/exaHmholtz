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

  exaSettings s; exaSettingsCreate(h,NULL,&s);
  exaSettingsSet("general::order",getExaInt(2),s);

  exaMesh mesh;
  exaMeshRead(&mesh,"/home/thilina/Repos/NekExamples/b_0001/bb",
    "nek",s);

  exaHmholtz hmhz; exaHmholtzCreate(h,s,&hmhz);
  exaHmholtzSetup(hmhz);

  exaUInt dofs=exaMeshGetLocalDofs(mesh);
  exaInt i;

#if 0
  printf("nel:%d 1d dofs:%d local dofs:%d\n",
    exaMeshGetElements(mesh),exaMeshGet1DDofs(mesh),
    exaMeshGetLocalDofs(mesh));

  for(i=0;i<dofs;i++)
    printf("%02lld ",mesh->glo_num[i]);
  printf("\n");
  for(i=0;i<dfos;i++)
    printf("%.2lf %.2lf %.2lf\n",mesh->xm1[i],mesh->ym1[i],
      mesh->zm1[i]);
  printf("\n");
#endif

  exaVector q; exaVectorCreate(h,dofs,exaScalar_t,&q);
  exaScalar *in; exaMalloc(dofs,&in);
  for(i=0;i<dofs;i++) in[i]=1.0;
  exaVectorWrite(q,in);

  exaApplyMask(q,mesh->d_maskIds,hmhz);

  exaScalar *out; exaVectorRead(q,(void**)&out);
  for(i=0;i<dofs;i++)
    printf("%.2lf ",out[i]);

  exaFree(in); exaDestroy(q);

  exaMeshDestroy(mesh);
  exaHmholtzDestroy(hmhz);
  exaDestroy(s);

  exaFinalize(h);

  MPI_Finalize();

  return 0;
}
