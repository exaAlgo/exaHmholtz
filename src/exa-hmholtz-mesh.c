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
    fprintf(stderr,"Reading the mesh from a file is not"
      " implemented yet.");
  }

  exaMeshSetHandle(mesh,&h);

  exaDebug(h,"[/meshCreate]\n");
  return 0;
}

int exaMeshInitialized(exaMesh mesh){
  exaHandle h; exaMeshGetHandle(mesh,&h);
  exaDebug(h,"[meshInitialized]\n");

  int initialized=1;

  if(mesh->nelt==-1||mesh->nx1==-1||mesh->ndim==-1)
    initialized=0;
  if(mesh->xm1==NULL||mesh->ym1==NULL||mesh->zm1==NULL)
    initialized=0;
  if(mesh->mask==NULL||mesh->geom==NULL||mesh->D==NULL)
    initialized=0;
  if(mesh->gloNum==NULL)
    initialized=0;

  exaDebug(h,"[/meshInitialized]\n");
  return initialized;
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
  exaHandle h; exaMeshGetHandle(mesh,&h);
  exaDebug(h,"[exaMeshSetNElements] %s:%d nelem=%d\n",__FILE__,
    __LINE__,nelem);

  mesh->nelt=nelem;
  return 0;
}

int exaMeshGet1DDofs(exaMesh mesh){
  return mesh->nx1;
}
int exaMeshSet1DDofs(exaMesh mesh,int nx1){
  exaHandle h; exaMeshGetHandle(mesh,&h);
  exaDebug(h,"[exaMeshSet1DDofs] %s:%d nx1=%d\n",__FILE__,
    __LINE__,nx1);

  mesh->nx1=nx1;
  return 0;
}

int exaMeshGetDim(exaMesh mesh){
  return mesh->ndim;
}
int exaMeshSetDim(exaMesh mesh,int dim){
  exaHandle h; exaMeshGetHandle(mesh,&h);
  exaDebug(h,"[exaMeshSetDim] %s:%d ndim=%d\n",__FILE__,
    __LINE__,dim);

  mesh->ndim=dim;
  return 0;
}

int exaMeshSetXcoords(exaMesh mesh,exaScalar *xc){
  exaHandle h; exaMeshGetHandle(mesh,&h);
  exaDebug(h,"[exaMeshSetXcoords] %s:%d xm1=%p\n",__FILE__,
    __LINE__,xc);

  mesh->xm1=xc;
  return 0;
}
exaScalar *exaMeshGetXcoords(exaMesh mesh){
  return mesh->xm1;
}

int exaMeshSetYcoords(exaMesh mesh,exaScalar *yc){
  exaHandle h; exaMeshGetHandle(mesh,&h);
  exaDebug(h,"[exaMeshSetXcoords] %s:%d ym1=%p\n",__FILE__,
    __LINE__,yc);

  mesh->ym1=yc;
  return 0;
}
exaScalar *exaMeshGetYcoords(exaMesh mesh){
  return mesh->ym1;
}

int exaMeshSetZcoords(exaMesh mesh,exaScalar *zc){
  exaHandle h; exaMeshGetHandle(mesh,&h);
  exaDebug(h,"[exaMeshSetXcoords] %s:%d zm1=%p\n",__FILE__,
    __LINE__,zc);

  mesh->zm1=zc;
  return 0;
}
exaScalar *exaMeshGetZcoords(exaMesh mesh){
  return mesh->zm1;
}

int exaMeshSetGlobalNumbering(exaMesh mesh,exaLong *gloNum){
  exaHandle h; exaMeshGetHandle(mesh,&h);
  exaDebug(h,"[exaMeshSetGlobalNumbering] %s:%d glo_num=%p\n",
    __FILE__, __LINE__,gloNum);

  //should this be a copy?
  mesh->gloNum=gloNum;
  return 0;
}
exaLong *exaMeshGetGlobalNumbering(exaMesh mesh){
  return mesh->gloNum;
}

int exaMeshSetMask(exaMesh mesh,exaScalar *mask){
  exaHandle h; exaMeshGetHandle(mesh,&h);
  exaDebug(h,"[exaMeshSetMask] %s:%d rmask=%p\n", __FILE__,
    __LINE__,mask);

  mesh->mask=mask;
  return 0;
}
exaScalar *exaMeshGetMask(exaMesh mesh){
  return mesh->mask;
}

int exaMeshSetGeometricFactors(exaMesh mesh,exaScalar *geom){
  exaHandle h; exaMeshGetHandle(mesh,&h);
  exaDebug(h,"[exaMeshSetGeometricFactors] %s:%d geom=%p\n",
    __FILE__,__LINE__,geom);

  mesh->geom=geom;
  return 0;
}
exaScalar *exaMeshGetGeometricFactors(exaMesh mesh){
  return mesh->geom;
}

int exaMeshSetDerivativeMatrix(exaMesh mesh,exaScalar *D){
  exaHandle h; exaMeshGetHandle(mesh,&h);
  exaDebug(h,"[exaMeshSetDerivativeMatrix] %s:%d dxm1=%p\n",
    __FILE__,__LINE__,D);

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
  exaDebug(h,"[exaMeshSetup] %s:%d\n",__FILE__,__LINE__);

  assert(exaMeshInitialized(mesh));

  int nx1=exaMeshGet1DDofs(mesh);
  int ndim=exaMeshGetDim(mesh);
  int ngeom=exaMeshGetNGeom(mesh);
  int elemDofs=exaMeshGetDofsPerElement(mesh);

  exaSettingsSet("defines::p_Nq"   ,getExaUInt(nx1     ),s);
  exaSettingsSet("defines::p_Np"   ,getExaUInt(elemDofs),s);
  exaSettingsSet("defines::p_Nggeo",getExaUInt(ngeom   ),s);

  //p_G00ID,p_G01ID,p_G02ID,p_G11ID,p_G12ID,p_G22ID,p_GWJID
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
