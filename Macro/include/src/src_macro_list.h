#ifndef _MACRO_LIST_src_H_
#define _MACRO_LIST_src_H_
#include <stdio.h>
#include <stdlib.h>

#ifndef MACROBegin
#  define MACROBegin do{
#endif
#ifndef MACROEnd
#  define MACROEnd }while(0)
#endif

#define _MACRO_LIST(TYPE,_NAME)          \
struct list_##TYPE##_##_NAME             \
{                                        \
   TYPE data;                            \
   struct list_##TYPE##_##_NAME  *next;  \
   struct list_##TYPE##_##_NAME  *prev;  \
}

#define _MACRO_DefineList(TYPE)  typedef _MACRO_LIST(TYPE,linkedlist) def_list_##TYPE 
#define _MACRO_DeclareList(TYPE) def_list_##TYPE
#define _MACRO_CreateList(list)  ( __typeof__(list) ) calloc (1,sizeof(*list))

#define _MACRO_ListInit(PTR__node)       \
MACROBegin                               \
   (PTR__node)->data = 0;                \
   (PTR__node)->prev = NULL;             \
   (PTR__node)->next = NULL;             \
MACROEnd

#define _MACRO_ListShow(ptr_node,Show_FunctionPTR_ToNode)\
MACROBegin                                               \
   _MACRO_GetHead(ptr_node,head_tmp_ptr_node);           \
   while(head_tmp_ptr_node)                              \
   {                                                     \
      Show_FunctionPTR_ToNode(head_tmp_ptr_node);        \
      head_tmp_ptr_node = head_tmp_ptr_node->next;       \
   }                                                     \
   printf("\n");                                         \
MACROEnd

#define _MACRO_NodeLink(__head,__node)      \
MACROBegin                                  \
   __node->prev = __head;                   \
   if(__head)                               \
   {                                        \
      __node->next = __head->next;          \
      __head->next = __node;                \
   }                                        \
MACROEnd

#define _MACRO_DefineFunction_GetTail(TYPE)                                  \
_MACRO_DeclareList(TYPE)* FuncGetTail_##TYPE(_MACRO_DeclareList(TYPE)* node) \
{                                                                            \
   __typeof__(node) n = node;                                                \
   while(n->next)n = n->next;                                                \
   return n;                                                                 \
}

#define _MACRO_UseFunction_GetTail(TYPE,node) FuncGetTail_##TYPE(node)

#define _MACRO_DefineFunction_GetHead(TYPE)                                  \
_MACRO_DeclareList(TYPE)* FuncGetHead_##TYPE(_MACRO_DeclareList(TYPE)* node) \
{                                                                            \
   __typeof__(node) n = node;                                                \
   while(n->prev) n = n->prev;                                               \
   return n;                                                                 \
}

#define _MACRO_UseFunction_GetHead(TYPE,node) FuncGetHead##TYPE(node)

#define _MACRO_GetTail(node,TailName)                            \
__typeof__(node)  TailName = node;                               \
MACROBegin  while(TailName->next) TailName = TailName->next; MACROEnd

#define _MACRO_GetHead(node,HeadName)                            \
__typeof__(node)  HeadName = node;                               \
MACROBegin  while(HeadName->prev) HeadName = HeadName->prev; MACROEnd

#define _MACRO_ListLink(list1,list2)                    \
MACROBegin                                              \
   _MACRO_GetTail(list1,tail1_tmp_list1);               \
   _MACRO_GetHead(list2,head2_tmp_list2);               \
   tail1_tmp_list1->next = head2_tmp_list2;             \
   head2_tmp_list2->prev = tail1_tmp_list1;             \
MACROEnd

#define _MACRO_ListPushBack(ptrNode,DATA)                               \
MACROBegin                                                              \
   __typeof__(DATA) __tmp__DATA = DATA;                                 \
   _MACRO_GetTail(ptrNode,tail_tmp_ptrNode);                            \
   tail_tmp_ptrNode->next = _MACRO_CreateList(tail_tmp_ptrNode->next);  \
   tail_tmp_ptrNode->next->prev = tail_tmp_ptrNode;                     \
   memcpy(&(tail_tmp_ptrNode->data),&(__tmp__DATA),sizeof(__tmp__DATA));\
MACROEnd

#define _MACRO_ListPushFront(node,DATA)                              \
MACROBegin                                                           \
   __typeof__(DATA) __tmp__DATA = DATA;                              \
   _MACRO_GetHead(node,head_tmp_node);                               \
   memcpy(&(_tmp_DATA_n->data),&(__tmp__DATA),sizeof(DATA));         \
   head_tmp_node ->prev = _MACRO_CreateList(head_tmp_node ->prev);   \
   head_tmp_node ->prev->next = head_tmp_node;                       \
