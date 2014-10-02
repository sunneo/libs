#include "hashtable.h"
#include <stdio.h>
#include <sys/time.h>
int inteq(const void* s1,const void* s2){   return ((int)s1) == ((int)s2);  }

double getTimeOfDay(){
   struct timeval tmv;
   gettimeofday(&tmv,0);
   return (double)tmv.tv_sec+1e-6*tmv.tv_usec;
}
#define N 1000000
double t_put;
double t_remove;
int key[ N ];
int main(){
   struct Hashtable* hash = hashtbl_new(0,(int(*)(const void*,const void*))inteq);
   int i;
   int jj = 0;
   for(i=0; i<N; ++i){
	  key[ i ] = rand()%N+1;
	  t_put -= getTimeOfDay();
	  hashtbl_put(hash,(const void*)key[ i ],(const void*)key[ i ]);
	  t_put += getTimeOfDay();
   }
   for(i=0; i<N; ++i){
	  t_remove -= getTimeOfDay();
	  hashtbl_remove(hash,(const void*)key[ i ]);
	  t_remove += getTimeOfDay();
   }
   hashtbl_delete(hash);
   printf("remove : %lf\n",t_remove);
   printf("put    : %lf\n",t_put);
   return 0;
}
