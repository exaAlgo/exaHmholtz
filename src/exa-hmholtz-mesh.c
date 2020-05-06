#include <exa-hmholtz-impl.h>
#include <math.h>

int exaMeshCreate(exaMesh *mesh_,const char *meshFile,exaHandle h){
  exaDebug(h,"[meshCreate]\n");
  exaMalloc(1,mesh_); exaMesh mesh=*mesh_;

  if(meshFile==NULL){
    mesh->nelt=-1;
    mesh->nx1 =-1;
    mesh->ndim=-1;

    mesh->xm1=mesh->ym1=mesh->zm1=NULL;
    mesh->xc =mesh->yc =mesh->zc =NULL;

    mesh->gloNum=NULL;

    mesh->mask=NULL;
    mesh->geom=NULL;
    mesh->D   =NULL;
  }else{
    //TODO: read the mesh from mesh file
    fprintf(stderr,"Reading the mesh from a file is not"
      " implemented yet.");
  }

  mesh->hmholtzAx=NULL;

  exaMeshSetHandle(mesh,&h);

  exaDebug(h,"[/meshCreate]\n");
  return 0;
}

int exaMeshInitialized(exaMesh mesh){
  exaHandle h; exaMeshGetHandle(mesh,&h);
  exaDebug(h,"[meshInitialized]\n");

  int initialized=1;

  if(mesh->nelt==-1||mesh->ndim==-1)
    initialized=0;
  if(mesh->xc==NULL||mesh->yc==NULL||mesh->zc==NULL)
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
  if(exaRank(h)==0)
    exaDebug(h,"[exaMeshSetNElements] %s:%d nelem=%d\n",
      __FILE__,__LINE__,nelem);

  mesh->nelt=nelem;

  if(exaRank(h)==0)
    exaDebug(h,"[/exaMeshSetNElements]\n");

  return 0;
}

int exaMeshGet1DDofs(exaMesh mesh){
  return mesh->nx1;
}
int exaMeshSet1DDofs(exaMesh mesh,int nx1){
  exaHandle h; exaMeshGetHandle(mesh,&h);
  if(exaRank(h)==0)
    exaDebug(h,"[exaMeshSet1DDofs] %s:%d nx1=%d\n",__FILE__,__LINE__,nx1);

  mesh->nx1=nx1;

  if(exaRank(h)==0)
    exaDebug(h,"[/exaMeshSet1DDofs]\n");

  return 0;
}

int exaMeshGetDim(exaMesh mesh){
  return mesh->ndim;
}
int exaMeshSetDim(exaMesh mesh,int dim){
  exaHandle h; exaMeshGetHandle(mesh,&h);
  if(exaRank(h)==0)
    exaDebug(h,"[exaMeshSetDim] %s:%d dim=%d\n",__FILE__,__LINE__,dim);

  mesh->ndim=dim;

  if(exaRank(h)==0)
    exaDebug(h,"[/exaMeshSetDim]\n");
  return 0;
}

int exaMeshSetElemX(exaMesh mesh,exaScalar *xc){
  exaHandle h; exaMeshGetHandle(mesh,&h);
  if(exaRank(h)==0)
    exaDebug(h,"[exaMeshSetElemX] %s:%d xc=%p\n",__FILE__,__LINE__,xc);

  mesh->xc=xc;

  if(exaRank(h)==0)
    exaDebug(h,"[/exaMeshSetElemX]\n");
  return 0;
}
exaScalar *exaMeshGetElemX(exaMesh mesh){
  exaHandle h; exaMeshGetHandle(mesh,&h);
  return mesh->xc;
}

int exaMeshSetElemY(exaMesh mesh,exaScalar *yc){
  exaHandle h; exaMeshGetHandle(mesh,&h);
  if(exaRank(h)==0)
    exaDebug(h,"[exaMeshSetElemY] %s:%d yc=%p\n",__FILE__,__LINE__,yc);

  mesh->yc=yc;

  if(exaRank(h)==0)
    exaDebug(h,"[/exaMeshSetElemY]\n");

  return 0;
}
exaScalar *exaMeshGetElemY(exaMesh mesh){
  exaHandle h; exaMeshGetHandle(mesh,&h);
  return mesh->yc;
}

