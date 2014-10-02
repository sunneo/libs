#ifndef HEAP_WITH_NO_REALLOC_H_
#define HEAP_WITH_NO_REALLOC_H_
#ifndef __stdcall
#  define __stdcall __attribute__((__stdcall__))
#endif
#ifndef __CallConv 
#   define __CallConv __stdcall
#endif
/**
 * heap structure without reallocation.
 * IceCold (sunneo)
 * email   : sunneo529@hotmail.com
 * homepage: http://sunneo.myweb.hinet.net
 */
 
/**
 * HeapNode is a type of linked list,which include
 * the layers of array, which size increse by power of 2.
 */
typedef struct HeapNrNode{
   void** data;
   struct HeapNrNode* prev,*next;
   int size,back;
}HeapNrNode;

/**
 * the major heap entry point.
 */
typedef struct HeapNr{
   int size;
   HeapNrNode* root,*backptr;
   int (*compare)(const void*,const void*);
}HeapNr;
#ifdef __cplusplus
extern "C"{
#endif
HeapNr* heapnr_create( int(*compare)(const void*,const void*) );
void heapnr_push(HeapNr* heap, const void* data);
void heapnr_pop(HeapNr* heap);
int heapnr_size(const HeapNr* heap);
int heapnr_empty(const HeapNr* heap);
void* heapnr_top(const HeapNr* heap);
void* heapnr_pop_return(HeapNr* heap);
void heapnr_delete(HeapNr* heap);
void heapnr_traverse(HeapNr* heap,void(*fn)(void*));
void heapnr_clear(HeapNr* heap);
void heapnr_reorder(HeapNr* heap);
#ifdef __cplusplus
  }
#endif
#endif
