#ifndef MACRO_Array_src_h_
#define MACRO_Array_src_h_
#include <stdlib.h>

#ifndef MACROBegin
#  define MACROBegin do{
#endif
#ifndef MACROEnd
#  define MACROEnd }while(0)
#endif 
#ifndef MACROEscape
#  define  MACROEscape break
#endif
#ifndef _min
#  define _min(PTR__Result,A,B)                          \
   MACROBegin                                            \
      __typeof__(A) _tmp_A = (A);                        \
      __typeof__(B) _tmp_B = (B);                        \
      *(PTR__Result) = _tmp_A > _tmp_B ? _tmp_B :_tmp_A ;\
   MACROEnd
#endif

#ifndef _max
#  define _max(PTR__Result,A,B)                          \
   MACROBegin                                            \
      __typeof__(A) _tmp_A = (A);                        \
      __typeof__(B) _tmp_B = (B);                        \
      *(PTR__Result) = _tmp_A > _tmp_B ? _tmp_A :_tmp_B ;\
   MACROEnd
#endif

#define _MACRO_Array(TYPE) \
struct                     \
{                          \
   TYPE* data;             \
   size_t back;            \
   size_t size;            \
}

#define _MACRO_ArrayDefine(TYPE) typedef _MACRO_Array(TYPE) def_Array_##TYPE
#define _MACRO_ArrayDeclare(TYPE) def_Array_##TYPE

#define _MACRO_ArrayCreate(PTR__Array) (__typeof__((PTR__Array)))calloc(1,sizeof(*(PTR__Array)))


#define _MACRO_ArrayResize(PTR__Array,__NEWSIZE)                                                                                                         \
  ((PTR__Array)->data                                                                                                                                    \
  ? ((PTR__Array)->data = (__typeof__((PTR__Array)->data))realloc((PTR__Array)->data,( (PTR__Array)->size = (__NEWSIZE))*sizeof(*(PTR__Array)->data) ))  \
      ?(PTR__Array)                                                                                                                                      \
      :(PTR__Array)                                                                                                                                      \
  : ( (PTR__Array)->data = (__typeof__((PTR__Array)->data)) malloc (  ( (PTR__Array)->size = (__NEWSIZE?__NEWSIZE:1024)  ) * sizeof( * (PTR__Array)->data )  ) ) \
      ?(PTR__Array)                                                                                                                                      \
      :(PTR__Array)                                                                                                                                      \
  )

#define _MACRO_ArrayIndexOf__Address(PTR__Array,__INDEX)      \
  ( (PTR__Array)->size > (__INDEX)                            \
    ? &(PTR__Array)->data[ (__INDEX) ]                        \
    : (_MACRO_ArrayResize((PTR__Array),(__INDEX)+1)->data)    \
      ? &(PTR__Array)->data[ (__INDEX) ]                      \
      : &(PTR__Array)->data[ (__INDEX) ])

#define _MACRO_ArrayIndexOf(PTR__Array,__INDEX)  (*_MACRO_ArrayIndexOf__Address(PTR__Array,__INDEX))

#define _MACRO_ArrayAssign(PTR__Array,__INDEX,DATA)                                                         \
MACROBegin                                                                                                  \
  __typeof__((DATA)) __tmp_DATA = (DATA);                                                                   \
  while((PTR__Array)->size < __INDEX) PTR__Array = _MACRO_ArrayResize((PTR__Array),(PTR__Array)->size<<1); \
  memcpy( &((PTR__Array)->data[ __INDEX ] ),&(__tmp_DATA), sizeof(__tmp_DATA) );                            \
MACROEnd


#define _MACRO_ArrayDestroy(PTR__Array)             \
MACROBegin                                          \
  __typeof__((PTR__Array)) __tmp_del = (PTR__Array);\
  if(__tmp_del->data) free(__tmp_del->data);        \
  free(__tmp_del);                                  \
MACROEnd

#define _MACRO_ArrayForEach(PTR__Array,Function__To_Element)               \
MACROBegin                                                                 \
   __typeof__((PTR__Array)->size)  __tmp_index = 0;                        \
   for(__tmp_index=0; __tmp_index<(PTR__Array)->back; ++__tmp_index)       \
     Function__To_Element(_MACRO_ArrayIndexOf((PTR__Array),__tmp_index));  \
MACROEnd

#define _MACRO_ArrayPushBack(PTR__Array,__DATA)                                                  \
MACROBegin                                                                                       \
  __typeof__(__DATA) _local_tmp_data = (__DATA);                                                 \
  if((PTR__Array)->back +1 >= (PTR__Array)->size )                                               \
    (PTR__Array) = _MACRO_ArrayResize((PTR__Array),((PTR__Array)->size<<1));                           \
  memcpy(&(PTR__Array)->data[ (PTR__Array)->back++ ],&(_local_tmp_data),sizeof(_local_tmp_data));\
MACROEnd

#define _MACRO_ArrayCopy(PTR__Array_dst,PTR__Array_src)                                         \
MACROBegin                                                                                      \
  size_t min_of_size = 0;                                                                       \
  _min(&min_of_size,(PTR__Array_dst)->back,(PTR__Array_src)->back);                             \
  memcpy((PTR__Array_dst)->data,(PTR__Array_src)->data,sizeof(*(PTR__Array_src)->data)*min_of_size);\
MACROEnd

#define _MACRO_ArrayErase(PTR__Array)                                          \
MACROBegin                                                                     \
  memset((PTR__Array)->data,0,sizeof(*(PTR__Array)->data)*(PTR__Array)->size); \
MACROEnd

#endif