int exaMeshSetElemZ(exaMesh mesh,exaScalar *zc){
  exaHandle h; exaMeshGetHandle(mesh,&h);
  if(exaRank(h)==0)
    exaDebug(h,"[exaMeshSetElemZ] %s:%d zc=%p\n",__FILE__,__LINE__,zc);

  mesh->zc=zc;

  if(exaRank(h)==0)
    exaDebug(h,"[/exaMeshSetElemZ]\n");

  return 0;
}
exaScalar *exaMeshGetElemZ(exaMesh mesh){
  exaHandle h; exaMeshGetHandle(mesh,&h);
  return mesh->zc;
}

int exaMeshSetMeshX(exaMesh mesh,exaScalar *xm1){
  exaHandle h; exaMeshGetHandle(mesh,&h);
  if(exaRank(h)==0)
    exaDebug(h,"[exaMeshSetMeshX] %s:%d xm1=%p\n",__FILE__,__LINE__,xm1);

  mesh->xm1=xm1;

  if(exaRank(h)==0)
    exaDebug(h,"[/exaMeshSetMeshX]\n");
  return 0;
}
exaScalar *exaMeshGetMeshX(exaMesh mesh){
  exaHandle h; exaMeshGetHandle(mesh,&h);
  return mesh->xm1;
}

int exaMeshSetMeshY(exaMesh mesh,exaScalar *ym1){
  exaHandle h; exaMeshGetHandle(mesh,&h);
  if(exaRank(h)==0)
    exaDebug(h,"[exaMeshSetMeshY] %s:%d ym1=%p\n",__FILE__,__LINE__,ym1);

  mesh->ym1=ym1;

  if(exaRank(h)==0)
    exaDebug(h,"[/exaMeshSetMeshY]\n");

  return 0;
}
exaScalar *exaMeshGetMeshY(exaMesh mesh){
  exaHandle h; exaMeshGetHandle(mesh,&h);
  return mesh->ym1;
}

int exaMeshSetMeshZ(exaMesh mesh,exaScalar *zm1){
  exaHandle h; exaMeshGetHandle(mesh,&h);
  if(exaRank(h)==0)
    exaDebug(h,"[exaMeshSetMeshZ] %s:%d zm1=%p\n",__FILE__,__LINE__,zm1);

  mesh->zm1=zm1;

  if(exaRank(h)==0)
    exaDebug(h,"[/exaMeshSetMeshZ]\n");

  return 0;
}
exaScalar *exaMeshGetMeshZ(exaMesh mesh){
  exaHandle h; exaMeshGetHandle(mesh,&h);
  return mesh->zm1;
}

