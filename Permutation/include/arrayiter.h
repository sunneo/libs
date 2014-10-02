#ifndef ARRAY_ITERATOR_H_
#define ARRAY_ITERATOR_H_
#include "iter.h"

typedef struct{
   Iterator super;
   void* array;
   int idx;
   int elesize;
   int size;
}ArrayIterator; 

ArrayIterator* arrayiter_create(void* array,int elesize,int size);
void           arrayiter_delete(ArrayIterator* arrayIter);
#endif
