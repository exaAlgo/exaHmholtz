#include "exa-hmholtz-impl.h"

int exaHmholtzCG(exaVector d_x,exaVector d_f,exaScalar lambda,exaScalar tol,
  int maxit,exaHmholtz hz)
{
  exaInt size=exaVectorGetSize(d_x);
  assert(size==exaVectorGetSize(d_f));

  int nIter;
  return nIter;
}
