#include <stdio.h>
#include <stdlib.h>
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
   Fiber* fiber[2]={ fiber_create(testfnc,NULL), fiber_create(testfnc,NULL) };
   int i;
   int terminatedCnt=0;
   int loop=0;
   for(i=0; i<2; ++i){
      fiber_start(fiber[i]);
   }
   i = 0;
   while(terminatedCnt<2){
      if(!fiber_terminated(fiber[terminatedCnt])){
         fiber_resume(fiber[terminatedCnt]);
      }
      else{
         fiber_delete(fiber[terminatedCnt]);
         fiber[terminatedCnt]=NULL;
         ++terminatedCnt;
      }
      ++loop;
      if(loop % 5 ==0){
         fprintf(stderr,"cor_main yield at loop %d\n",loop);
         fiber_yieldi(loop);
      }
   }
}

int main(){
   Fiber* fiberMain = fiber_create(cor_main,NULL);
   fiber_start(fiberMain);
   while(!fiber_terminated(fiberMain)){
      fprintf(stderr,"back to main...\n");
      fiber_resume(fiberMain);
   }
   fiber_delete(fiberMain);
   return 0;

}
