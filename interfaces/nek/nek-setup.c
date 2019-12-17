#include "nek-setup.h"

#include <stdlib.h>
#include <unistd.h>

nekData_private nekData;

void mkSIZE(int lx1,int lxd,exaInt lelt,exaLong lelg,int ldim,
  int lpmin,int ldimt)
{
  char line[BUFSIZ];
  char fname[BUFSIZ];
  char cmd[BUFSIZ];

  const char *cache_dir = getenv("NEKRS_CACHE_DIR");
  const char *nekrs_nek5000_dir = getenv("EXA_NEK5000_DIR");

  // Read and generate the new size file.
  sprintf(fname,"%s/core/SIZE.template", nekrs_nek5000_dir);
  FILE *fp = fopen(fname,"r");
  char *sizeFile;
  if (fp) {
    fseek(fp, 0, SEEK_END);
    long length = ftell(fp);
    rewind(fp);
    // allocate actual length + some buffer
    sizeFile = (char *) calloc(length+500,sizeof(char));
    if(!sizeFile) {
      fprintf(stderr, "Error allocating space for SIZE file.\n");
      exit(EXIT_FAILURE);
    }
  } else {
    fprintf(stderr,"Error opening %s/core/SIZE.template!\n",
      nekrs_nek5000_dir);
    exit(EXIT_FAILURE);
  }

  int count = 0;
  while(fgets(line,BUFSIZ,fp)!=NULL){
    if(strstr(line, "parameter (lx1=") != NULL) {
      sprintf(line, "      parameter (lx1=%d)\n", lx1);
    } else if(strstr(line, "parameter (lxd=") != NULL) {
      sprintf(line, "      parameter (lxd=%d)\n", lxd);
    } else if(strstr(line, "parameter (lelt=") != NULL) {
      sprintf(line, "      parameter (lelt=%d)\n", lelt);
    } else if(strstr(line, "parameter (lelg=") != NULL) {
      sprintf(line, "      parameter (lelg=%lld)\n", lelg);
    } else if(strstr(line, "parameter (ldim=") != NULL) {
      sprintf(line, "      parameter (ldim=%d)\n", ldim);
    } else if(strstr(line, "parameter (lpmin=") != NULL) {
      sprintf(line, "      parameter (lpmin=%d)\n", lpmin);
    } else if(strstr(line, "parameter (ldimt=") != NULL) {
      sprintf(line, "      parameter (ldimt=%d)\n", ldimt);
    } else if(strstr(line, "parameter (mxprev=") != NULL) {
      sprintf(line, "      parameter (mxprev=%d)\n", 1);
    } else if(strstr(line, "parameter (lgmres=") != NULL) {
      sprintf(line, "      parameter (lgmres=%d)\n", 1);
    } else if(strstr(line, "parameter (lorder=") != NULL) {
      sprintf(line, "      parameter (lorder=%d)\n", 1);
    } else if(strstr(line, "parameter (lelr=") != NULL) {
      sprintf(line, "      parameter (lelr=%d)\n", 128*lelt);
    }

    strcpy(sizeFile+count,line);
    count+=strlen(line);
  }
  fclose(fp);

  // read size if exists
  sprintf(fname,"%s/SIZE",cache_dir);
  fp=fopen(fname,"r"); 

  int writeSize=0,oldVal;
  size_t len;
  if(fp!=NULL) {
    while(getline((char**)&line,&len,fp)>0) {
      if(strstr(line,"lelg=")!=NULL) {
          sscanf(line,"%*[^=]=%d",&oldVal);
          if(oldVal<lelg) writeSize = 1; 
      }
      if(strstr(line,"lelt=")!=NULL) {
          sscanf(line,"%*[^=]=%d",&oldVal);
          if(oldVal<lelt) writeSize = 1; 
      }
      if(strstr(line,"lx1=")!=NULL) {
          sscanf(line,"%*[^=]=%d",&oldVal);
          if(oldVal!=lx1) writeSize = 1;
      }
      if(strstr(line,"ldimt=")!=NULL) {
          sscanf(line,"%*[^=]=%d",&oldVal);
          if(oldVal<ldimt) writeSize = 1;
      }
    }
  }
  fclose(fp);

  if(writeSize) {
    fp = fopen(fname,"w");
    fputs(sizeFile,fp);
    fclose(fp);
    free(sizeFile);
  } else {
    printf("using existing SIZE file %s/SIZE\n", cache_dir);
  }

  fflush(stdout);
}

