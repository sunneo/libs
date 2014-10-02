#ifndef LINLED_LIST_H_
#  define LINLED_LIST_H_

#  ifdef DLLLIB
#    ifdef BUILDING_DLL
#      define DLLIMPORT __declspec(dllexport)
#    else
#      define DLLIMPORT __declspec(dllimport)
#    endif
#  else
#    define DLLIMPORT
#  endif
#  ifndef ListCall
#    define ListCall __attribute__((__fastcall__))
#  endif

typedef int(*EqualCompare)(const void*,const void*);
typedef void(*FunctionPtr)(void*);
typedef void *ListNode;
typedef void *ListIter;

typedef struct List{
   ListNode front,back;
   unsigned int elesize;
   unsigned int size;
   EqualCompare eql;
}List;

DLLIMPORT List*        ListCall list_create(unsigned int elesize,EqualCompare eql);
DLLIMPORT unsigned int ListCall list_size(const List* list);
DLLIMPORT int          ListCall list_empty(const List* list);
DLLIMPORT void         ListCall list_clear(List* list);
DLLIMPORT void         ListCall list_delete(List* list);
DLLIMPORT void         ListCall list_push_back(List* list,const void* data);
DLLIMPORT void         ListCall list_push_front(List* list,const void* data);
DLLIMPORT void         ListCall list_erase(List* list,const void* data);
DLLIMPORT void         ListCall list_erase_node(List* list,ListNode node);
DLLIMPORT void         ListCall list_remove_equal(List* list,const void* data);
DLLIMPORT void         ListCall list_remove_not_equal(List* list,const void* data);
DLLIMPORT void         ListCall list_pop_back(List* list);
DLLIMPORT void         ListCall list_pop_front(List* list);
DLLIMPORT void*        ListCall list_front(const List* list);
DLLIMPORT void*        ListCall list_back(const List* list);
DLLIMPORT void         ListCall list_foreach(List* list, FunctionPtr fn);



DLLIMPORT ListNode     ListCall list_find_first_node(List* list, const void* data);
DLLIMPORT ListNode     ListCall list_find_last_node(List* list, const void* data);
DLLIMPORT ListNode     ListCall list_find_first_not_node(List* list, const void* data);
DLLIMPORT ListNode     ListCall list_find_last_not_node(List* list, const void* data);
DLLIMPORT ListNode     ListCall list_find_next_node(List* list,const ListNode node,const void* data);
DLLIMPORT void*        ListCall list_node_get_data(const ListNode node);


DLLIMPORT void*        ListCall list_find_first(List* list, const void* data);
DLLIMPORT void*        ListCall list_find_last(List* list, const void* data);
DLLIMPORT void*        ListCall list_find_first_not(List* list, const void* data);
DLLIMPORT void*        ListCall list_find_last_not(List* list, const void* data);

DLLIMPORT ListIter     ListCall list_get_iter(const List* list);
DLLIMPORT int          ListCall list_iter_hasNext(const ListIter listiter);
DLLIMPORT void*        ListCall list_iter_data(const ListIter iter);
DLLIMPORT int          ListCall list_iter_hasPrev(ListIter iter);
DLLIMPORT void*        ListCall list_iter_prev(ListIter iter);
DLLIMPORT void*        ListCall list_iter_next(ListIter iter);
DLLIMPORT ListIter     ListCall list_iter_rewind(ListIter iter);
DLLIMPORT void         ListCall list_iter_delete(ListIter iter);

#endif
