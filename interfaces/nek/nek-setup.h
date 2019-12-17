#if !defined(NEK_SETUP_)
#define NEK_SETUP_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>
#include <mpi.h>

#include <exa.h>
#include <exa-hmholtz-impl.h>
#include <nek.h>

int buildNekCase(const char *casename,exaHmholtz hz);
int nekSetup(exaMesh *mesh,const char *casename,exaHmholtz hz);

#endif
