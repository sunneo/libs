#ifndef HEAP_H_
#define HEAP_H_
#include "src/src_macro_heap.h"

#define heap_def(TYPE)            macro_heap_def(TYPE) 
#define heap_decl(TYPE)           macro_heap_decl(TYPE) 
#define heap_create(GETTER,TYPE,Comparator,ELEMENT_DESTRUCTOR)         macro_heap_create(GETTER,TYPE,Comparator,ELEMENT_DESTRUCTOR)
#define heap_size(PTR__HEAP)      macro_heap_size((PTR__HEAP))
#define heap_top(PTR__HEAP)       macro_heap_top((PTR__HEAP)) 
#define heap_empty(PTR__HEAP)     macro_heap_empty((PTR__HEAP))
#define heap_push(PTR__HEAP,DATA) macro_heap_push((PTR__HEAP),(DATA))
#define heap_pop(PTR__HEAP)            macro_heap_pop((PTR__HEAP))
#define heap_clear(PTR__HEAP)     macro_heap_clear((PTR__HEAP))
#define heap_delete(PTR__HEAP)    macro_heap_delete((PTR__HEAP))

#endif
