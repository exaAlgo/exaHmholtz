#ifndef _EXA_FORTRAN_HMHOLTZ_H_
#define _EXA_FORTRAN_HMHOLTZ_H_

#include <exa-fortran.h>

typedef int exaFortranMesh;
typedef int exaFortranHmholtz;

exaMesh exaMeshF2C(exaFortranMesh mesh);
exaHmholtz exaHmholtzF2C(exaFortranHmholtz hmholtz);

#endif
