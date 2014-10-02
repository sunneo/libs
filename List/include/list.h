#ifndef LINLED_LIST_H_
#  define LINLED_LIST_H_


typedef int ( *EqualCompare ) ( const void*,const void* );
typedef void ( *FunctionPtr ) ( void* );

typedef struct tagListNode
{
   void* data;
   struct tagListNode* prev;
   struct tagListNode* next;
} ListNode;
typedef void *ListIter;

typedef struct List
{
   ListNode* front;
   ListNode* back;
   unsigned int elesize;
   unsigned int size;
   EqualCompare eql;
} List;

 List*         list_create ( unsigned int elesize,EqualCompare eql );
 unsigned int  list_size ( const List* list );
 int           list_empty ( const List* list );
 void          list_clear ( List* list );
 void          list_delete ( List* list );
 void          list_push_back ( List* list,const void* data );
 void          list_push_front ( List* list,const void* data );
 int           list_erase ( List* list,const void* data );
 int           list_erase_node ( List* list,ListNode* node );
 void          list_remove_equal ( List* list,const void* data );
 void          list_remove_not_equal ( List* list,const void* data );
 void          list_pop_back ( List* list );
 void          list_pop_front ( List* list );
 void*         list_front ( const List* list );
 void*         list_back ( const List* list );
 void          list_foreach ( List* list, FunctionPtr fn );



 ListNode*      list_find_first_node ( List* list, const void* data );
 ListNode*      list_find_last_node ( List* list, const void* data );
 ListNode*      list_find_first_not_node ( List* list, const void* data );
 ListNode*      list_find_last_not_node ( List* list, const void* data );
 ListNode*      list_find_next_node ( List* list,const ListNode* node,const void* data );
 void*          list_node_get_data ( const ListNode* node );


 void*         list_find_first ( List* list, const void* data );
 void*         list_find_last ( List* list, const void* data );
 void*         list_find_first_not ( List* list, const void* data );
 void*         list_find_last_not ( List* list, const void* data );

 ListIter      list_get_iter ( const List* list );
 int           list_iter_hasNext ( const ListIter listiter );
 void*         list_iter_data ( const ListIter iter );
 int           list_iter_hasPrev ( ListIter iter );
 void*         list_iter_prev ( ListIter iter );
 void*         list_iter_next ( ListIter iter );
 ListIter      list_iter_rewind ( ListIter iter );
 void          list_iter_delete ( ListIter iter );

#endif
