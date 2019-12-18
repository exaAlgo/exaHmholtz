#include <exa-hmholtz-impl.h>

int exaMeshSetup(exaMesh mesh,exaHmholtz hmhz){
  //geomatric factors
  //derivative matrix
  return 0;
}

exaInt exaMeshGetElements(exaMesh mesh){
  return mesh->nelt;
}

int exaMeshGet1DDofs(exaMesh mesh){
  return mesh->nx1;
}

int exaMeshGetElementDofs(exaMesh mesh){
  int dofs=mesh->nx1;
  return (mesh->ndim==2) ? dofs*dofs : dofs*dofs*dofs;
}

int exaMeshGetLocalDofs(exaMesh mesh){
  int dofs=mesh->nx1;
  exaInt nelt=mesh->nelt;
  return (mesh->ndim==2) ? dofs*dofs*nelt : dofs*dofs*dofs*nelt;
}
