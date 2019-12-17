#include "nek.h"

static void (*usrdat_ptr)(void);
static void (*usrdat2_ptr)(void);
static void (*usrdat3_ptr)(void);
static void (*userchk_ptr)(void);
static void (*uservp_ptr)(void);
static void (*userf_ptr)(void);
static void (*userq_ptr)(void);
static void (*userbc_ptr)(void);
static void (*useric_ptr)(void);
static void (*userqtl_ptr)(void);
static void (*usrsetvert_ptr)(void);

static void (*nek_ptr_ptr)(void **, char *, int*);
static void (*nek_outfld_ptr)(void);
static void (*nek_uic_ptr)(int *);
static void (*nek_end_ptr)(void);
static void (*nek_restart_ptr)(char *, int *);
static void (*nek_map_m_to_n_ptr)(double *a,int *na,double *b,
  int *nb, int *if3d,double *w, int *nw);
static void (*nek_outpost_ptr)(double *v1,double *v2,double *v3,
  double *vp,double *vt, char *name, int);
static void (*nek_uf_ptr)(double *, double *, double *);
static int  (*nek_lglel_ptr)(int *);
static void (*nek_setup_ptr)(int *, char *, char *, int*, int, int);
static void (*nek_ifoutfld_ptr)(int *);
static void (*nek_setics_ptr)(void);
static int  (*nek_bcmap_ptr)(int *, int*);
static int  (*nek_nbid_ptr)(void);
static void (*nek_get_coarse_galerkin_ptr)(double*,double*,int*,
  int*,double*,double*);

void noop_func(void) {}

void *nek_ptr(const char *id){
  void *ptr;
  int len = strlen(id);
  (*nek_ptr_ptr)(&ptr, (char*)id, &len);
  return ptr;
}

void nek_outfld(){
  (*nek_outfld_ptr)();
}

void nek_uic(int ifield){
  (*nek_uic_ptr)(&ifield);
}

void nek_end(){
  (*nek_end_ptr)();
}

void nek_map_m_to_n(double *a, int na, double *b, int nb) {
  // output a, intput b
  int if3d = (*(int *)nek_ptr("ndim") == 3);

  int maxn = na > nb ? na : nb;
  int N = 2*maxn*maxn*maxn;
  double *w = (double *) malloc(sizeof(double)*N);

  (*nek_map_m_to_n_ptr)(a, &na, b, &nb, &if3d, w, &N);
  free(w);
}

void nek_get_coarse_galerkin(double *a,double lambda,int nx1,int nxc,int ndim, int nelv){
  int ncr=nxc*nxc;
  int workSize=nx1*nx1*nelv;
  if(ndim==3){
    workSize*=nx1;
    ncr*=nxc;
  }

  double *w1=(double*)calloc(workSize,sizeof(double));
  double *w2=(double*)calloc(workSize,sizeof(double));

  (*nek_get_coarse_galerkin_ptr)(a,&lambda,&ncr,&nxc,w1,w2);

  free(w1); free(w2);
}

void nek_uf(double *u, double *v, double *w)
{
  (*nek_uf_ptr)(u, v, w);
}

int nek_lglel(int e)
{
  int ee = e+1;
  return (*nek_lglel_ptr)(&ee) - 1;
}

void nek_ifoutfld(int i)
{
  (*nek_ifoutfld_ptr)(&i);
}

void nek_setics(void)
{
  (*nek_setics_ptr)();
}

void nek_userchk(void)
{
  (*userchk_ptr)();
}

void nek_restart(char *str,int len){
  (*nek_restart_ptr)(str,&len);
}

int nek_nbid(void){
  return (*nek_nbid_ptr)();
}

DEFINE_USER_FUNC(usrdat)
DEFINE_USER_FUNC(usrdat2)
DEFINE_USER_FUNC(usrdat3)
DEFINE_USER_FUNC(uservp)
DEFINE_USER_FUNC(userf)
DEFINE_USER_FUNC(userq)
DEFINE_USER_FUNC(userbc)
DEFINE_USER_FUNC(useric)
DEFINE_USER_FUNC(usrsetvert)
DEFINE_USER_FUNC(userqtl)

void check_error(char *error) {
  if(error != NULL) {
    fprintf(stderr, "Error: %s\n", error);
    exit(EXIT_FAILURE);
  }
}

