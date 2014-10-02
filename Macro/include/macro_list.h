#ifndef _MACRO_LIST_H_
#define _MACRO_LIST_H_
#include "src/src_macro_list.h"
/********************(Macro-list built by IceCold_Sunneo -modified version)************
 *  Modified:
 *       CreateList(TYPE)               ----> CreateList(node)
 *       ListPushBack(TYPE,Node,DATA)    ---> ListPushBack(Node,DATA) 
 *       ListPushFront(TYPE,Node,DATA)   ---> ListPushFront(Node,DATA)
 *       ListInsertBack(TYPE,node,DATA)  ---> ListInsertBack(node,DATA)
 *       ListInsertFront(TYPE,node,DATA)  --> ListInsertFront(node,DATA)
 *
 *  With this macro
 *  you could:
 *             define a type of list - as
 *             you build a unique-type list structure  
 *             and operate them.
 *             you have the following operand
 *   =================== basic operand for create  =======================
 *
 *   defList(TYPE)                   : Define a type of list(before function)
 *      - ListDefine(TYPE)               (alias)
 *   declList(TYPE)                  : Declare a list of type
 *      - ListDeclare(TYPE)              (alias)   
 *   CreateList(node)                : Allocate memory for a node
 *      - ListCreateNode(node)           (alias)   
 *      - ListCreate(node)               (alias)   
 *   ListInit(Node)                  : Initialize a node
 *   NodeLink(head,node)             : Link two node(s)
 *   ListLink(list1,list2)           : Link two list(s)
 *
 *   =================== operand for data =================================
 *
 *   ListPushBack(Node,DATA)         : Push DATA  after node
 *   ListPushFront(Node,DATA)        : Push DATA  in front of node
 *   ListPopBack(node)               : pop a data after node(at tail)
 *   ListPopFront(node)              : pop a data in front of node(at head)
 *   ListInsertBack(node,DATA)       : Insert a DATA after node
 *   ListInsertFront(node,DATA)      : Insert a DATA in front of node
 *
 *   ==================  operand for release memory ========================
 *
 *   ListDeleteNode(node)            : Delete a node
 *   ListDeleteNodeIndex(head,index) : Delete a node from node count to index
 *   ListDeepCopy(dst,src)           : Deep-copy from src to dst
 *   ListClearFront(node)            : Clear all elements in front of node(remain node)
 *   ListClearBack(node)             : Clear all elements after node(remain node) 
 *   ListRelease(node)               : keep node,clear all element
 *   ListDestroy(node)               : destroy whole list
 *
 *   ==================  operand for pointer ===============================
 *
 *   GetHead(node,HeadName)          : Declare a node named (headname) point to node's head 
 *      - ListGetHead(node,HeadName)     (alias)  
 *   GetTail(node,TailName)          : Declare a node named (headname) point to node's tail 
 *      - ListGetTail(node,TailName)     (alias)
 *   defFuncGetTail(TYPE)            : Define a function to get tail of type
 *   useFuncGetTail(TYPE,node)       : call the defined function to get tail
 *   defFuncGetHead(TYPE)            : Define a function to get tail of type
 *   useFuncGetHead(TYPE,node)       : call the defined function to get head
 *   ListShow(Node)                  : show the list's elements
 *
 *****************************************************************************/
/* standard operand */
#define defList(TYPE)                   _MACRO_DefineList(TYPE)    
#define declList(TYPE)                  _MACRO_DeclareList(TYPE) 
#define CreateList(TYPE)                _MACRO_CreateList(TYPE)
#define ListInit(Node)                  _MACRO_ListInit(Node)
#define NodeLink(head,node)             _MACRO_Nodelink(head,node)
#define ListLink(list1,list2)           _MACRO_ListLink(list1,list2)
/* data operand */
#define ListPushBack(Node,DATA)         _MACRO_ListPushBack(Node,DATA)
#define ListPushFront(Node,DATA)        _MACRO_ListPushFront(TYPE,Node,DATA)
#define ListPopBack(node)               _MACRO_ListPopBack(node)
#define ListPopFront(node)              _MACRO_ListPopFront(node)
#define ListInsertBack(node,DATA)       _MACRO_ListInsertBack(node,DATA)
#define ListInsertFront(node,DATA)      _MACRO_ListInsertFront(node,DATA)
#define ListDeleteIndex(head,index)     _MACRO_ListDeleteIndex(head,index)
#define ListDeleteNode(node)            _MACRO_ListDeleteNode(node)
#define ListDeepCopy(dst,src)           _MACRO_ListDeepCopy(dst,src)
/* release operand */
#define ListClearFront(node)            _MACRO_ListClearFront(node)
#define ListClearBack(node)             _MACRO_ListClearBack(node)
#define ListRelease(node)               _MACRO_ListRelease(node)
#define ListDestroy(node)               _MACRO_ListDestroy(node)
/* ptr operand */
#define GetHead(node,HeadName)          _MACRO_GetHead(node,HeadName)
#define GetTail(node,TailName)          _MACRO_GetTail(node,TailName)
#define defFuncGetTail(TYPE)            _MACRO_DefineFunction_GetTail(TYPE)
#define useFuncGetTail(TYPE,node)       _MACRO_UseFunction_GetTail(TYPE,node)
#define defFuncGetHead(TYPE)            _MACRO_DefineFunction_GetHead(TYPE)
#define useFuncGetHead(TYPE,node)       _MACRO_UseFunction_GetHead(TYPE,node)
#define ListShow(Node)                  _MACRO_ListShow(Node)
/* define alias */
#define ListGetHead(node,HeadName)      GetHead(node,HeadName)
#define ListGetTail(node,TailName)      GetTail(node,TailName)
#define ListCreateNode(node)            CreateList(node)
#define ListCreate(node)                ListCreateNode(node)
#define ListDefine(TYPE)                defList(TYPE)
#define ListDeclare(TYPE)               declList(TYPE)

#endif
