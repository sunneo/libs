#include "heap_nr.h"
#ifndef __fastcall
#  define __fastcall  __attribute__((__fastcall__))
#endif
#include <stdlib.h>

HeapNrNode* __fastcall 
heapnode_create(int size,HeapNrNode* prev,HeapNrNode* next){
   HeapNrNode* n = (HeapNrNode*)malloc(sizeof(HeapNrNode));
   n->prev = prev;
   n->next = next;
   n->size = size;
   n->back = 0;
   n->data = (void**)malloc(sizeof(void*) * size);
   return n;
}


static void __fastcall 
heapnode_delete(HeapNrNode* n){
   if(n->prev) n->prev->next = n->next;
   if(n->next) n->next->prev = n->prev;
   free(n->data);
   free(n);
}

HeapNr* 
heapnr_create( int(*compare)(const void*,const void*) ){
   HeapNr* h = (HeapNr*)malloc(sizeof(HeapNr));
   h->size = 0;
   h->compare = compare;
   h->backptr = h->root = heapnode_create(1,NULL,NULL);
   return h;
}

typedef void* PVOID;
typedef void** PPVOID;
typedef const void* CLPVOID;

static void __fastcall 
swap(PPVOID a,PPVOID b){
   PVOID t = *a;
   *a = *b;
   *b = t;
}



/**
 * compare and swap used to reduce the address of operation,
 * if we dont use this funciton, it could be
 * if( compare( addr(a), addr(b) )
 *   swap(addr(a), addr(b))
 * that's total 4 addr operations. ( ignore the swap )
 * 
 *  otherwise we use the expression(function)
 *  it should be following
 *  t1 = addr(a), t2 = addr(b)
 *  if( compare( t1, t2 ) )
 *    swap( t1, t2 )
 *  it's only 2 addr operations.
 */
static int __fastcall 
compare_and_swap(PPVOID a,PPVOID b, int(*compare)(CLPVOID,CLPVOID) ) {
   PVOID t;
   if(!compare(*a,*b) ) return 0;
   t = *a;
   *a = *b;
   *b = t;
   return 1;
}



void 
heapnr_push(HeapNr* h, const void* data){
   HeapNrNode* prev = NULL;
   HeapNrNode* n = h->backptr;
   int back = n->back++;
   h->size+=1;
   n->data[ back ] = (void*)data;
   if( n->back >= n->size ){
       if(!n->next) n->next = heapnode_create( n->size<<1 , n , NULL );
       h->backptr = n->next;
   }
   while( (prev = n->prev) ){
      if( !compare_and_swap( n->data+back ,prev->data+(back>>1), h->compare ) ) 
         return;
      n = prev;
      back >>=1;
   }
}

/** 
 * choose_ptr_if_null
 * to make expression a->b = a->b?a->b:c short,
 * the a->b ? a->b have twice reference operation,
 * just like
 * 
 * p = a;
 * p+=b;
 * if(p !=0)
 *   ret = a;
 *   ret += b;
 * else
 *   ret = c
 * ....... 6 moves
 * 
 * if we add cache , it should be 
 *
 * cache = a;
 * cache += b;
 * if(cache != 0)
 *   ret = cache
 * else
 *   ret = c
 * ......... 5 moves 
 *
 */
static const void* __fastcall 
choose_ptr_if_null(const void* a,const void* ifnull){
   return a?a:ifnull;
}

void 
heapnr_pop(HeapNr* heap){
   HeapNrNode* next = heap->root;
   HeapNrNode* current;
   int cur,left,right,maxidx;
   if(! heap->size ) return;
   --heap->size;
   if(!heap->backptr->back) 
     heap->backptr = (HeapNrNode*)choose_ptr_if_null(heap->backptr->prev,heap->root);
   current = heap->backptr;
   *next->data = current->data[ --current->back ] ;
   current = next;
   left = cur = 0;
   right = 1;
   while( ( next = current->next ) && (left < next->back) ){
      maxidx = (right>=next->back? left:( heap->compare(next->data[ left ],next->data[ right ]) ? left: right)  );
      if(! compare_and_swap( next->data +  maxidx ,current->data + cur ,heap->compare ) ) 
         return;
      cur = maxidx;
      left = cur<<1;
      right = left + 1;
      current = next;
   }
}

int 
heapnr_size(const HeapNr* heap){
   return heap->size;
}

int 
heapnr_empty(const HeapNr* heap){
   return heap->size == 0;
}

void* 
heapnr_top(const HeapNr* heap){
   return heap->size == 0 ? NULL: *heap->root->data;
}

void* 
heapnr_pop_return(HeapNr* heap){
   void* ret = heapnr_top(heap);
   heapnr_pop(heap);
   return ret;
}

void 
heapnr_delete(HeapNr* heap){
   HeapNrNode* prev;
   while(heap->backptr){
      prev = heap->backptr->prev;
      heapnode_delete(heap->backptr);
      heap->backptr = prev;
   }
   free(heap);
}

void 
heapnr_traverse(HeapNr* heap,void(*fn)(void*)){
    HeapNrNode* r = heap->root;
    size_t i;
    while( r!=0 ){
       for(i=0; i<r->back; ++i)
         fn(r->data[ i ]);
       r = r->next;
    }
}

void 
heapnr_reorder(HeapNr* heap){
   size_t index,i;
   HeapNrNode* level = heap->root->next;
   HeapNrNode* prev,*n;
   if(heap->size < 2) return;
   while(level){
      for(i = 0; i<level->back; ++i){
        index = i;
        n = level;
        while( (prev = n->prev) !=0 ){
          if( !compare_and_swap(n->data +  index ,prev->data +  (index>>1) ,heap->compare) ) 
              break ;
          n = prev;
          index>>=1;
        }
      }
      level = level->next;
   }
}

void 
heapnr_clear(HeapNr* heap){
   HeapNrNode* p;
   while(heap->backptr!=heap->root){
     p = heap->backptr->prev;
     heapnode_delete(heap->backptr);
     heap->backptr = p;
   }
}

