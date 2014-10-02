#include "powerset.h"
#include <stdio.h>
static void powerset_show(Powerset* pset){
   Iterator* iter = powerset_fetch_iter(pset);
   while(iter_hasNext(iter)){
      printf("%c",*(char*)iter_next(iter));
   }
   printf("\n");
}

int main(){
   char str[] = "abcdefghijklmnopqrstuvwxyz";
   Powerset* pset = powerset_create(str,sizeof(char),5);
   while(powerset_hasNext(pset)){
      powerset_show(powerset_next(pset));
   }
   powerset_delete(pset);
   return 0;
}
