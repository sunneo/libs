#ifndef PERMUTATION_H_
#define PERMUTATION_H_
#include "iter.h"

typedef struct {
   void* permArray;
   int* numArrayRef;
   Iterator* iter;
   int isLast;
   int size;
   int elesize;
   int fetched;
}Permutation;

Permutation* perm_create(void* array,int elesize,int size);
void         perm_dropData(Permutation* perm);
int          perm_hasNext(const Permutation* perm);
Permutation* perm_next(Permutation* perm);
Iterator*    perm_fetch_iter(Permutation* perm);
void         perm_delete(Permutation* perm);

#endif 
