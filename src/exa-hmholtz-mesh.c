#include <exa-hmholtz-impl.h>

typedef struct{
  exaLong id;
} maskID;

int exaMeshSetup(exaMesh mesh,exaHmholtz hmhz){
  exaHandle h;
  exaHmholtzGetHandle(hmhz,&h);

  int nelt =mesh->nelt;
  int ndim =mesh->ndim;
  int ngeom=mesh->ngeom;

  int nx1=mesh->nx1;
  exaUInt ndofs=nx1*nx1;
  if(ndim==3) ndofs*=nx1;

  exaUInt totalDofs=ndofs*nelt;

  /* copy geometric factors and derivative matrix */
  exaVectorCreate(h,totalDofs*ngeom,exaScalar_t,&mesh->d_geom);
  exaVectorWrite(mesh->d_geom,mesh->geom);

  exaVectorCreate(h,nx1*nx1,exaScalar_t,&mesh->d_D);
  exaVectorWrite(mesh->d_D,mesh->D);

  /* setup gather scatter */
  exaGSSetup(mesh->glo_num,totalDofs,exaGetComm(h),0,0,&mesh->gs);
  exaBufferCreate(&mesh->buf,1024);

  /* setup multiplicities on host */
  exaMalloc(totalDofs,&mesh->rmult);
  exaUInt i;
  for(i=0;i<totalDofs;i++)
    mesh->rmult[i]=1.0;
  exaGSOp(mesh->rmult,exaScalar_t,exaAddOp,0,mesh->gs,mesh->buf);
  for(i=0;i<totalDofs;i++)
    mesh->rmult[i]=1.0/mesh->rmult[i];

  /* copy these to device */
  exaVectorCreate(h,totalDofs,exaScalar_t,&mesh->d_rmult);
  exaVectorWrite(mesh->d_rmult,mesh->rmult);

  /* setup mask */
  maskID id;
  exaArrayInit(&mesh->maskIds,maskID,10);
  for(i=0;i<totalDofs;i++)
    if(mesh->mask[i]!=0){
      id.id=mesh->glo_num[i];
      exaArrayAppend(mesh->maskIds,&id);
    }

  exaUInt size=exaArrayGetSize(mesh->maskIds);
  exaLong *ids; exaCalloc(size,&ids);
  maskID *ptr=exaArrayGetPointer(mesh->maskIds);
  for(i=0;i<size;i++){
    ids[i]=ptr[i].id;
  }
  exaVectorCreate(h,size,exaLong_t,&mesh->d_maskIds);
  exaVectorWrite(mesh->d_maskIds,ids);
  exaFree(ids);

  return 0;
}

int exaMeshFinalize(exaMesh mesh){
  exaDestroy(mesh->d_maskIds);
  exaDestroy(mesh->maskIds);
  exaDestroy(mesh->d_rmult);
  exaFree(mesh->rmult);
  exaBufferFree(mesh->buf);
  exaGSFree(mesh->gs);
  exaDestroy(mesh->d_geom);
  exaDestroy(mesh->d_D);
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
