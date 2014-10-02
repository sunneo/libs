
/**
 * the following are demo partitions.
 */
  
#include <stdio.h>

#include <time.h>
#define N 1000000

int __CallConv intcmp(const void* a,const void* b){
   return a>b;
}


void __CallConv demo(int n)
{
   Heap* h = heap_create( intcmp );
   int i = 0;
   for(i=0; i<n; ++i){
      heap_push(h,(const void*) rand() );
   }
   printf(" complete 1 \n"); 
   while( heap_pop_return(h) ){
      /*heap_pop( h );*/
   }
   printf(" complete 2 \n"); 
   heap_delete(h);
}

void criteria_evaluate( void (* __CallConv fn)(int),int n )
{
   clock_t c = clock();
   double sec;
   fn(n);
   sec = ((double)(clock()-c))/CLOCKS_PER_SEC;
   printf(" data proceed : %-11d\n",N);
   printf(" time elapsed : %-7.3lf\n",sec);
   printf(" throughput   : %-7.3lf data/sec\n",N/sec);
}

int main(){
   criteria_evaluate(demo,N);
   system("pause");
   return 0;
}
        
