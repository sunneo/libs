#ifndef ITERATOR_H_
#define ITERATOR_H_
typedef struct {
   void* vtbl;
}Iterator;

Iterator* iter_create();
int       iter_hasNext(const Iterator* iter);
void*     iter_next(Iterator* iter);
void      iter_reset(Iterator* iter);
void      iter_remove(Iterator* iter);
void      iter_delete(Iterator* iter);

typedef struct {
   int   (*hasNext)(const Iterator* iter);
   void* (*next)(Iterator*);
   void  (*reset)(Iterator*);
   void  (*remove)(Iterator*);
}IteratorVTable;

#endif
