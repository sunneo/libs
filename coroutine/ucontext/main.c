#include <stdio.h>
#include "coroutine.h"
int gid=0;
static void testfnc(void* parm){ 
   int i;
   int id = gid++;
   for(i=0; i<10; ++i){
      fprintf(stderr,"[%d] Hello World:%d\n",id,i);
      fiber_yieldi(i);
   } 
}


void cor_main(){
   Fiber* fiber = fiber_create(testfnc,NULL);
   Fiber* fiber2 = fiber_create(testfnc,NULL);
   int terminatedCnt=0;
   int loop=0;
   fiber_start(fiber);
   fiber_start(fiber2);
   while(terminatedCnt<2){
      if(!fiber_terminated(fiber)){
         fprintf(stderr,"Main get %d\n",fiber_get_yieldi(fiber));
         fiber_resume(fiber);
      }
      else{
         ++terminatedCnt;
         fiber_delete(fiber);
         if(fiber2!=NULL) fiber=fiber2;
      }
      ++loop;
      if(loop % 5 ==0){
         fprintf(stderr,"cor_main yield at loop %d\n",loop);
         fiber_yieldi(loop);
      }
   }
}

int main(){
   Fiber* fiber= fiber_create(cor_main,NULL);
   fiber_start(fiber);
   while(!fiber_terminated(fiber)){
      fprintf(stderr,"back to main...\n");
      fiber_resume(fiber);
   }
}
