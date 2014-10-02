#ifndef MACRO_STACK_H_
#define MACRO_STACK_H_
#include "src/src_macro_stack.h"
/********************(Macro-Stack v2.03 Build by IceCold Sunneo)****************
 *
 *  With this macro library 
 *  you could:
 *             define a type-specialized stack
 *             and use the following manipulation to operate it
 *
 *             StackDefine(TYPE)   
 *                 : Define a Stack  with type-specialized 
 *             StackDeclare(TYPE)                    
 *                 : Declare a defined stack
 *             StackCreate(TYPE)           
 *                 : Just like Constructor
 *             StackIsEmpty(PTR__Stack)   
 *                 : Is The Stack Empty ?           
 *             StackIsFull(PTR__Stack)   
 *                 : Is The Stack Full ? (Under Restrict mode )            
 *             StackIsRestrict(PTR__Stack)           
 *                 : Is The Stack under Restrict mode?
 *             StackSetMaxSize(PTR__Stack,__SIZE)  
 *                 : Restrict the Stack in Size
 *             StackRestrict(PTR__Stack)       
 *                 : Restrict the Stack in current size      
 *             StackUnRestrict(PTR__Stack)        
 *                 : To UnRestrict it
 *             StackPush(PTR__Stack,DATA)            
 *                 : Push Data into Stack
 *             StackCurrent(PTR__Stack)  
 *             StackTop(PTR__Stack)
 *                 : Get The Current data 
 *             StackPop(PTR__Stack)      
 *                 : Pop data from stack            
 *             StackPopTo(PTR__Stack,PTR__Container) 
 *                 : Pop data from stack to Container
 *
 ******************************************************************************/


#define StackDefine(TYPE)                                                                           _MACRO_StackDefine(TYPE)
#define StackDeclare(TYPE)                                                                          _MACRO_StackDeclare(TYPE)
#define StackCreate(PTR__Stack)                                                                     _MACRO_StackCreate(PTR__Stack)
#define StackDestroy(PTR__Stack)                                                                    _MACRO_StackDestroy(PTR__Stack)

#define StackIsEmpty(PTR__Stack)                                                                    _MACRO_StackIsEmpty(PTR__Stack)
#define StackIsFull(PTR__Stack)                                                                     _MACRO_StackIsFull(PTR__Stack)
#define StackIsRestrict(PTR__Stack)                                                                 _MACRO_StackIsRestrict(PTR__Stack)

#define StackSetRestrictSize(PTR__Stack,__SIZE)                                                     _MACRO_StackSetRestrictSize(PTR__Stack,__SIZE)
#define StackSetMaxSize(PTR__Stack,__SIZE)                                                          _MACRO_StackSetRestrictSize(PTR__Stack,__SIZE)
#define StackRestrict(PTR__Stack)                                                                   _MACRO_StackRestrict(PTR__Stack)
#define StackUnRestrict(PTR__Stack)                                                                 _MACRO_StackUnRestrict(PTR__Stack)

#define StackPush(PTR__Stack,DATA)                                                                  _MACRO_StackPush(PTR__Stack,DATA)
#define StackCurrent(PTR__Stack)                                                                    _MACRO_StackCurrent(PTR__Stack)
#define StackGetCurrent(PTR__Stack)                                                                 _MACRO_StackGetCurrent(PTR__Stack)
#define StackTop(PTR__Stack)                                                                        _MACRO_StackTop(PTR__Stack)
#define StackPop(PTR__Stack)                                                                        _MACRO_StackPop(PTR__Stack)
#define StackPopTo(PTR__Stack,PTR__Container)                                                       _MACRO_StackPopTo(PTR__Stack,PTR__Container)
#define StackGetSize(PTR__Stack)                                                                    _MACRO_StackGetSize(PTR__Stack)

#endif
