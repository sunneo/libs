#ifndef MACRO_DOUBLE_ENDED_QUEUE_src_H_
#define MACRO_DOUBLE_ENDED_QUEUE_src_H_
#include "src_macro_list.h"
#include "macro_base.h"

#define _MACRO_DEQUE(TYPE)                 \
  struct                                   \
  {                                        \
     _deque_base_##TYPE* front;            \
     _deque_base_##TYPE* back;             \
     size_t size;                          \
     size_t ressize;                       \
     char flag;                            \
  }

#define DEQueueFlag_Empty 1
#define DEQueueFlag_Full 2
#define DEQueueFlag_Restrict 4
#define DEQueueFlag_Init 8

#define _MACRO_DEQueDefine(TYPE) typedef _MACRO_LIST(TYPE,_deque_base_) _deque_base_##TYPE; typedef _MACRO_DEQUE(TYPE) def__deque_##TYPE

#define _MACRO_DEQueDeclare(TYPE) def__deque_##TYPE

#define _MACRO_DEQueIsEmpty(PTR__DEQUE) ((PTR__DEQUE)->flag&DEQueFlag_Empty)
#define _MACRO_DEQueIsFull(PTR__DEQUE) ((PTR__DEQUE)->flag&DEQueFlag_Full)
#define _MACRO_DEQueIsRestrict(PTR__DEQUE) ((PTR__DEQUE)->flag&DEQueFlag_Restrict)
#define _MACRO_DEQueIsInit(PTR__DEQUE) ((PTR__DEQUE)->flag&DEQueFlag_Init)

#define _MACRO_DEQueSetEmpty(PTR__DEQUE) ((PTR__DEQUE)->flag|=DEQueFlag_Empty))
#define _MACRO_DEQueSetFull(PTR__DEQUE) ((PTR__DEQUE)->flag|=DEQueFlag_Full))
#define _MACRO_DEQueSetRestrict(PTR__DEQUE) ((PTR__DEQUE)->flag|=DEQueFlag_Restrict))
#define _MACRO_DEQueSetInit(PTR__DEQUE) ((PTR__DEQUE)->flag|=DEQueFlag_Init))

#define _MACRO_DEQueUnEmpty(PTR__DEQUE) ((PTR__DEQUE)->flag&=~(DEQueFlag_Empty))
#define _MACRO_DEQueUnFull(PTR__DEQUE) ((PTR__DEQUE)->flag&=~(DEQueFlag_Full))
#define _MACRO_DEQueUnRestrict(PTR__DEQUE) ((PTR__DEQUE)->flag&=~(DEQueFlag_Restrict))
#define _MACRO_DEQueUnInit(PTR__DEQUE) ((PTR__DEQUE)->flag&=~(DEQueFlag_Init))



#define _MACRO_DEQueCreate(PTR__DEQUE) ((__typeof__((PTR__DEQUE)))calloc(1,sizeof(*(PTR__DEQUE)) ) )
#define _MACRO_DEQueInit(PTR__DEQUE)                      \
MACROBegin                                                \
  (PTR__DEQUE)->front = ListCreate((PTR__DEQUE)->front);  \
  (PTR__DEQUE)->back = front;                             \
  _MACRO_DEQueSetInit(PTR__DEQUE);                        \
MACROEnd

#define _MACRO_DEQuePushBack(PTR__DEQUE,__DATA)                                           \
MACROBegin                                                                                \
  if(!_MACRO_DEQueIsInit((PTR__DEQUE)) )                                                  \
    _MACRO_DEQueInit((PTR__DEQUE));                                                       \
  _MACRO_DEQueUnEmpty((PTR__DEQUE));                                                      \
  if(_MACRO_DEQueIsFull((PTR__DEQUE)) ) MACROEscape;                                      \
  ListPushBack((PTR__DEQUE)->back,__DATA);                                                \
  ++(PTR__DEQUE)->size;                                                                   \
  (PTR__DEQUE)->back = (PTR__DEQUE)->back->next;                                          \
  if(_MACRO_DEQueIsRestrict((PTR__DEQUE)) && (PTR__DEQUE)->size >= (PTR__DEQUE)->ressize) \
    _MACRO_DEQueSetFull(PTR__DEQUE);                                                      \
MACROEnd

#define _MACRO_DEQuePushFront(PTR__DEQUE,__DATA)                                          \
MACROBegin                                                                                \
  if(!_MACRO_DEQueIsInit((PTR__DEQUE)) )                                                  \
    _MACRO_DEQueInit((PTR__DEQUE));                                                       \
  _MACRO_DEQueUnEmpty((PTR__DEQUE));                                                      \
  if(_MACRO_DEQueIsFull((PTR__DEQUE)) ) MACROEscape;                                      \
  ListPushFront((PTR__DEQUE)->front,__DATA);                                              \
  ++(PTR__DEQUE)->size;                                                                   \
  (PTR__DEQUE)->front = (PTR__DEQUE)->front->prev;                                        \
  if(_MACRO_DEQueIsRestrict((PTR__DEQUE)) && (PTR__DEQUE)->size >= (PTR__DEQUE)->ressize) \
    _MACRO_DEQueSetFull(PTR__DEQUE);                                                      \
MACROEnd

#define _MACRO_DEQuePopFront(PTR__DEQUE)                                        \
MACROBegin                                                                      \
  __typeof__((PTR__DEQUE)->front) _ptr_to__front = (PTR__DEQUE)->front;         \
  if(_MACRO_DEQueIsEmpty((PTR__DEQUE)) ) MACROEscape;                           \
  _MACRO_DEQueUnFull((PTR__DEQUE));                                             \
  if((PTR__DEQUE)->front->next) (PTR__DEQUE)->front = (PTR__DEQUE)->front->next;\
  ListDeleteNode(_ptr_to__front);                                               \
  --(PTR__DEQUE)->size;                                                         \
  if(!(PTR__DEQUE)->size) _MACRO_DEQueSetEmpty((PTR__DEQUE));                   \
MACROEnd

#define _MACRO_DEQuePopBack(PTR__DEQUE)                                         \
MACROBegin                                                                      \
  __typeof__((PTR__DEQUE)->back) _ptr_to__back = (PTR__DEQUE)->back;            \
  if(_MACRO_DEQueIsEmpty((PTR__DEQUE)) ) MACROEscape;                           \
  _MACRO_DEQueUnFull((PTR__DEQUE));                                             \
  if((PTR__DEQUE)->back->prev) (PTR__DEQUE)->back = (PTR__DEQUE)->back->prev;   \
  ListDeleteNode(_ptr_to__back);                                                \
  --(PTR__DEQUE)->size;                                                         \
  if(!(PTR__DEQUE)->size) _MACRO_DEQueSetEmpty((PTR__DEQUE));                   \
MACROEnd

#define _MACRO_DEQueGetFront(PTR__DEQUE)     ((PTR__DEQUE)->front->data)
#define _MACRO_DEQueGetBack(PTR__DEQUE)     ((PTR__DEQUE)->back->data)
#define _MACRO_DEQueGetSize(PTR__DEQUE)     ((PTR__DEQUE)->size)







#endif
