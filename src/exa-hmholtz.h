#ifndef _EXA_HMHOLTZ_H_
#define _EXA_HMHOLTZ_H_

#include <exa.h>

typedef struct exaHmholtz_private *exaHmholtz;

int exaHmholtzCreate(exaHandle h,exaHmholtz *solver);
int exaHmholtzSetup (exaHmholtz solver);
int exaHmholtzDestroy(exaHmholtz solver);

#endif
