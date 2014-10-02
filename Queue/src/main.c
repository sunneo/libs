#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
int main(){
   int i = 0;
   Queue* queue = queue_create(sizeof(int));
   for(i=0; i<1000; ++i)
	  queue_push(queue,&i);
   while(!queue_empty(queue)){
	  i = *(int*)queue_front(queue);
	  queue_pop(queue);
	  printf("%d ",i);
   }
   queue_delete(queue);
   printf("\n");
   return 0;
}
