#include <stdio.h>
#include "coroutine.h"
static void testfnc(void* parm){ 
   int i;
   for(i=0; i<10; ++i){
      fprintf(stderr,"Hello World:%d\n",i);
      fiber_yieldi(i);
   } 
}

int main(){
   Fiber* fiber;
   fiber = fiber_create(testfnc,NULL);
   fiber_start(fiber);
   while(!fiber_terminated(fiber)){
      fprintf(stderr,"Main get %d\n",fiber_get_yieldi(fiber));
      fiber_resume(fiber);
   }
   fiber_delete(fiber);
   return 0;
}
