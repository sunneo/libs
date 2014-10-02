#ifndef __MACRO_STACK_src_H_
#define __MACRO_STACK_src_H_
#include "src_macro_list.h"
#ifndef _MACRO_LIST_H_
#  error "I need IceCold-sunneo:: macro_list.h "
#endif

#include <stdlib.h>
#ifndef MACROBegin
#  define MACROBegin do{
#endif
#ifndef MACROEnd
#  define MACROEnd }while(0)
#endif
#ifndef MACROEscape
#  define MACROEscape break
#endif

#define StackFlag_Restrict 0x02
#define StackFlag_Full     0x04
#define StackFlag_Init     0x08

#define _MACRO_STACK(TYPE)    \
struct                        \
{                             \
   _stack_base_##TYPE* top;   \
   _stack_base_##TYPE* bottom;\
   size_t size;               \
   size_t ResSize;            \
   char flag;                 \
}

#define _MACRO_StackDefine(TYPE) typedef _MACRO_LIST(TYPE,stackbase) _stack_base_##TYPE; typedef _MACRO_STACK(TYPE) def_Stack_##TYPE
#define _MACRO_StackDeclare(TYPE) def_Stack_##TYPE

#define _MACRO_StackCreate(PTR__Stack) (__typeof__(PTR__Stack))calloc(1,sizeof(*PTR__Stack))

#define _MACRO_StackIsRestrict(PTR__Stack) ((PTR__Stack)->flag & StackFlag_Restrict)
#define _MACRO_StackIsFull(PTR__Stack) ((PTR__Stack)->flag & StackFlag_Full)
#define _MACRO_StackIsEmpty(PTR__Stack) (!(PTR__Stack)->size)
#define _MACRO_StackIsInit(PTR__Stack) ((PTR__Stack)->flag & StackFlag_Init)

#define _MACRO_StackInit(PTR__Stack)                       \
MACROBegin                                                 \
   (PTR__Stack)->flag |= StackFlag_Init;                   \
   (PTR__Stack)->bottom = ListCreate((PTR__Stack)->bottom);\
   (PTR__Stack)->top = (PTR__Stack)->bottom;               \
MACROEnd


#define _MACRO_StackPush(PTR__Stack,DATA)                                                \
MACROBegin                                                                               \
  if(!_MACRO_StackIsInit((PTR__Stack)))                                                  \
    _MACRO_StackInit((PTR__Stack));                                                      \
  if(_MACRO_StackIsFull((PTR__Stack)) ) MACROEscape;                                     \
  if( _MACRO_StackIsRestrict(PTR__Stack) && (PTR__Stack)->size >= (PTR__Stack)->ResSize) \
  {                                                                                      \
     _MACRO_StackSetFull((PTR__Stack));                                                  \
     MACROEscape;                                                                        \
  }                                                                                      \
  ListPushBack((PTR__Stack)->top,DATA);                                                  \
  (PTR__Stack)->top = (PTR__Stack)->top->next;                                           \
  ++(PTR__Stack)->size;                                                                  \
MACROEnd                                                                                                                                  


#define _MACRO_StackPop(PTR__Stack)                                        \
MACROBegin                                                                 \
  __typeof__(PTR__Stack->top) PTR__CurrentNode = (PTR__Stack)->top;        \
  _MACRO_StackUnFull((PTR__Stack));                                        \
  if(_MACRO_StackIsEmpty((PTR__Stack))) MACROEscape;                       \
  (PTR__Stack)->top = (PTR__Stack)->top->prev;                             \
  ListDeleteNode(PTR__CurrentNode);                                        \
  --(PTR__Stack)->size;                                                    \
MACROEnd

#define _MACRO_StackSetRestrictSize(PTR__Stack,__SIZE)\
MACROBegin                                         \
  (PTR__Stack)->ResSize = __SIZE;                  \
  (PTR__Stack)->flag |= StackFlag_Restrict;        \
MACROEnd

#define _MACRO_StackSetRestrict(PTR__Stack) _MACRO_StackSetRestrictSize(PTR__Stack,PTR__Stack->size)
#define _MACRO_StackUnRestrict(PTR__Stack) (PTR__Stack)->flag &= (~StackFlag_Restrict)

#define _MACRO_StackCurrent(PTR__Stack) (!_MACRO_StackIsEmpty((PTR__Stack))?(PTR__Stack)->top->prev->data:(PTR__Stack)->top->data)
#define _MACRO_StackTop(PTR__Stack) _MACRO_StackCurrent((PTR__Stack))
#define _MACRO_StackGetCurrent(PTR__Stack)    _MACRO_StackCurrent((PTR__Stack))
#define _MACRO_StackGetSize(PTR__Stack)  ((PTR__Stack)->size)


#define _MACRO_StackSetFull(PTR__Stack) ((PTR__Stack)->flag |= StackFlag_Full )
#define _MACRO_StackUnFull(PTR__Stack) ((PTR__Stack)->flag &= (~StackFlag_Full) )
#define _MACRO_StackSetEmpty(PTR__Stack) ((PTR__Stack)->flag |= StackFlag_Empty )


#define _MACRO_StackPopTo(PTR__Stack,PTR__Container)                                                     \
MACROBegin                                                                                               \
  __typeof__(_MACRO_StackCurrent((PTR__Stack))) __local__stack__tmp = _MACRO_StackCurrent((PTR__Stack)); \
  if(StackIsEmpty((PTR__Stack)) || !_MACRO_StackIsInit((PTR__Stack))) MACROEscape;                       \
  memcpy((PTR__Container),&__local__stack__tmp,sizeof(_MACRO_StackCurrent((PTR__Stack))));               \
  _MACRO_StackPop((PTR__Stack));                                                                         \
MACROEnd

#define _MACRO_StackDestroy(PTR__Stack)                             \
MACROBegin                                                          \
  if( !PTR__Stack || !_MACRO_StackIsInit((PTR__Stack))) MACROEscape;\
  ListRelease((PTR__Stack)->bottom);                                \
  free((PTR__Stack));                                               \
MACROEnd


#endif
