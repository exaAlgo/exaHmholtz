#ifndef _EXA_HMHOLTZ_IMPL_H_
#define _EXA_HMHOLTZ_IMPL_H_

struct exaHmholtz_private{
  exaHandle h;
  exaProgram prog;
  exaKernel Ax;
  exaVector d_x,d_b,d_r;
};

#endif