void set_function_handles(const char *session_in,int verbose) {
  // load lib{session_in}.so
  char lib_session[BUFSIZ], *error;

  const char *cache_dir = getenv("NEKRS_CACHE_DIR");
  sprintf(lib_session, "%s/lib%s.so", cache_dir, session_in);

  void *handle = dlopen(lib_session,RTLD_NOW|RTLD_GLOBAL);
  if(!handle) {
    fprintf(stderr, "%s\n", dlerror());
    exit(EXIT_FAILURE);
  }

  // check if we need to append an underscore
  char us[2] = "";
  char func[BUFSIZ];
  usrdat_ptr = (void (*)(void)) dlsym(handle, "usrdat_");
  if (usrdat_ptr) strcpy(us,"_");
  dlerror();

#define fname(s) (strcpy(func,(s)), strcat(func, us), func)

  usrdat_ptr = (void (*)(void)) dlsym(handle, fname("usrdat"));
  check_error(dlerror());
  usrdat2_ptr = (void (*)(void)) dlsym(handle, fname("usrdat2"));
  check_error(dlerror());
  usrdat3_ptr = (void (*)(void)) dlsym(handle, fname("usrdat3"));
  check_error(dlerror());
  userchk_ptr = (void (*)(void)) dlsym(handle, fname("userchk"));
  check_error(dlerror());

  nek_ptr_ptr = (void (*)(void **, char *, int *)) dlsym(handle,
    fname("nekf_ptr"));
  check_error(dlerror());
  nek_setup_ptr = (void (*)(int *,char*,char*,int*, int, int))
    dlsym(handle, fname("nekf_setup"));
  check_error(dlerror());
  nek_uic_ptr = (void (*)(int *)) dlsym(handle, fname("nekf_uic"));
  check_error(dlerror());
  nek_end_ptr = (void (*)(void)) dlsym(handle, fname("nekf_end"));
  check_error(dlerror());
  nek_outfld_ptr = (void (*)(void)) dlsym(handle,
    fname("nekf_outfld"));
  check_error(dlerror());
  nek_restart_ptr = (void (*)(char *,int *)) dlsym(handle,
    fname("nekf_restart"));
  check_error(dlerror());
  nek_uf_ptr = (void (*)(double *, double *, double *))
    dlsym(handle,fname("nekf_uf"));
  check_error(dlerror());
  nek_lglel_ptr = (int (*)(int *)) dlsym(handle,fname("nekf_lglel"));
  check_error(dlerror());
  nek_ifoutfld_ptr = (void (*)(int *)) dlsym(handle,
    fname("nekf_ifoutfld"));
  check_error(dlerror());
  nek_setics_ptr=(void (*)(void)) dlsym(handle,fname("nekf_setics"));
  check_error(dlerror());
  nek_bcmap_ptr = (int (*)(int *, int *)) dlsym(handle,
    fname("nekf_bcmap"));
  check_error(dlerror());
  nek_map_m_to_n_ptr = (void (*)(double*,int*,double*,int*,int*,
    double *, int *)) dlsym(handle, fname("map_m_to_n"));
  check_error(dlerror());
  nek_nbid_ptr = (int (*)(void)) dlsym(handle,fname("nekf_nbid"));
  check_error(dlerror());
  nek_get_coarse_galerkin_ptr=(void(*)(double*,double*,int*,int*,
    double*,double*)) dlsym(handle,
    fname("nekf_get_coarse_galerkin"));
  check_error(dlerror());

#define postfix(x) x##_ptr
#define load_or_noop(s)\
  do {\
    postfix(s)=(void (*)(void)) dlsym(handle,fname(#s));\
    if(!(postfix(s))){\
      postfix(s)=noop_func;\
      if(verbose) printf("Setting function " #s " to noop_func.\n");\
    } else if(verbose){\
      printf("Loading " #s " from lib%s.so\n",session_in);\
    }\
  } while (0)

  load_or_noop(uservp);
  load_or_noop(userf);
  load_or_noop(userq);
  load_or_noop(userbc);
  load_or_noop(useric);
  load_or_noop(userqtl);
  load_or_noop(usrsetvert);

#undef fname
#undef postfix
#undef load_or_noop
}

int nek_bcmap(int bid, int ifld) {
  return (*nek_bcmap_ptr)(&bid, &ifld);
}

void nek_interface_init(MPI_Fint nek_comm,char *cwd,char *casename,
  int nscal)
{
  set_function_handles(casename,0);
  (*nek_setup_ptr)(&nek_comm,cwd,casename,&nscal,strlen(cwd),
    strlen(casename));
}
