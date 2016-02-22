#include <setjmp.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "coroutine.h"
static Fiber* co_sched;
static Fiber* co_current;

Fiber* fiber_create(void(*fnc)(void*),void* param){
   Fiber* ret = (Fiber*)malloc(sizeof(Fiber));
   memset(ret,0,sizeof(Fiber));
   ret->fnc = fnc;
   ret->param = param;
   ret->status = 0;
   if(!co_current){
      if(!co_sched){
         co_sched = (Fiber*)malloc(sizeof(Fiber));
         memset(co_sched,0,sizeof(Fiber));
      }
      co_current=co_sched;
   }
   ret->parent = co_current;
   return ret;
}

void fiber_start(Fiber* fiber){
   if(co_current == NULL){
      co_current = co_sched;
      co_current->stkbase=(void*)(((size_t)((char*)(&fiber)))+sizeof(void*));

   }
   if(setjmp(co_current->ctx) == 0){
      fiber->status=1;
      fiber->stkbase=(void*)(((size_t)((char*)(&fiber)))+sizeof(void*));
      co_current=fiber;
      fiber->fnc(fiber->param);
      co_current->status=-1;
      co_current=co_current->parent;
   }

}


static void fiber_yieldto(Fiber* fiber){
   void* base;
   size_t newstksiz=0;
   size_t allocstksiz=0;
   base = &base;
   if((size_t)base > (size_t)co_current->stkbase){
      newstksiz=((size_t)base) - (size_t)co_current->stkbase;
      co_current->overridePtr=co_current->stkbase;
   }
   else{
      newstksiz= (size_t)co_current->stkbase - (size_t)base;
      co_current->overridePtr=base;
   }
   allocstksiz=((newstksiz+0xfff)&(~(0xfff))); // at least 1 page
   if(co_current->stk == NULL){
      co_current->stk = malloc(allocstksiz);
   }
   else{
      if(co_current->stksiz != newstksiz){
         co_current->stk = realloc(co_current->stk,allocstksiz);
      }
   }
   co_current->stksiz = newstksiz;
   if(setjmp(co_current->ctx) == 0){
      memcpy(co_current->stk,co_current->overridePtr,allocstksiz);
      co_current = fiber;
      co_current->status = 1;
      longjmp(co_current->ctx,1);
   }
   else{
      memcpy(co_current->overridePtr,co_current->stk,co_current->stksiz);
   }
}

void fiber_yield(FiberYieldValue value){
   if(NULL==co_current) return;
   co_current->value = value;
   if(co_current->parent != NULL){
      fiber_yieldto(co_current->parent);
   }
   else{
      fiber_yieldto(co_sched);
   }
}



void fiber_resume(Fiber* fiber){
   if( fiber->status > 0 ){
      co_current->status=2;
      if(setjmp(co_current->ctx) == 0){
         co_current=fiber;
         co_current->status=1;
         longjmp(fiber->ctx,1);
      }
   }
}
int fiber_terminated(const Fiber* fiber){
   if(!fiber) return 1;
   return fiber->status == -1;
}

void fiber_delete(Fiber* fiber){
   if(!fiber) return;
   if(fiber->stk){
      free(fiber->stk);
      fiber->stk=0;
   }
   free(fiber);
}

void fiber_yieldi(int i){   FiberYieldValue val;   val.intv=i;   fiber_yield(val); }
void fiber_yieldf(float f){   FiberYieldValue val;   val.floatv=f;   fiber_yield(val); }
void fiber_yieldlf(double lf){   FiberYieldValue val;   val.doublev=lf;   fiber_yield(val); }
void fiber_yieldll(unsigned long long ll){   FiberYieldValue val;   val.unsignedlonglongv=ll;   fiber_yield(val); }
void fiber_yieldv(void* v){   FiberYieldValue val;   val.pvoid=v;   fiber_yield(val); }
FiberYieldValue fiber_get_yield(Fiber* f){
   return f->value;
}
int fiber_get_yieldi(Fiber* f){ return f->value.intv; }
void* fiber_get_yieldv(Fiber* f){ return f->value.pvoid; }
float fiber_get_yieldf(Fiber* f){ return f->value.floatv; }
double fiber_get_yieldlf(Fiber* f){ return f->value.doublev; }
unsigned long long fiber_get_yieldll(Fiber* f){ return f->value.unsignedlonglongv; }


