#include "heap_nr.h"
#include <stdio.h>
#include <stdlib.h>


int cmp(const void* a,const void *b){
   return *(double*)a > *(double*)b;
}

int main(){
   HeapNr* h = heapnr_create(cmp);
   int i;
   double* d = (double*)malloc(sizeof(double));
   *d = 5.0;
   heapnr_push(h,d);
   heapnr_pop(h);
   for(i=0; i<10000; ++i){
      d = (double*)malloc(sizeof(double));
      *d = rand()/32767.0;
      heapnr_push(h,d);
   }
   while(!heapnr_empty(h)){
       printf("%lf\n",*(double*)heapnr_top(h));
       free(heapnr_top(h));
       heapnr_pop(h);
   }
   heapnr_delete(h);
   return 0;
}


