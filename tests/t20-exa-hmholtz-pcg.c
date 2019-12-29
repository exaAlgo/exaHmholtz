#include "exa-hmholtz.h"
#include "exa-memory.h"

#include <math.h>
#include <time.h>

// matrix multiplication kernel
static exaKernel kernelAx;
// size x size matrix stored in row major order
static exaVector Amatrix;

int calcAx(exaVector q,exaVector Aq,exaHmholtz hz);
int matVec(exaScalar *A,exaScalar *q,exaUInt size,exaScalar *Aq);
int populate(exaScalar *q,exaUInt size);

int main(int argc,char *argv[])
{
  srand(time(NULL)); // initialize the random number generator
  exaUInt size=10; // set problem size

  MPI_Init(NULL,NULL);

  if(argc!=2){
    printf("Usage: %s <backend>\n",argv[0]);
    exit(0);
  }

  exaHandle h; exaInit(&h,MPI_COMM_WORLD,argv[1]);

  exaSettings s; exaSettingsCreate(h,NULL,&s);
  exaSettingsSet("general::order",getExaInt(10),s);

  exaHmholtz hmhz; exaHmholtzCreate(h,s,&hmhz);
  exaHmholtzSetup(hmhz);

  exaProgram p; exaProgramCreate(h,argv[0],s,&p);
  exaKernelCreate(p,"Ax",&kernelAx);

  // Set answer
  exaScalar *xx; exaCalloc(size,&xx); populate(xx,size);

  // create matrix A - make sure it is SPD
  exaScalar *A; exaCalloc(size*size,&A); populate(A,size*size);
  for(exaUInt i=0;i<size;i++){
    exaScalar sum=0;
    for(exaUInt j=0;j<i;j++){
      exaScalar s=A[i*size+j]+A[j*size+i];
      A[i*size+j]=A[j*size+i]=0.5*s;
      sum+=s;
    }
    A[i*size+i]+=100+fabs(sum);
  }
  exaVectorCreate(h,size*size,exaScalar_t,&Amatrix);
  exaVectorWrite(Amatrix,A);

  // set RHS
  exaScalar *Axx; exaCalloc(size,&Axx); matVec(A,xx,size,Axx);
  exaVector b; exaVectorCreate(h,size,exaScalar_t,&b);
  exaVectorWrite(b,Axx);

  // Call CG with zero initial guess
  exaVector x; exaVectorCreate(h,size,exaScalar_t,&x);
  exaScalar *in; exaMalloc(size,&in);
  exaUInt i;
  for(i=0;i<size;i++) in[i]=0.0;
  exaVectorWrite(x,in);
  int nIter=exaHmholtzCGGeneral(x,calcAx,b,1e-8,100,0,hmhz);

  // nIter <= size should hold true
  assert(nIter<=size);

  // compare answer
  exaScalar *out; exaVectorRead(x,(void**)&out);
  for(i=0;i<size;i++)
    if(fabs(out[i]-xx[i])>1e-7)
      fprintf(stderr,"Error: %lf != %lf\n",out[i],xx[i]);

  exaFree(xx); exaFree(A); exaFree(Axx); exaFree(in);
  exaDestroy(Amatrix); exaDestroy(x); exaDestroy(b);
  exaDestroy(s); exaDestroy(kernelAx);

  exaMeshDestroy(mesh);
  exaHmholtzDestroy(hmhz);
  exaFinalize(h);

  MPI_Finalize();

  return 0;
}

int calcAx(exaVector q,exaVector Aq,exaHmholtz hz){
  exaUInt N=exaVectorGetSize(q );
  exaUInt M=exaVectorGetSize(Aq);
  assert(M==N);

  exaKernelRun(kernelAx,getExaUInt(M),q,Amatrix,Aq);

  return 0;
}

int matVec(exaScalar *A,exaScalar *q,exaUInt size,exaScalar *Aq){
  exaUInt i,j;

  for(i=0;i<size;i++){
    Aq[i]=0;
    for(j=0;j<size;j++)
      Aq[i]+=A[i*size+j]*q[j];
  }

  return 0;
}

int populate(exaScalar *q,exaUInt size){
  exaUInt i=0,M=100;
  for(i=0;i<size;i++) q[i]=(rand()%M)/(M/2+0.1);

  return 0;
}
