#ifndef MACRO_QUEUE_src_H
#define MACRO_QUEUE_src_H
#include "src_macro_list.h"
#ifndef _MACRO_LIST_H_
#  error "I need IceCold-sunneo:: macro_list.h "
#endif

#ifndef MACROBegin
#  define MACROBegin do{
#endif

#ifndef MACROEnd
#  define MACROEnd }while(0)
#endif

#define QueueFlag_Empty 1
#define QueueFlag_Full 2
#define QueueFlag_Restrict 4
#define QueueFlag_Init 8

#define _MACRO_QueueIsEmpty(PTR__Queue)       (!(PTR__Queue)->size)
#define _MACRO_QueueIsFull(PTR__Queue)        ((PTR__Queue)->flag & QueueFlag_Full)
#define _MACRO_QueueIsRestrict(PTR__Queue)    ((PTR__Queue)->flag & QueueFlag_Restrict)
#define _MACRO_QueueIsInitialized(PTR__Queue) ((PTR__Queue)->flag & QueueFlag_Init) 

#define _MACRO_QueueSetRestrictSize(PTR__Queue,__RESSIZE)              \
MACROBegin                                                             \
  __typeof__(__RESSIZE) _de_RESSIZE = __RESSIZE;                       \
  ((PTR__Queue)->flag |= QueueFlag_Restrict);                          \
  if(__RESSIZE < (PTR__Queue)->size ) _de_RESSIZE = (PTR__Queue)->size;\
  (PTR__Queue)->res_size = _de_RESSIZE;                                \
MACROEnd

#define _MACRO_QueueSetFull(PTR__Queue) ((PTR__Queue)->flag |= QueueFlag_Full)
#define _MACRO_QueueSetEmpty(PTR__Queue) ((PTR__Queue)->flag |= QueueFlag_Empty)
#define _MACRO_QueueSetInitialized(PTR__Queue) ((PTR__Queue)->flag |= QueueFlag_Init)
#define _MACRO_QueueSetRestrict(PTR__Queue) _MACRO_QueueSetRestrictSize((PTR__Queue),(PTR__Queue)->size)

#define _MACRO_QueueGetSize(PTR__Queue) ((PTR__Queue)->size)

#define _MACRO_QueueUnRestrict(PTR__Queue)    \
MACROBegin                                    \
  (PTR__Queue)->flag &= (~QueueFlag_Restrict);\
MACROEnd

#define _MACRO_QueueUnFull(PTR__Queue)    \
MACROBegin                                \
  (PTR__Queue)->flag &= (~QueueFlag_Full);\
MACROEnd

#define _MACRO_QueueUnEmpty(PTR__Queue)    \
MACROBegin                                 \
  (PTR__Queue)->flag &= (~QueueFlag_Empty);\
MACROEnd

#define _MACRO_Queue(TYPE)                \
  struct                                  \
  {                                       \
     _queue_base_##TYPE* head;            \
     _queue_base_##TYPE* tail;            \
     size_t size;                         \
     size_t res_size;                     \
     char flag;                           \
  }

#define _MACRO_QueueDefine(TYPE) typedef _MACRO_LIST(TYPE,queuebase) _queue_base_##TYPE; typedef _MACRO_Queue(TYPE) def_Queue_##TYPE 
#define _MACRO_QueueDeclare(TYPE) def_Queue_##TYPE

#define _MACRO_QueueCreate(PTR__Queue)  (__typeof__(PTR__Queue))calloc(1,sizeof(*(PTR__Queue)) )

#define _MACRO_QueueInit(PTR__Queue)                  \
MACROBegin                                            \
  (PTR__Queue)->head = ListCreate((PTR__Queue)->head);\
  (PTR__Queue)->tail = (PTR__Queue)->head;            \
   _MACRO_QueueSetEmpty(PTR__Queue);                  \
   _MACRO_QueueSetInitialized(PTR__Queue);            \
MACROEnd

#define _MACRO_QueueDestroy(PTR__Queue)                                        \
MACROBegin                                                                     \
  if(!_MACRO_QueueIsEmpty(PTR__Queue) && _MACRO_QueueIsInitialized(PTR__Queue))\
    ListRelease(PTR__Queue->head);                                             \
  free((PTR__Queue));                                                          \
MACROEnd

#define _MACRO_QueuePushback(PTR__Queue,__DATA)                                              \
MACROBegin                                                                                   \
  __typeof__((__DATA)) __tmp_data = (__DATA);                                                \
  if(!_MACRO_QueueIsInitialized(PTR__Queue))                                                 \
    _MACRO_QueueInit((PTR__Queue));                                                          \
  if(_MACRO_QueueIsFull(PTR__Queue)) break;                                                  \
  if(_MACRO_QueueIsRestrict(PTR__Queue) && (PTR__Queue)->size+1 > (PTR__Queue)->res_size)    \
     _MACRO_QueueSetFull(PTR__Queue);                                                        \
  ListPushBack((PTR__Queue)->tail,__tmp_data);                                               \
  (PTR__Queue)->tail = (PTR__Queue)->tail->next;                                             \
  ++(PTR__Queue)->size;                                                                      \
MACROEnd

#define _MACRO_QueuePop(PTR__Queue)                                        \
MACROBegin                                                                 \
  __typeof__((PTR__Queue)->head) __tmp_pnode_head = (PTR__Queue)->head;    \
  if(!_MACRO_QueueIsInitialized( (PTR__Queue) ) ) break;                   \
  _MACRO_QueueUnFull(PTR__Queue);                                          \
  if(_MACRO_QueueIsEmpty((PTR__Queue))) break;                             \
  if((PTR__Queue)->head->next) (PTR__Queue)->head = (PTR__Queue)->head->next;\
  ListDeleteNode(__tmp_pnode_head);                                        \
  --(PTR__Queue)->size;                                                    \
MACROEnd

#define _MACRO_QueuePopTo(PTR__Queue,PTR__CONTAINER)                                  \
MACROBegin                                                                            \
  if(_MACRO_QueueIsEmpty(PTR__Queue) || !_MACRO_QueueIsInitialized(PTR__Queue)) break;\
  memcpy((PTR__CONTAINER),&(PTR__Queue)->head->data,sizeof(__CONTAINER));             \
  _MACRO_QueuePop(PTR__Queue);                                                        \
MACROEnd

    
#define _MACRO_QueueGetCurrent(PTR__Queue) ((PTR__Queue)->head->data)


#define QueueIterator(PTR__Queue) __typeof__( (PTR__Queue)->head )
#define QueueIteratorStart(PTR__Queue) ( (PTR__Queue)->head )
#define QueueIteratorEnd(PTR__Queue) ((PTR__Queue)->tail)
#define QueueIteratorNext(__QueueIterator) ((__QueueIterator)->next?(__QueueIterator)->next:(__QueueIterator))
#define QueueIteratorData(__QueueIterator) ((__QueueIterator)->data)

#endif
