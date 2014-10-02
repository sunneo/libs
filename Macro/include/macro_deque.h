#ifndef MACRO_DOUBLE_ENDED_QUEUE_src_H_
#define MACRO_DOUBLE_ENDED_QUEUE_src_H_
#include "src/srcmacro_deque.h"

#define DEQueCreate(PTR__DEQUE)                                                                         _MACRO_DEQueCreate(PTR__DEQUE)
#define DEQuePushBack(PTR__DEQUE,__DATA)                                                                _MACRO_DEQuePushBack(PTR__DEQUE,__DATA)
#define DEQuePushFront(PTR__DEQUE,__DATA)                                                               _MACRO_DEQuePushFront(PTR__DEQUE,__DATA)
#define DEQuePopFront(PTR__DEQUE)                                                                       _MACRO_DEQuePopFront(PTR__DEQUE)
#define DEQuePopBack(PTR__DEQUE)                                                                        _MACRO_DEQuePopBack(PTR__DEQUE)
#define DEQueGetFront(PTR__DEQUE)                                                                       _MACRO_DEQueGetFront(PTR__DEQUE)
#define DEQueGetBack(PTR__DEQUE)                                                                        _MACRO_DEQueGetBack(PTR__DEQUE)
#define DEQueGetSize(PTR__DEQUE)                                                                        _MACRO_DEQueGetSize(PTR__DEQUE)
#define DEQueDefine(TYPE)                                                                               _MACRO_DEQueDefine(TYPE)
#define DEQueDeclare(TYPE)                                                                              _MACRO_DEQueDeclare(TYPE)
#define DEQueIsEmpty(PTR__DEQUE)                                                                        _MACRO_DEQueIsEmpty(PTR__DEQUE)
#define DEQueIsFull(PTR__DEQUE)                                                                         _MACRO_DEQueIsFull(PTR__DEQUE)

#endif