MACROEnd

#define _MACRO_ListPopBack(node)                   \
MACROBegin                                         \
   _MACRO_GetTail(node,tail_tmp_node);             \
   _MACRO_ListDeleteNode(tail_tmp_node);           \
MACROEnd

#define _MACRO_ListPopFront(node)       \
MACROBegin                              \
   _MACRO_GetHead(node,head);           \
   _MACRO_ListDeleteNode(head);         \
MACROEnd

#define _MACRO_ListInsertBack(node,DATA)                             \
MACROBegin                                                           \
   __typeof__(node) _tmp_DATA_n = _MACRO_CreateList(_tmp_DATA_n);    \
   __typeof__(DATA) __tmp__DATA = (DATA);                            \
   memcpy(&(_tmp_DATA_n->data),&(__tmp__DATA),sizeof(DATA));         \
   _MACRO_NodeLink(node->next,_tmp_DATA_n);                          \
MACROEnd

#define _MACRO_ListInsertFront(node,DATA)                            \
MACROBegin                                                           \
   __typeof__(node) _tmp_DATA_n = _MACRO_CreateList(_tmp_DATA_n);    \
   __typeof__(DATA) __tmp__DATA = (DATA);                            \
   memcpy(&(_tmp_DATA_n->data),&(__tmp__DATA),sizeof(DATA));         \
   _MACRO_NodeLink(node->prev,_tmp_DATA_n);                          \
MACROEnd

#define _MACRO_ListDeleteNode(__node)                                    \
MACROBegin                                                               \
   __typeof__(__node) __node__del = __node;                              \
   if(__node__del->next)__node__del->next->prev = __node__del->prev;     \
   if(__node__del->prev)__node__del->prev->next = __node__del->next;     \
   free(__node__del);                                                    \
MACROEnd


#define _MACRO_ListDeleteIndex(node,index)             \
MACROBegin                                             \
   __typeof__(node) _tmp_TYPE_n = node;                \
   unsigned int uiindex = index;                       \
   while(uiindex-->0)                                  \
     _tmp__n = _tmp__n->next;                          \
   _MACRO_ListDeleteNode(_tmp__n);                     \
MACROEnd

#define _MACRO_ListDeepCopy(dst,src)                                                           \
MACROBegin                                                                                     \
   __typeof__(dst) _tmp_dst_pdst = dst;                                                      \
   __typeof__(src) _tmp_dst_psrc = src;                                                      \
   while(_tmp_dst_psrc->next&&_tmp_dst_pdst->next)                                           \
   {                                                                                         \
      memcpy(&(_tmp_dst_pdst->data),&(_tmp_dst_psrc->data),sizeof(_tmp_dst_psrc->data));     \
      _tmp_dst_psrc = _tmp_dst_psrc->next;                                                   \
      _tmp_dst_pdst = _tmp_dst_pdst->next;                                                   \
   }                                                                                           \
MACROEnd

#define _MACRO_ListClearFront(node)                                                \
MACROBegin                                                                         \
   _MACRO_GetHead(node,_tmp_node_ptr);                                             \
   while(_tmp_node_ptr!=node&&_tmp_node_ptr->next!=node&&_tmp_node_ptr->next)      \
   {                                                                               \
       _tmp_node_ptr=_tmp_node_ptr->next;                                          \
       free(_tmp_node_ptr->prev);                                                  \
   }                                                                               \
   if(node->prev)free(node->prev);                                                 \
   node->prev = NULL;                                                              \
MACROEnd

#define _MACRO_ListClearBack(node)                                  \
MACROBegin                                                          \
   __typeof__(node) _tmp_node_ptr = node;                           \
   if(_tmp_node_ptr->next)_tmp_node_ptr = _tmp_node_ptr->next;      \
   while(_tmp_node_ptr->next)                                       \
   {                                                                \
      _tmp_node_ptr = _tmp_node_ptr->next;                          \
      free(_tmp_node_ptr->prev);                                    \
   }                                                                \
   if(_tmp_node_ptr!=node&&_tmp_node_ptr)free(_tmp_node_ptr);       \
   node->next = NULL;                                               \
MACROEnd

#define _MACRO_ListRelease(node)               \
MACROBegin                                     \
   _MACRO_ListClearFront(node);                \
   _MACRO_ListClearBack(node);                 \
MACROEnd

#define _MACRO_ListDestroy(node)               \
MACROBegin                                     \
   _MACRO_ListRelease(node);                   \
   free(node);                                 \
MACROEnd

#endif
