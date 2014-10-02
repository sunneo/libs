#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
int main(int argc, char *argv[])
{
   Stack* stack = stack_create(0);
   int i;
   for(i=1000; i>=0; --i)
      stack_push(stack,(void*)i);
   while(!stack_empty(stack)){
      printf("%d ",stack_top(stack));
      stack_pop(stack);
   }
   stack_delete(stack);
   printf("\n");
   return 0;
}
