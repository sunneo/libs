#include "permutation.h"
#include "arrayiter.h"
#include <stdlib.h>

static void swap_int(int* a,int *b){
   int t = *a;
   *a = *b ;
   *b = t;
}

static void swap_c(char* a,char* b,int elesize){
   char t;
   while(elesize--){
      t = *a;
      *a = *b;
      *b = t;
      ++a;
      ++b;
   }
}

Permutation* perm_create(void* array,int elesize,int size){
   Permutation* perm = (Permutation*)malloc(sizeof(Permutation));
   int i = 0;
   perm->permArray = array;
   perm->size = size;
   perm->isLast = 0;
   perm->numArrayRef = (int*)malloc(sizeof(int)*size);
   perm->fetched = 0;
   for(i=0; i<size; ++i){
      perm->numArrayRef[ i ] = i;
   }
   perm->iter = (Iterator*)arrayiter_create(perm->permArray,elesize,size);
   perm->elesize = elesize;
   return perm;
}

static int perm_step1(Permutation* perm){
   int j=perm->size-1; 
   while(j > 0){
      if(perm->numArrayRef[ j-1 ] < perm->numArrayRef[ j ]){
         break;        
      }
      --j;
   }
   if(j == 0){
      perm->isLast = 1;
      return j;
   }
   return j-1;
}

static int perm_step2(Permutation* perm,int j){
   int l=perm->size-1;
   while(l > 0){
      if(perm->numArrayRef[ j ] < perm->numArrayRef[ l ]){
         break;
      }
      --l;        
   }
   return l;
}

static void* perm_array_idx_of(const Permutation* perm,int idx){
   return (void*)(((char*)perm->permArray)+idx * perm->elesize);
}

static int perm_step3(Permutation* perm,int j,int l){
   swap_int(&perm->numArrayRef[j],&perm->numArrayRef[l]);
   swap_c((char*)perm_array_idx_of(perm,j),(char*)perm_array_idx_of(perm,l),perm->elesize);
}

static int perm_step4(Permutation* perm,int j,int n){
   while(j < n){
      ++j;
      --n;
      perm_step3(perm,j,n);
   }
}
void perm_dropData(Permutation* perm){
   if(perm){
      perm->fetched = 1;
   }
}

Permutation* perm_next(Permutation* perm){
   int j,l;
   if(!perm) return 0;
   if(perm->fetched != 0){
      if(perm->isLast !=0 ) {
         return perm;
      }
      j = perm_step1(perm);
      l = perm_step2(perm,j);
      perm_step3(perm,j,l);
      perm_step4(perm,j,perm->size);
      perm_step1(perm);
   }
   return perm;
}

Iterator* perm_fetch_iter(Permutation* perm){
   if(!perm) return 0;
   iter_reset(perm->iter); 
   perm->fetched = 1;
   return perm->iter;
}

int perm_hasNext(const Permutation* perm){
   if(!perm) return 0;
   return (perm->isLast == 0);
}

void perm_delete(Permutation* perm){
   if(!perm) return;
   arrayiter_delete((ArrayIterator*)perm->iter);
   free(perm->numArrayRef);
}


