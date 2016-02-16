#ifndef UCTX_COROUTINE_H
#define UCTX_COROUTINE_H
#define __debug(x) x
#define __debugfv(FMT,...) __debug(fprintf(stderr,"%s %d %s:" FMT "\n",__FILE__,__LINE__,__func__, __VA_ARGS__))
#define __debugs(s) __debug(fprintf(stderr,"%s %d %s:%s\n",__FILE__,__LINE__,__func__, s))
#include <stdint.h>
#include <ucontext.h>
typedef union FiberYieldValue{
      void* pvoid;
      uint8_t uint8v;
      uint16_t uint16v;
      uint32_t uint32v;
      uint64_t uint64v;
      int intv;
      short shortv;
      long longv;
      long long longlongv;
      unsigned long long unsignedlonglongv;
      int8_t int8v;
      int16_t int16v;
      int32_t int32v;
      int64_t int64v;
      size_t sizetv;
      ssize_t ssizetv;
      float floatv;
      double doublev;
}FiberYieldValue;

typedef struct Fiber{
   ucontext_t ctx;
   int id;
   int8_t status;
   FiberYieldValue value;
   void* parm;
   void (*fnc)(void*);
}Fiber;

Fiber* fiber_create(void(*fnc)(void*),void* parm);
void fiber_resume(Fiber* fiber);
void fiber_start(Fiber* fiber);
int fiber_terminated(Fiber* fiber);
void fiber_delete(Fiber* fiber);

void fiber_yield(FiberYieldValue v);
void fiber_yieldi(int i);
void fiber_yieldf(float f);
void fiber_yieldlf(double lf);
void fiber_yieldll(uint64_t ll);
void fiber_yieldv(void* v);
FiberYieldValue fiber_get_yield(Fiber* f);
int fiber_get_yieldi(Fiber* f);
void* fiber_get_yieldv(Fiber* f);
float fiber_get_yieldf(Fiber* f);
double fiber_get_yieldlf(Fiber* f);
uint64_t fiber_get_yieldll(Fiber* f);
#endif
