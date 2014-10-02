#include "arrayiter.h"
#include <stdlib.h>

static int arrayiter_hasNext(const ArrayIterator* a){
   return a->idx < a->size;
}

static void* arrayiter_next(ArrayIterator* a){
   void* ret = (((char*)a->array)+a->idx*a->elesize);
   ++a->idx;
   return ret;
}

static void arrayiter_reset(ArrayIterator* a){
   a->idx = 0;
}

static void arrayiter_remove(ArrayIterator* a){
}


static IteratorVTable ArrayIteratorVirtualTable = {
   (int(*)(const Iterator*))arrayiter_hasNext,
   (void*(*)(Iterator*))arrayiter_next,
   (void (*)(Iterator*))arrayiter_reset,
   (void (*)(Iterator*))arrayiter_remove
};

ArrayIterator* arrayiter_create(void* array,int elesize,int size){
   ArrayIterator* arrayiter = (ArrayIterator*)malloc(sizeof(ArrayIterator));
   arrayiter->super.vtbl = &ArrayIteratorVirtualTable;
   arrayiter->array = array;
   arrayiter->elesize = elesize;
   arrayiter->size = size;
   arrayiter->idx = 0;
   return arrayiter; 
}

void arrayiter_delete(ArrayIterator* arrayiter){
   free(arrayiter);
}


