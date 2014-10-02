#ifndef HEAP_SOURCE_H_
#define HEAP_SOURCE_H_

#define MACRO_HEAP__SWAP(PTR__HEAP,A,B) \
do{                                     \
   (PTR__HEAP)->tmpForSwap = (A);       \
   (A) = (B);                           \
   B = (PTR__HEAP)->tmpForSwap;         \
}while(0)

#define _HEAP(TYPE)                         \
   struct                                   \
   {                                        \
      TYPE* data;                           \
      TYPE tmpForSwap;                      \
      int typesize;                         \
      int size,back,front;                  \
      int (*compare)(const TYPE,const TYPE);\
      void (*pop_data)(TYPE);               \
   }

#define macro_heap_def(TYPE) typedef _HEAP(TYPE) heap_##TYPE
#define macro_heap_decl(TYPE) heap_##TYPE

#define macro_heap_create(GETTER,TYPE,Comparator,ELEMENT_DESTRUCTOR) \
do{                                                                  \
   (GETTER) = (__typeof__(GETTER))malloc(sizeof(heap_decl(TYPE)));   \
   (GETTER)->data = NULL;                                            \
   (GETTER)->typesize = sizeof(TYPE);                                \
   (GETTER)->size = 0;                                               \
   (GETTER)->front = 0;                                              \
   (GETTER)->back = 0;                                               \
   (GETTER)->compare = (Comparator);                                 \
   (GETTER)->pop_data = ELEMENT_DESTRUCTOR;                          \
}while(0)

#define macro_heap_size_allocator(PTR__HEAP)                                                                     \
do{                                                                                                              \
   if((PTR__HEAP)->size == 0)                                                                                    \
   {                                                                                                             \
      (PTR__HEAP)->size = 1;                                                                                     \
      (PTR__HEAP)->data = (__typeof__((PTR__HEAP)->data)) malloc((PTR__HEAP)->typesize);                                     \
      break;                                                                                                     \
   }                                                                                                             \
   else                                                                                                     \
   {                                                                                                             \
      if((PTR__HEAP)->back >= (PTR__HEAP)->size)                                                                 \
        (PTR__HEAP)->size = 1 + ((PTR__HEAP)->size<<1);                                                            \
      (PTR__HEAP)->data = (__typeof__((PTR__HEAP)->data)) realloc((PTR__HEAP)->data, (PTR__HEAP)->typesize * (PTR__HEAP)->size );\
      break;                                                                                                     \
   }                                                                                                             \
}while(0)

#define macro_heap_getparent(IDX) (((IDX) & 1) ? (((IDX)-1) >>1) : ((IDX)>>1) - 1)
#define index_of_node(PTR__HEAP,INDEX)    ((PTR__HEAP)->data[ (INDEX) ]  )

#define macro_heap_push(PTR__HEAP,DATA)                                                                  \
do{                                                                                                      \
   int _parent,_idxback;                                                                                 \
   macro_heap_size_allocator((PTR__HEAP));                                                               \
   index_of_node((PTR__HEAP),(PTR__HEAP)->back) = (DATA);                                                \
   if((PTR__HEAP)->compare)                                                                              \
   {                                                                                                     \
      for(_idxback = (PTR__HEAP)->back; _idxback; _idxback = _parent)                                    \
      {                                                                                                  \
         _parent = macro_heap_getparent(_idxback);                                                        \
         if(  (PTR__HEAP)->compare(   index_of_node((PTR__HEAP),_idxback)  , index_of_node((PTR__HEAP),_parent) ) )                    \
            MACRO_HEAP__SWAP(  (PTR__HEAP) ,  index_of_node((PTR__HEAP),_parent),index_of_node((PTR__HEAP),_idxback)  )  ;                              \
         else break;                                                                                     \
      }                                                                                                  \
   }                                                                                                     \
   ++(PTR__HEAP)->back;                                                                                  \
}while(0)

#define macro_heap_size(PTR__HEAP) ((PTR__HEAP)->back - (PTR__HEAP)->front)
#define macro_heap_top(PTR__HEAP) (heap_size(PTR__HEAP)?(PTR__HEAP)->data[ 0 ]:0)
#define macro_heap_empty(PTR__HEAP) ((PTR__HEAP)->back == (PTR__HEAP)->front)

#define macro_heap_pop(PTR__HEAP)                                                                                                     \
do{                                                                                                                                   \
   int current,max_idx;                                                                                                               \
   unsigned int max_arr[2];                                                                                                                    \
   if(macro_heap_empty(PTR__HEAP)) break;                                                                                             \
   if(!(PTR__HEAP)->compare)                                                                                                          \
   {                                                                                                                                  \
      ++(PTR__HEAP)->front;                                                                                                           \
      if(macro_heap_empty((PTR__HEAP)) )                                                                                              \
        (PTR__HEAP)->front = (PTR__HEAP)->back = 0;                                                                                   \
      break;                                                                                                                          \
   }                                                                                                                                  \
   current = --(PTR__HEAP)->back;                                                                                                     \
   MACRO_HEAP__SWAP((PTR__HEAP),index_of_node((PTR__HEAP),0),index_of_node((PTR__HEAP),current));                                     \
   if((PTR__HEAP)->pop_data) (PTR__HEAP)->pop_data(index_of_node((PTR__HEAP),current));                                               \
   current = 0;                                                                                                                       \
   for(;;)                                                                                                                            \
   {                                                                                                                                  \
      max_arr[ 0 ] = (current<<1)+1;                                                                                                  \
      if(max_arr[ 0 ] >= (PTR__HEAP)->back) break;                                                                                    \
      max_arr[ 1 ] = max_arr[ 0 ]+1;                                                                                                  \
      if(max_arr[ 1 ] >= (PTR__HEAP)->back) max_idx = max_arr[ 0 ];                                                                   \
      else max_idx = max_arr[ (PTR__HEAP)->compare(index_of_node((PTR__HEAP),max_arr[ 1 ]),index_of_node((PTR__HEAP),max_arr[ 0 ])) ];\
      if((PTR__HEAP)->compare( index_of_node((PTR__HEAP),max_idx), index_of_node((PTR__HEAP),current) ) )                             \
      {                                                                                                                               \
         MACRO_HEAP__SWAP((PTR__HEAP),index_of_node((PTR__HEAP),current),index_of_node((PTR__HEAP),max_idx) );                        \
         current = max_idx;                                                                                                           \
      }                                                                                                                               \
      else break;                                                                                                                     \
   }                                                                                                                                  \
}while(0)

#define macro_heap_clear(PTR__HEAP)                                   \
do{                                                                   \
   if(macro_heap_empty(PTR__HEAP) break;                              \
   if((PTR__HEAP)->pop_data)                                          \
      while(--(PTR__HEAP)->back)                                      \
        (PTR__HEAP)->pop_data((PTR__HEAP)->data[ (PTR__HEAP)->back ]);\
   else (PTR__HEAP)->back = 0;                                        \
   free((PTR__HEAP)->data);                                           \
   (PTR__HEAP)->size = 0;                                             \
}while(0)

#define macro_heap_delete(PTR__HEAP)\
do{                                 \
   macro_heap_clear(PTR__HEAP);     \
   free(PTR__HEAP);                 \
   PTR__HEAP = 0;                   \
}while(0)

#endif