int exaMeshSetGlobalNumbering(exaMesh mesh,exaLong *gloNum){
  exaHandle h; exaMeshGetHandle(mesh,&h);
  if(exaRank(h)==0)
    exaDebug(h,"[exaMeshSetGlobalNumbering] %s:%d glo_num=%p\n",
      __FILE__, __LINE__,gloNum);

  //should this be a copy?
  mesh->gloNum=gloNum;

  if(exaRank(h)==0)
    exaDebug(h,"[exaMeshSetGlobalNumbering]\n");

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

static int gatherScatterSetup(exaMesh mesh){
  exaHandle h; exaMeshGetHandle(mesh,&h);

  exaUInt totalDofs=exaMeshGetLocalDofs(mesh);

  /* setup gather scatter */
  exaGSSetup(mesh->gloNum,totalDofs,exaGetComm(h),0,0,&mesh->gs);
  exaBufferCreate(&mesh->buf,1024);

  /*TODO: setup global offsets and ids */

  /* setup multiplicities on host */
  exaMalloc(totalDofs,&mesh->rmult);
  exaUInt i;
  for(i=0;i<totalDofs;i++)
    mesh->rmult[i]=1.0;

  exaGSOp(mesh->rmult,exaScalar_t,exaAddOp,0,mesh->gs,mesh->buf);

  for(i=0;i<totalDofs;i++){
    mesh->rmult[i]=1.0/mesh->rmult[i];
    exaDebug(h,"%lf ",mesh->rmult[i]);
  }
  exaDebug(h,"\n");

  return 0;
}

typedef struct{
  exaInt id;
} maskID;

static int copyDataToDevice(exaMesh mesh){
  exaHandle h; exaMeshGetHandle(mesh,&h);

  int nelt=exaMeshGetNElements(mesh);
  int ndim=exaMeshGetDim(mesh);
  int nx1 =exaMeshGet1DDofs(mesh);
  int ngeom=exaMeshGetNGeom(mesh);

  exaUInt totalDofs=exaMeshGetLocalDofs(mesh);

  /* copy geometric factors and derivative matrix */
  exaVectorCreate(h,totalDofs*ngeom,exaScalar_t,&mesh->d_geom);
  exaVectorWrite(mesh->d_geom,mesh->geom);

  exaVectorCreate(h,nx1*nx1,exaScalar_t,&mesh->d_D);
  exaScalar *D; exaCalloc(nx1*nx1,&D);
  for(int i=0;i<nx1;i++)
    for(int j=0;j<nx1;j++)
    D[i*nx1+j]=mesh->D[j*nx1+i];
  exaVectorWrite(mesh->d_D,D);
  exaFree(D);

  /* copy multiplicities to device */
  exaVectorCreate(h,totalDofs,exaScalar_t,&mesh->d_rmult);
  exaVectorWrite(mesh->d_rmult,mesh->rmult);

  /* setup mask */
  maskID id;
  exaArrayInit(&mesh->maskIds,maskID,10);
  exaDebug(h,"Masked ids: ");

  exaUInt i;
  for(i=0;i<totalDofs;i++)
    if(fabs(mesh->mask[i])<EXA_TOL){
      exaDebug(h,"%d ",i);
      id.id=i;
      exaArrayAppend(mesh->maskIds,&id);
    }
  exaDebug(h,"\n");

  exaUInt size=exaArrayGetSize(mesh->maskIds);

  exaInt *ids; exaCalloc(size,&ids);
  maskID *ptr=exaArrayGetPointer(mesh->maskIds);
  exaDebug(h,"Masked ids: ");
  for(i=0;i<size;i++){
    ids[i]=ptr[i].id;
    exaDebug(h,"%d ",ids[i]);
  }
  exaDebug(h,"\n");

  /* copy masks to device */
  exaVectorCreate(h,size,exaInt_t,&mesh->d_maskIds);
  exaVectorWrite(mesh->d_maskIds,ids);
  exaFree(ids);

  return 0;
}

static int buildKernels(exaSettings s,exaMesh mesh){
  exaHandle h; exaMeshGetHandle(mesh,&h);

  const char *kernelDir;
  exaSettingsGet(&kernelDir,"hmholtz::kernel_dir",s);

  char fname[BUFSIZ];
  strcpy(fname,kernelDir);
  int pathLength=strlen(kernelDir);

  strcpy(fname+pathLength,"/hmholtz");
  exaDebug(h,"Hmholtz operator kernel=%s\n",fname);

  exaProgram p;
  exaProgramCreate(h,fname,s,&p);
  exaKernelCreate(p,"BK5",&mesh->hmholtzAx);
  exaProgramFree(p);

  return 0;
}

int exaMeshSetup(exaMesh mesh,exaSettings s){
  exaHandle h; exaMeshGetHandle(mesh,&h);
  exaDebug(h,"[exaMeshSetup] %s:%d\n",__FILE__,__LINE__);

  assert(exaMeshInitialized(mesh)!=0);

  exaUInt nx1=exaMeshGet1DDofs(mesh);
  exaUInt ndim=exaMeshGetDim(mesh);
  exaUInt ngeom=exaMeshGetNGeom(mesh);
  exaUInt elemDofs=exaMeshGetDofsPerElement(mesh);

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

  gatherScatterSetup(mesh);
  copyDataToDevice(mesh);
  buildKernels(s,mesh);

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
  exaDestroy(mesh->hmholtzAx);

  exaFree(mesh);

  return 0;
}
