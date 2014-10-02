
#include <stdlib.h>
#include <stdio.h>
#include "include/macro_heap.h"
#define N 3
heap_def(int);

int comp(const int a,const int b)
{
   return a>b;
}

int main()
{
   int i=0;
   heap_decl(int)* h;
   heap_create(h,int,comp,NULL);
   for(i=0; i<10; ++i)
   {
      heap_push(h, i );
      printf("%d\n",heap_top(h));
   }
   printf("\n");
   for(i=0; i<10; ++i)
   {
      printf("%d\n",heap_top(h));
      heap_pop(h);      
   }
   system("pause");
   return 0;
}
