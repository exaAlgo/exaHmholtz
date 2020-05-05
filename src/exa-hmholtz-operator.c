#include <exa-hmholtz-impl.h>

int exaHmholtzOperator(exaVector p,exaVector Ap,exaMesh mesh)
{
  exaInt nElements=exaMeshGetNElements(mesh);
  exaScalar lambda=0;

  exaKernelRun(mesh->hmholtzAx,getExaUInt(nElements),mesh->d_geom,
    mesh->d_D,getExaScalar(lambda),p,Ap);

  return 0;
}
