#include "permutation.h"
#include <stdlib.h>
#include <stdio.h>
#define ARRAYLEN 10
static void perm_show(Permutation* perm){
   Iterator* iter = perm_fetch_iter(perm);
   while(iter_hasNext(iter)){
      printf("%c",*(char*)iter_next(iter));
   }
   printf("\n");
}

int main(){
   char str[ARRAYLEN] = "abcdefghijklmnopqrstuvwxyz0123456789";
   Permutation* perm = perm_create(str,sizeof(char),ARRAYLEN);
   while(perm_hasNext(perm)){
      perm_show(perm_next(perm));
   }
   perm_delete(perm);
   return 0;
}
