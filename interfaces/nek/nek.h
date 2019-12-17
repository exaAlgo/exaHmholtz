#ifndef NEK_INTERFACE_H_
#define NEK_INTERFACE_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>

#include "mpi.h"

#define DECLARE_USER_FUNC(a) void nek_ ## a(void);
#define DEFINE_USER_FUNC(a) void nek_ ## a(void) { (* a ## _ptr)(); }

DECLARE_USER_FUNC(usrdat)
DECLARE_USER_FUNC(usrdat2)
DECLARE_USER_FUNC(usrdat3)
DECLARE_USER_FUNC(uservp)
DECLARE_USER_FUNC(userf)
DECLARE_USER_FUNC(userq)
DECLARE_USER_FUNC(userbc)
DECLARE_USER_FUNC(useric)
DECLARE_USER_FUNC(usrsetvert)
DECLARE_USER_FUNC(userqtl)

void* nek_ptr(const char *id);
void  nek_outfld(void);
void  nek_uic(int ifield);
void  nek_end(void);
void  nek_map_m_to_n(double *a, int na, double *b, int nb);
void  nek_outpost(double *v1, double *v2, double *v3, double *vp,
  double *vt, char *name);
int   nek_lglel(int e);
void  nek_uf(double *u, double *v, double *w);
void  nek_ifoutfld(int i);
void  nek_userchk(void);
int   nek_bcmap(int bid, int ifld);
int   nek_nbid(void);
void  nek_get_coarse_galerkin(double *a,double lambda,int nx1,
  int nxc,int ndim, int nelv);
void  nek_restart(char *str,int len);
void  nek_interface_init(MPI_Fint nek_comm,char *cwd,
  char *casename,int nscal);
void  nek_setics(void);

#endif
