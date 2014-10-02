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

static struct Hashtable* address_tbl(){
   static struct Hashtable* ret;
   if(ret == NULL){
      ret = hashtbl_new(0,0);
   }
   return ret;
}

static void address_tbl_add(size_t input, void* obj){
   const static size_t Kilo = 1024;
   const static size_t Mega = 1024*1024;
   const static size_t Giga = 1024*1024*1024;
   size_t g,m,k,b;
   g = input / Giga;
   m = (input % Giga) / Mega;
   k = (input % Mega) / Kilo;
   b = (input % Kilo);

}

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
