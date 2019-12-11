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
  exaUInt size=2;

  MPI_Init(NULL,NULL);

  if(argc!=2){
    printf("Usage: %s <backend>\n",argv[0]);
    exit(0);
  }

  // Create exaHandle
  exaHandle h; exaInit(&h,MPI_COMM_WORLD,argv[1]);
  // Initialize settings
  exaSettings s; exaSettingsInit(h,NULL,&s);
  // Create hmholtz solver
  exaHmholtz hmhz; exaHmholtzCreate(h,s,&hmhz);
  // Create program
  exaProgram p; exaProgramCreate(h,argv[0],s,&p);
  // Create kernel
  exaKernelCreate(p,"Ax",&kernelAx);

  // set weights to all ones
  exaVector one; exaVectorCreate(h,size,&one);
  exaScalar *in; exaMalloc(size,&in);
  exaUInt i;
  for(i=0;i<size;i++) in[i]=1.0;
  exaVectorWrite(one,in);

  srand(time(NULL));
  exaScalar *xx; exaCalloc(size,&xx); populate(xx,size);
#if 0
  printf("xx=\n");
  for(int i=0;i<size;i++){
    printf("%lf ",xx[i]);
  }
  printf("\n");
#endif

  // create matrix A - make sure it is SPD
  exaScalar *A; exaCalloc(size*size,&A); populate(A,size*size);
  for(exaUInt i=0;i<size;i++){
    exaScalar sum=0;
    for(exaUInt j=0;j<i;j++){
      exaScalar s=A[i*size+j]+A[j*size+i];
      A[i*size+j]=A[j*size+i]=0.5*s;
      sum+=s;
    }
    A[i*size+i]+=sum;
  }

#if 0
  printf("A=\n");
  for(int i=0;i<size;i++){
    for(int j=0;j<size;j++){
      //A[i*size+j]=(i>j);
      printf("%lf ",A[i*size+j]);
    }
    printf("\n");
  }
#endif
  exaVectorCreate(h,size*size,&Amatrix); exaVectorWrite(Amatrix,A);

  exaScalar *Axx; exaCalloc(size,&Axx); matVec(A,xx,size,Axx);
#if 0
  printf("Axx=\n");
  for(int i=0;i<size;i++){
    printf("%lf ",Axx[i]);
  }
  printf("\n");
#endif

  exaVector b; exaVectorCreate(h,size,&b); exaVectorWrite(b,Axx);

  // Call pcg
  exaVector x; exaVectorCreate(h,size,&x);
  for(i=0;i<size;i++) in[i]=0.0;
  exaVectorWrite(x,in);

  int nIter=exaHmholtzCG(x,calcAx,b,one,1e-7,100,hmhz);

  // get answer back
  exaScalar *out; exaCalloc(size,&out); exaVectorRead(x,out);

  // compare answer;
  for(i=0;i<size;i++)
    if(fabs(out[i]-xx[i])>1e-6)
      fprintf(stderr,"%lf != %lf\n",out[i],xx[i]);

  exaFree(xx); exaFree(A); exaFree(Axx); exaFree(in); exaFree(out);
  exaDestroy(Amatrix); exaDestroy(x); exaDestroy(b); exaDestroy(one);
  exaDestroy(s); exaDestroy(kernelAx);

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
