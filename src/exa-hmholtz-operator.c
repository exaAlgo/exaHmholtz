#include <exa-hmholtz-impl.h>

int exaHmholtzOperator(exaVector p,exaVector Ap,exaMesh mesh,
  exaHmholtz hz)
{
  exaInt nElements=exaMeshGetElements(mesh);
  exaScalar lambda=0;

  exaKernelRun(hz->hmholtzAx,getExaUInt(nElements),mesh->d_geom,
    mesh->d_D,getExaScalar(lambda),p,Ap);

  return 0;
}
