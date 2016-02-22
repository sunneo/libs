#ifndef COROUTINE_JUMPBUF_H_
#define COROUTINE_JUMPBUF_H_

#define __debug(x) x
#define __debugfv(FMT,...) __debug(fprintf(stderr,"%s %d %s:" FMT "\n",__FILE__,__LINE__,__func__, __VA_ARGS__))
#define __debugs(s) __debug(fprintf(stderr,"%s %d %s:%s\n",__FILE__,__LINE__,__func__, s))
#include <stdint.h>
#include <setjmp.h>
typedef union FiberYieldValue{
      void* pvoid;
      int intv;
      short shortv;
      long longv;
      long long longlongv;
      unsigned long long unsignedlonglongv;
      size_t sizetv;
      float floatv;
      double doublev;
}FiberYieldValue;


typedef struct Fiber{
   jmp_buf ctx;
   void* stk;
   void* stkbase;
   void* overridePtr;
   size_t stksiz;
   void* param;
   void (*fnc)(void*);
   int status;
   FiberYieldValue value;
   struct Fiber* parent;
}Fiber;

Fiber* fiber_create(void(*fnc)(void*),void* param);
void fiber_start(Fiber* fiber);
void fiber_yield(FiberYieldValue value);
void fiber_resume(Fiber* fiber);
int fiber_terminated(const Fiber* fiber);
void fiber_delete(Fiber* fiber);

void fiber_yieldi(int i);
void fiber_yieldf(float f);
void fiber_yieldlf(double lf);
void fiber_yieldll(unsigned long long);
void fiber_yieldv(void* v);
FiberYieldValue fiber_get_yield(Fiber* f);
int fiber_get_yieldi(Fiber* f);
void* fiber_get_yieldv(Fiber* f);
float fiber_get_yieldf(Fiber* f);
double fiber_get_yieldlf(Fiber* f);
unsigned long long fiber_get_yieldll(Fiber* f);

#endif
