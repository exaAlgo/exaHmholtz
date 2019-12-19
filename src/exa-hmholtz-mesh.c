#include <exa-hmholtz-impl.h>
#include <nek-setup.h>

#include <math.h>

typedef struct{
  exaInt id;
} maskID;

int copyDataToDevice(exaMesh mesh){
  exaHandle h; exaMeshGetHandle(mesh,&h);

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

  /*TODO: move setup rotines out of here */
  /* setup gather scatter */
  exaGSSetup(mesh->glo_num,totalDofs,exaGetComm(h),0,0,&mesh->gs);
  exaBufferCreate(&mesh->buf,1024);

  /*TODO: setup global offsets and ids */

  /* setup multiplicities on host */
  exaMalloc(totalDofs,&mesh->rmult);
  exaUInt i;
  for(i=0;i<totalDofs;i++)
    mesh->rmult[i]=1.0;
  exaGSOp(mesh->rmult,exaScalar_t,exaAddOp,0,mesh->gs,mesh->buf);
  for(i=0;i<totalDofs;i++)
    mesh->rmult[i]=1.0/mesh->rmult[i];

  /* copy multiplicities to device */
  exaVectorCreate(h,totalDofs,exaScalar_t,&mesh->d_rmult);
  exaVectorWrite(mesh->d_rmult,mesh->rmult);

  /* setup mask */
  maskID id;
  exaArrayInit(&mesh->maskIds,maskID,10);
  for(i=0;i<totalDofs;i++)
    if(fabs(mesh->mask[i])<EXA_TOL){
      id.id=i;
      exaArrayAppend(mesh->maskIds,&id);
    }

  exaUInt size=exaArrayGetSize(mesh->maskIds);

  exaInt *ids; exaCalloc(size,&ids);
  maskID *ptr=exaArrayGetPointer(mesh->maskIds);
  for(i=0;i<size;i++){
    ids[i]=ptr[i].id;
  }

  /* copy masks to device */
  exaVectorCreate(h,size,exaInt_t,&mesh->d_maskIds);
  exaVectorWrite(mesh->d_maskIds,ids);
  exaFree(ids);

  return 0;
}

int exaMeshRead(exaMesh *mesh_,const char *meshName,
  const char *interface,exaSettings s)
{
  exaMalloc(1,mesh_); exaMesh mesh=*mesh_;
  exaHandle h; exaSettingsGetHandle(s,&h);
  exaMeshSetHandle(mesh,&h);

  if(strcmp(interface,"nek")==0){
    nekSetup(mesh,meshName,s);
  }

  //TODO: Fix following
  int nx1=exaMeshGet1DDofs(mesh);
  int elemDofs=exaMeshGetElementDofs(mesh);
  int ngeom=exaMeshGetNGeom(mesh);

  //p_Nggeo,p_Np
  exaSettingsSet("defines::p_Nq"   ,getExaUInt(nx1     ),s);
  exaSettingsSet("defines::p_Np"   ,getExaUInt(elemDofs),s);
  exaSettingsSet("defines::p_Nggeo",getExaUInt(ngeom   ),s);

  //p_G00ID,p_G01ID,p_G02ID,p_G11ID,p_G12ID,p_G22ID,p_GWJID
  int ndim=exaMeshGetDim(mesh);
  if(ndim==2){
    exaSettingsSet("defines::p_G00ID",getExaUInt(0),s);
    exaSettingsSet("defines::p_G01ID",getExaUInt(1),s);
    exaSettingsSet("defines::p_G11ID",getExaUInt(2),s);
    exaSettingsSet("defines::p_GWJID",getExaUInt(3),s);
  }else{
    exaSettingsSet("defines::p_G00ID",getExaUInt(0),s);
    exaSettingsSet("defines::p_G01ID",getExaUInt(1),s);
    exaSettingsSet("defines::p_G02ID",getExaUInt(2),s);
    exaSettingsSet("defines::p_G11ID",getExaUInt(3),s);
    exaSettingsSet("defines::p_G12ID",getExaUInt(4),s);
    exaSettingsSet("defines::p_G22ID",getExaUInt(5),s);
    exaSettingsSet("defines::p_GWJID",getExaUInt(6),s);
  }

  copyDataToDevice(mesh);

  return 0;
}

int exaMeshSetHandle(exaMesh mesh,exaHandle *h){
  mesh->h=*h;
  return 0;
}

int exaMeshGetHandle(exaMesh mesh,exaHandle *h){
  *h=mesh->h;
  return 0;
}

int exaMeshFinalize(exaMesh mesh){
  exaDestroy(mesh->d_maskIds);
  exaDestroy(mesh->maskIds);

  exaDestroy(mesh->d_rmult);
  exaFree(mesh->rmult);

  exaBufferFree(mesh->buf);
  exaGSFree(mesh->gs);

  exaFree(mesh->geom);
  exaDestroy(mesh->d_geom);

  exaDestroy(mesh->d_D);
  exaFree(mesh);

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

int exaMeshGetNGeom(exaMesh mesh){
  int ndim=mesh->ndim;
  return (ndim*(ndim+1))/2;
}

int exaMeshGetDim(exaMesh mesh){
  return mesh->ndim;
}
