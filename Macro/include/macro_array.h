#ifndef MACRO_ARRAY_H_
#define MACRO_ARRAY_H_
#include "src/src_macro_array.h"
/******************(Macro-Array Build by IceCold Sunneo)**************************
 *  
 *
 *
 *
 *
 *
 *     ArrayDefine(TYPE)                   
 *     ArrayDeclare(TYPE)                  
 *     ArrayCreate(PTR__Array)             
 *     ArrayDestroy(PTR__Array)            
 *     ArrayAssign(PTR__Array,__INDEX,DATA)
 *     ArrayResize(PTR__Array,__NEWSIZE)
 *     ArrayIndexOf(PTR__Array,__INDEX)
 *     ArrayCopy(PTR__ArrayDst,PTR__ArraySrc)
 *     ArrayForEach(PTR__Array,__Function)
 *     ArrayPushBack(PTR__Array,__DATA)
 *     ArrayErase(PTR__Array)
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 **********************************************************************************/

#define ArrayDefine(TYPE)                                                                                                                                               _MACRO_ArrayDefine(TYPE)
#define ArrayDeclare(TYPE)                                                                                                                                              _MACRO_ArrayDeclare(TYPE)
#define ArrayCreate(PTR__Array)                                                                                                                                         _MACRO_ArrayCreate(PTR__Array)
#define ArrayDestroy(PTR__Array)                                                                                                                                        _MACRO_ArrayDestroy(PTR__Array)
#define ArrayAssign(PTR__Array,__INDEX,DATA)                                                                                                                            _MACRO_ArrayAssign(PTR__Array,__INDEX,DATA)
#define ArrayResize(PTR__Array,__NEWSIZE)                                                                                                                               _MACRO_ArrayResize(PTR__Array,__NEWSIZE)
#define ArrayIndexOf(PTR__Array,__INDEX)                                                                                                                                _MACRO_ArrayIndexOf(PTR__Array,__INDEX)
#define ArrayForEach(PTR__Array,__Function)                                                                                                                             _MACRO_ArrayForEach(PTR__Array,__Function)
#define ArrayPushBack(PTR__Array,__DATA)                                                                                                                                _MACRO_ArrayPushBack(PTR__Array,__DATA)
#define ArrayCopy(PTR__ArrayDst,PTR__ArraySrc)                                                                                                                          _MACRO_ArrayCopy(PTR__ArrayDst,PTR__ArraySrc)
#define ArrayErase(PTR__Array)                                                                                                                                          _MACRO_ArrayErase(PTR__Array)

#endif
