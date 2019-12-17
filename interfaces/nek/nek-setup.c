#include <nek-setup.h>
#include <exa-memory.h>

#include <stdlib.h>
#include <unistd.h>

nekData_private nekData;
char *cache_dir;
char *nek5000_dir;
char *exa_dir;

void mkSIZE(exaHandle h,int lx1,int lxd,exaInt lelt,exaLong lelg,
  int ldim,int lpmin,int ldimt)
{
  char fname[BUFSIZ];
  char cmd[BUFSIZ];

  // Read and generate the new size file.
  sprintf(fname,"%s/core/SIZE.template",nek5000_dir);
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
      nek5000_dir);
    exit(EXIT_FAILURE);
  }

  char *line; exaCalloc(BUFSIZ,&line);
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
    fclose(fp);
  } else writeSize=1;

  if(writeSize) {
    exaDebug(h,"Writing new SIZE file: %s/SIZE\n",cache_dir);
    fp = fopen(fname,"w+");
    fputs(sizeFile,fp);
    fclose(fp);
    free(sizeFile);
  } else {
    exaDebug(h,"Using existing SIZE file: %s/SIZE\n",cache_dir);
  }
}

int buildNekCase(const char *casename,exaHmholtz hz){
  exaHandle h;
  exaHmholtzGetHandle(hz,&h);

  exaDebug(h,"Building Nek5000 case: %s\n",casename);

  exaSettings s;
  exaHmholtzGetSettings(hz,&s);

  exa_dir=getenv("EXA_DIR");
  if(exa_dir==NULL)
    exaSettingsGet(&exa_dir,"exa:install_dir",s);

  nek5000_dir=getenv("EXA_NEK5000_DIR");
  if(nek5000_dir==NULL)
    exaSettingsGet(&nek5000_dir,"nek5000::install_dir",s);

  char *last=strrchr(casename,'/');
  exaCalloc(last-casename+10,&cache_dir);
  strncpy(cache_dir,casename,last-casename+1);
  strcpy(cache_dir+strlen(cache_dir),".cache");
  exaDebug(h,"cache dir: %s\n",cache_dir);

  char buf[BUFSIZ];
  sprintf(buf,"mkdir -p %s\n",cache_dir);
  int retval=system(buf);
  if(retval!=0){
    fprintf(stderr,"Can not create cache dir: %s\n",cache_dir);
    exit(EXIT_FAILURE);;
  }

  FILE *fp;
  sprintf(buf,"%s.re2",casename);
  fp=fopen(buf,"r");
  if(!fp){
    fprintf(stderr,"Error: Cannot find %s.re2\n",buf);
    exit(EXIT_FAILURE);;
  }
  fgets(buf,80,fp);
  fclose(fp);

  // TODO: set from settings
  int N=8,np=1,ldimt=1;

  char ver[10];
  int nelgv,nelgt,ndim;
  sscanf(buf,"%5s %9d %1d %9d",ver,&nelgt,&ndim,&nelgv);
  int lelt=nelgt/np+2;
  mkSIZE(h,N+1,1,lelt,nelgt,ndim,np,ldimt);

  // Copy case.usr file to cache_dir
  sprintf(buf,"%s.usr",casename);
  if(access(buf,F_OK)!=-1){
    exaDebug(h,"Using case file: %s.usr\n",casename);
    sprintf(buf,"cp -pf %s.usr %s/ >build.log 2>&1",casename,
      cache_dir);
  } else {
    exaDebug(h,"Using zero.usr file from: %s/core\n",nek5000_dir);
    sprintf(buf,"cp -pf %s/core/zero.usr %s/%s.usr >>build.log 2>&1",
      nek5000_dir,cache_dir,casename);
  }
  retval=system(buf);
  if(retval){
    fprintf(stderr,"Error: Cannot find a case file\n");
    exit(EXIT_FAILURE);
  }

  exaFree(cache_dir);
#if 0
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
#endif
}

int nekSetup(exaMesh *mesh,const char *casename,exaHmholtz hz) {
  exaHandle h;
  exaHmholtzGetHandle(hz,&h);

  MPI_Comm c=exaGetMPIComm(h);
  MPI_Fint nek_comm=MPI_Comm_c2f(c);

  //TODO: Build nek case here
  int rank=exaRank(h);
  if(rank==0) buildNekCase(casename,hz);

#if 0
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
#endif
}
