#ifndef POWERSET_H_
#define POWERSET_H_
#include "iter.h"

typedef struct {
   void* array;
   int size;
   char* bset;
   Iterator* iter;
   int fetched;
   int isLast;
   int elesize;
}Powerset;

Powerset* powerset_create(void* array,int elesize,int size);
int       powerset_hasNext(const Powerset* set);
Powerset* powerset_next(Powerset* set);
void      powerset_reset(Powerset* set);
Iterator* powerset_fetch_iter(Powerset* set);
void      powerset_delete(Powerset* set);

#endif