int buildNekCase(exaSettings s){
  printf("building nek ... "); fflush(stdout);

  char buf[BUFSIZ];
  //TODO
  const char *casename;
  const char *cache_dir;
  const char *nek5000_dir;
  const char *exa_dir;

  FILE *fp;
  int retval;

  sprintf(buf,"%s.re2",casename);
  fp=fopen(buf,"r");
  if(!fp){
    printf("\nERROR: Cannot find %s!\n", buf);
    exit(EXIT_FAILURE);;
  }
  fgets(buf, 80, fp);
  fclose(fp);

  // TODO set from settings
  int N,np,ldimt;

  char ver[10];
  int nelgv, nelgt, ndim;
  sscanf(buf,"%5s %9d %1d %9d",ver,&nelgt,&ndim,&nelgv);
  int lelt=nelgt/np+2;
  mkSIZE(N+1,1,lelt,nelgt,ndim,np,ldimt);

  // Copy case.usr file to cache_dir
  sprintf(buf,"%s.usr",casename);
  if(access(buf,F_OK)!=-1){
    sprintf(buf,"cp -pf %s.usr %s/ >build.log 2>&1",casename,
      cache_dir);
  } else {
    sprintf(buf,"cp -pf %s/core/zero.usr %s/%s.usr >>build.log 2>&1",
      nek5000_dir,cache_dir,casename);
  }
  retval=system(buf);
  if (retval) exit(EXIT_FAILURE);;

  // Copy Nek5000/core from install_dir to cache_dir
  sprintf(buf,"cp -pr %s %s/",nek5000_dir,cache_dir);
  retval = system(buf);
  if (retval) exit(EXIT_FAILURE);;

  //TODO: Fix hardwired compiler flags 
  const char *fc;
  const char *cc;
  const char *fflags;
  const char *cflags;

  sprintf(buf,"cd %s && FC=%s CC=%s"
    "FFLAGS=%s CFLAGS=%s PPLIST=\"DPROCMAP PARRSB\" "
    "NEK_SOURCE_ROOT=%s/nek5000 %s/nek5000/bin/nekconfig %s "
    ">>build.log 2>&1",fc,cc,cache_dir,fflags,cflags,cache_dir,
    cache_dir,casename);
  retval = system(buf);
  if (retval) exit(EXIT_FAILURE);;

  //TODO
  const char *exa_interfaces_dir;

  sprintf(buf,"cd %s && EXA_WORKING_DIR=%s make -j4 -f "
    "%s/nek/share/Makefile nekInterface >>build.log 2>&1",
    cache_dir,cache_dir,exa_interfaces_dir);
  retval = system(buf);
  if (retval) exit(EXIT_FAILURE);;

  printf("done\n\n"); 
  fflush(stdout);
  sync();
  return 0;
}

int nekSetup(exaHandle h,exaSettings s) {
  //TODO
  MPI_Comm c=exaGetMPIComm(h);
  MPI_Fint nek_comm = MPI_Comm_c2f(c);

  //TODO: get this from settings
  const char *casename;

  //TODO: Build nek case here
  int rank; MPI_Comm_rank(c,&rank);
  if(rank==0) buildNekCase(s);

  //TODO read from settings
  int nscal=1;

  char cwd[FILENAME_MAX];
  getcwd(cwd,sizeof(cwd));

  nek_interface_init(nek_comm,(char *)cwd,(char *)casename,nscal);

  nekData.param = (double *) nek_ptr("param");
  nekData.ifield = (int *) nek_ptr("ifield");
  nekData.istep = (int *) nek_ptr("istep");
  nekData.time = (double *) nek_ptr("time");

  nekData.ndim = *(int *) nek_ptr("ndim");
  nekData.nelt = *(int *) nek_ptr("nelt");
  nekData.nelv = *(int *) nek_ptr("nelv");
  nekData.lelt = *(int *) nek_ptr("lelt");
  nekData.nx1 =  *(int *) nek_ptr("nx1");

  nekData.vx = (double *) nek_ptr("vx");
  nekData.vy = (double *) nek_ptr("vy"); 
  nekData.vz = (double *) nek_ptr("vz");
  nekData.pr = (double *) nek_ptr("pr");
  nekData.t  = (double *) nek_ptr("t");

  nekData.ifgetu = (int *) nek_ptr("ifgetu");
  nekData.ifgetp = (int *) nek_ptr("ifgetp");

  nekData.unx = (double *) nek_ptr("unx");
  nekData.uny = (double *) nek_ptr("uny"); 
  nekData.unz = (double *) nek_ptr("unz");
 
  nekData.xm1 = (double *) nek_ptr("xm1");
  nekData.ym1 = (double *) nek_ptr("ym1"); 
  nekData.zm1 = (double *) nek_ptr("zm1");
  nekData.xc = (double *) nek_ptr("xc");
  nekData.yc = (double *) nek_ptr("yc"); 
  nekData.zc = (double *) nek_ptr("zc");

  nekData.ngv = *(long long *) nek_ptr("ngv");
  nekData.glo_num = (long long *) nek_ptr("glo_num");
  nekData.cbscnrs = (double *) nek_ptr("cb_scnrs");
  nekData.cbc = (char *) nek_ptr("cbc");
  nekData.boundaryID = (int *) nek_ptr("boundaryID");
  nekData.eface1 = (int *) nek_ptr("eface1");
  nekData.eface = (int *) nek_ptr("eface");
  nekData.icface = (int *) nek_ptr("icface");
  nekData.comm = MPI_Comm_f2c(*(int *) nek_ptr("nekcomm"));

  return 0;
}
