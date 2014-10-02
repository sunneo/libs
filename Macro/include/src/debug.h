#ifndef DEBUG_H_
#define DEBUG_H_
#include <conio.h>
#include <stdio.h>

#if (defined(_DEBUG) && (_DEBUG != 0))
  #define Debug(msg) msg
#else
  #define Debug(msg)
#endif

#define Error(x,_bug_MSG_func_)       \
MACROBegin                            \
  if((x))                             \
  {                                   \
     Debug(_bug_MSG_func_);           \
     getch();                         \
     exit(-1);                        \
  }                                   \
MACROEnd


#endif
