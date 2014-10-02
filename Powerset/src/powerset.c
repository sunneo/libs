#include "powerset.h"
#include <stdlib.h>
#include "iter.h"
#include <string.h>
typedef struct {
   Iterator super;
   int idx;
   int elesize;
   int size;
   void* array;
   char* bitset;
}PowersetIter;

static int psetiter_has_next(const PowersetIter* iter){
   return iter->idx < iter->size;
}

static void psetiter_search_next_bit(PowersetIter* iter){
   while(iter->idx < iter->size){
      if(iter->bitset[ iter->idx ] != 0){
         break;
      }
      ++iter->idx;
   }
}

static int powerset_is_last(const Powerset* set){
   int ret = 0;
   int i = 0;
   for(i=0; i<set->size; ++i){
      ret |= (!set->bset[ i ]);
      if(ret) return 0;
   }
   return 1;
}

static void* psetiter_next(PowersetIter* iter){
   void* ret = ((char*)iter->array)+iter->elesize*iter->idx;
   ++iter->idx;
   psetiter_search_next_bit(iter);
   return ret;
}

static void psetiter_reset(PowersetIter* iter){
   iter->idx = 0;
   psetiter_search_next_bit(iter);
}

static void psetiter_remove(PowersetIter* iter){

}

static IteratorVTable PowersetIterVirtualTable = {
   (int(*)(const Iterator*))psetiter_has_next,
   (void*(*)(Iterator*))psetiter_next,
   (void(*)(Iterator*))psetiter_reset,
   (void(*)(Iterator*))psetiter_remove
};


static PowersetIter* psetiter_create(void* array,char* bset,int elesize,int size){
   PowersetIter* iter = (PowersetIter*)malloc(sizeof(PowersetIter));
   iter->idx = 0;
   iter->size = size;
   iter->elesize = elesize;
   iter->bitset = bset;
   iter->array = array;
   iter->super.vtbl = &PowersetIterVirtualTable;
   return iter;
}

static int next_subset( char* a, int cnt ){
  char* r = a+ (cnt);
  char c;
  while(--r >= a && (c = *r) ){
     *r = 0;
  }
  if(!c){
     return *r = 1;
  }
  return 0;
}

Powerset* powerset_create(void* array,int elesize,int size){
   Powerset* ret = (Powerset*)malloc(sizeof(Powerset));
   ret->array = array;
   ret->elesize = elesize;
   ret->size = size;
   ret->bset = (char*)calloc(sizeof(char),size);
   ret->fetched = 0;
   ret->iter = (Iterator*)psetiter_create(array,ret->bset,elesize,size);
   ret->isLast = 0;
   return ret;
}


int powerset_hasNext(const Powerset* set){
   if(!set) return 0;
   return set->isLast == 0;
}

Powerset* powerset_next(Powerset* set){
   if(!set) return 0;
   if(!set->fetched) return set;
   set->fetched = 0;
   next_subset(set->bset,set->size);
   set->isLast = powerset_is_last(set);
   return set;
}

Iterator* powerset_fetch_iter(Powerset* set){
   set->fetched = 1;
   iter_reset(set->iter);
   return set->iter;
}

void powerset_dropData(Powerset* set){
   if(!set) return;
   set->fetched = 1;
}

void powerset_delete(Powerset* set){
   if(!set) return;
   free(set->bset);
   free(set);
}

void powerset_reset(Powerset* set){
   if(!set) return;
   memset(set->bset,0,set->size);
}


