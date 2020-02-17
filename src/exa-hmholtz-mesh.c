#include <exa-hmholtz-impl.h>
#include <math.h>

int exaMeshCreate(exaMesh *mesh_,const char *meshFile,exaHandle h){
  exaDebug(h,"[meshCreate]\n");
  exaMalloc(1,mesh_); exaMesh mesh=*mesh_;

  if(meshFile==NULL){
    mesh->nelt=-1;
    mesh->nx1=-1;
    mesh->ndim=-1;
    mesh->xm1=mesh->ym1=mesh->zm1=NULL;
    mesh->gloNum=NULL;
    mesh->mask=mesh->geom=mesh->D=NULL;
  }else{
    //TODO: read the mesh from mesh file
    // meshRead(mesh,meshFile);
  }

  exaMeshSetHandle(mesh,&h);

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

exaInt exaMeshGetNElements(exaMesh mesh){
  return mesh->nelt;
}
int exaMeshSetNElements(exaMesh mesh,exaInt nelem){
  mesh->nelt=nelem;
  return 0;
}

int exaMeshGet1DDofs(exaMesh mesh){
  return mesh->nx1;
}
int exaMeshSet1DDofs(exaMesh mesh,int nx1){
  mesh->nx1=nx1;
  return 0;
}

int exaMeshGetDim(exaMesh mesh){
  return mesh->ndim;
}
int exaMeshSetDim(exaMesh mesh,int dim){
  mesh->ndim=dim;
  return 0;
}

int exaMeshSetXcoords(exaMesh mesh,exaScalar *xc){
  mesh->xm1=xc;
  return 0;
}
exaScalar *exaMeshGetXcoords(exaMesh mesh){
  return mesh->xm1;
}

int exaMeshSetYcoords(exaMesh mesh,exaScalar *yc){
  mesh->ym1=yc;
  return 0;
}
exaScalar *exaMeshGetYcoords(exaMesh mesh){
  return mesh->ym1;
}

int exaMeshSetZcoords(exaMesh mesh,exaScalar *zc){
  mesh->zm1=zc;
  return 0;
}
exaScalar *exaMeshGetZcoords(exaMesh mesh){
  return mesh->zm1;
}

int exaMeshSetGlobalNumbering(exaMesh mesh,exaLong *gloNum){
  mesh->gloNum=gloNum;
  return 0;
}
exaLong *exaMeshGetGlobalNumbering(exaMesh mesh){
  return mesh->gloNum;
}

int exaMeshSetMask(exaMesh mesh,exaScalar *mask){
  mesh->mask=mask;
  return 0;
}
exaScalar *exaMeshGetMask(exaMesh mesh){
  return mesh->mask;
}

int exaMeshSetGeometricFactors(exaMesh mesh,exaScalar *geom){
  mesh->geom=geom;
  return 0;
}
exaScalar *exaMeshGetGeometricFactors(exaMesh mesh){
  return mesh->geom;
}

int exaMeshSetDerivativeMatrix(exaMesh mesh,exaScalar *D){
  mesh->D=D;
  return 0;
}
exaScalar *exaMeshGetDerivativeMatrix(exaMesh mesh){
  return mesh->D;
}

int exaMeshGetDofsPerElement(exaMesh mesh){
  int dofs=mesh->nx1;
  return (mesh->ndim==2) ? dofs*dofs : dofs*dofs*dofs;
}

int exaMeshGetLocalDofs(exaMesh mesh){
  exaInt nelt=mesh->nelt;
  return nelt*exaMeshGetDofsPerElement(mesh);
}

int exaMeshGetNGeom(exaMesh mesh){
  int ndim=mesh->ndim;
  return (ndim*(ndim+1))/2+1;
}

int exaMeshSetup(exaMesh mesh,exaSettings s){
  exaHandle h; exaMeshGetHandle(mesh,&h);

  int order; exaSettingsGet(&order,"general::order",s);
  exaDebug(h,"exaMeshSetup: order=%d\n",order);
  int nx1=order+1; exaMeshSet1DDofs(mesh,nx1);

  int elemDofs=exaMeshGetDofsPerElement(mesh);
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
    mesh->G00ID=0;
    mesh->G01ID=1;
    mesh->G11ID=2;
    mesh->GWJID=3;
  }else{
    exaSettingsSet("defines::p_G00ID",getExaUInt(0),s);
    exaSettingsSet("defines::p_G01ID",getExaUInt(1),s);
    exaSettingsSet("defines::p_G02ID",getExaUInt(2),s);
    exaSettingsSet("defines::p_G11ID",getExaUInt(3),s);
    exaSettingsSet("defines::p_G12ID",getExaUInt(4),s);
    exaSettingsSet("defines::p_G22ID",getExaUInt(5),s);
    exaSettingsSet("defines::p_GWJID",getExaUInt(6),s);
    mesh->G00ID=0;
    mesh->G01ID=1;
    mesh->G02ID=2;
    mesh->G11ID=3;
    mesh->G12ID=4;
    mesh->G22ID=5;
    mesh->GWJID=6;
  }

  return 0;
}

int exaMeshDestroy(exaMesh mesh){
  exaHandle h; exaMeshGetHandle(mesh,&h);
  exaDebug(h,"[meshDestroy]\n");
#if 0
  exaDestroy(mesh->d_maskIds);
  exaDestroy(mesh->maskIds);

  exaDestroy(mesh->d_rmult);
  exaFree(mesh->rmult);

  exaBufferFree(mesh->buf);
  exaGSFree(mesh->gs);

  exaFree(mesh->geom);
  exaDestroy(mesh->d_geom);

  exaDestroy(mesh->d_D);
#endif

  exaFree(mesh);

  return 0;
}
