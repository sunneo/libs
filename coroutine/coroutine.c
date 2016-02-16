#include <stdlib.h>
#include <string.h>
#include <ucontext.h>
#include <stdint.h>
#include <stdio.h>
#include "coroutine.h"
static int gFiberStackSize=1024*1024*8;

static Fiber* co_sched;
static Fiber* co_current;

static void* fiber_allocate_stack(){
   void* ret;
   ret = (void*)malloc(gFiberStackSize);
   return ret;
}

static void fiber_stub(Fiber* fiber){
   co_current->status = 1;
   fiber->fnc(fiber->parm);
   co_current->status = -1;
}
Fiber* fiber_create(void(*fnc)(void*),void* parm){
   Fiber* ret;
   ret = (Fiber*)malloc(sizeof(Fiber));
   memset(ret,0,sizeof(Fiber));
   if(!co_sched){
      co_sched = (Fiber*)malloc(sizeof(Fiber));
      memset(co_sched,0,sizeof(Fiber));
   }
   ret->fnc = fnc;
   ret->parm = parm;
   getcontext(&ret->ctx);
   ret->ctx.uc_stack.ss_sp = fiber_allocate_stack();
   ret->ctx.uc_stack.ss_size = gFiberStackSize;
   ret->ctx.uc_stack.ss_flags = 0;
   ret->ctx.uc_link=&co_sched->ctx; 
   ret->parent = co_current;
   makecontext(&ret->ctx,(void(*)())fiber_stub,1,ret);
   return ret;
}

static void fiber_yieldto(Fiber* curfib,Fiber* fiber){
   ucontext_t* cur=&curfib->ctx;
   ucontext_t* newctx=&fiber->ctx;
   co_current=fiber;
   swapcontext(cur,newctx);
}

void fiber_start(Fiber* fiber){
   // TODO if sched is null,
   // start a sched which maintains queue.
   // let sched maintains them. 
   if(co_current==NULL){
      co_current = co_sched;
   }
   fiber_yieldto(co_current,fiber);
}
void fiber_delete(Fiber* fiber){
   if(fiber == NULL) return;
   free(fiber->ctx.uc_stack.ss_sp);
   free(fiber);
}

void fiber_yield(FiberYieldValue value){
   if(NULL==co_current) return;
   co_current->value = value;
   if(co_current->parent != NULL){
      fiber_yieldto(co_current,co_current->parent);
   }
   else{
      fiber_yieldto(co_current,co_sched);
   }
}
void fiber_yieldi(int i){   FiberYieldValue val;   val.intv=i;   fiber_yield(val); }
void fiber_yieldf(float f){   FiberYieldValue val;   val.floatv=f;   fiber_yield(val); }
void fiber_yieldlf(double lf){   FiberYieldValue val;   val.doublev=lf;   fiber_yield(val); }
void fiber_yieldll(uint64_t ll){   FiberYieldValue val;   val.uint64v=ll;   fiber_yield(val); }
void fiber_yieldv(void* v){   FiberYieldValue val;   val.pvoid=v;   fiber_yield(val); }
FiberYieldValue fiber_get_yield(Fiber* f){
   return f->value;
}
int fiber_get_yieldi(Fiber* f){ return f->value.intv; }
void* fiber_get_yieldv(Fiber* f){ return f->value.pvoid; }
float fiber_get_yieldf(Fiber* f){ return f->value.floatv; }
double fiber_get_yieldlf(Fiber* f){ return f->value.doublev; }
uint64_t fiber_get_yieldll(Fiber* f){ return f->value.uint64v; }

int fiber_terminated(Fiber* fiber){
   return fiber->status==-1;
}
void fiber_resume(Fiber* fiber){
   if(fiber == NULL) return;
   if(fiber->status==-1) return; // terminated
   fiber_yieldto(co_current,fiber); 
}

