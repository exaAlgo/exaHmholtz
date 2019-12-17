#if !defined(NEK_SETUP_)
#define NEK_SETUP_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>
#include <mpi.h>

#include <exa.h>
#include <nek.h>

typedef struct {
  double *param;

  int *istep;
  int *ifield;

  /* x,y and z co-ordinates */
  double *xm1, *ym1, *zm1;
  double *xc, *yc, *zc;

  double *unx, *uny, *unz;

  double *time;

  /* solution */
  double *vx, *vy, *vz;
  double *pr;
  double *t;

  int *ifgetu, *ifgetp;

  double *cbscnrs;

  /* global vertex ids */
  long long *glo_num, ngv;

  /* Boundary data */
  char *cbc;
  int *boundaryID;

  int NboundaryIDs;

  /* id to face mapping */
  int *eface1, *eface, *icface;

  /* dimension of the problem */
  int ndim;
  /* local problem size */
  int nelv, nelt;
  int lelt;
  /* polynomial order + 1*/
  int nx1;

  /* MPI communicator */
  MPI_Comm comm;
} nekData_private;

extern nekData_private nekData;

int buildNekCase(exaSettings s);
//int buildNekCase(const char *casename,const char *nek5000_dir,
//  int ldimt,int N,int np);
int nekSetup(exaHandle h,exaSettings options);

#endif
