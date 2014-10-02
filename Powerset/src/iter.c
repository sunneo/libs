#include "iter.h"
#include <stdlib.h>
/*--------------------------------------------------------------------------*/

static int iterinit_hasNext(const Iterator* iter){ return 0; }
static void* iterinit_next(Iterator* iter){ return 0; }
static void iterinit_reset(Iterator* iter){}
static void iterinit_remove(Iterator* iter){}

static IteratorVTable iteratorVirtualTable = {
   iterinit_hasNext,
   iterinit_next,
   iterinit_reset,
   iterinit_remove
};

Iterator* iter_create(){
   Iterator* iter = (Iterator*)malloc(sizeof(Iterator));
   iter->vtbl = &iteratorVirtualTable;
   return iter;
}

int iter_hasNext(const Iterator* iter){
   if(!iter) return 0;
   return ((IteratorVTable*)iter->vtbl)->hasNext(iter);
}

void* iter_next(Iterator* iter){
   if(!iter) return 0;
   return ((IteratorVTable*)iter->vtbl)->next(iter);
}

void iter_remove(Iterator* iter){
   ((IteratorVTable*)iter->vtbl)->remove(iter);
}

void iter_reset(Iterator* iter){
   ((IteratorVTable*)iter->vtbl)->reset(iter);
}

void iter_delete(Iterator* iter){
   free(iter);
}

/*--------------------------------------------------------------------------*/

