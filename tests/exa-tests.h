#ifndef _EXA_TESTS_H_
#define _EXA_TESTS_H_

#define GET_OKL_NAME(out,in) do{\
  int n=strlen(in);\
  strncpy(out,in,n-2);\
  out[n-2]='\0';\
} while(0);

#endif
