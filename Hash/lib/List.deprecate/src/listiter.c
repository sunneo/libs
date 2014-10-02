#include "list.h"
#include "../include/dbl_list_node.h"
#include <string.h>
#include <stdlib.h>
typedef struct {
   List* list;
   DblListNode* node;
   union{
      struct {
         short isBegin;
         short isEnd;
      }SDirEnd;
      short UEnd[ 2 ];
   }UDirEnd;
}ListIterStruct;

ListIter  ListCall
list_get_iter(const List* list){
   ListIterStruct* ret;
   if(list == 0) return 0;
   ret = (ListIterStruct*)malloc(sizeof(ListIterStruct));
   ret->list = (List*)list;
   ret->node = list->front;
   ret->UDirEnd.SDirEnd.isBegin = (ret->node == 0 || ret->node->UDirArray[ DIRPrev ] == 0);
   ret->UDirEnd.SDirEnd.isEnd = (ret->node == 0 || ret->node->UDirArray[ DIRNext ] == 0);
   return (ListIter)ret;
}

int ListCall
list_iter_hasNext(const ListIter listiter){
   return (listiter != 0) && !list_empty(((ListIterStruct*)listiter)->list) &&
       (  (  (ListIterStruct*)listiter)->UDirEnd.SDirEnd.isEnd == 0);
}

int ListCall
list_iter_hasPrev(ListIter listiter){
   return (listiter != 0) &&  !list_empty(((ListIterStruct*)listiter)->list) &&
          (((ListIterStruct*)listiter)->UDirEnd.SDirEnd.isBegin == 0) ;
}

void*
ListCall list_iter_data(const ListIter iter){
   if(iter && ((ListIterStruct*)iter)->node )
      return ((ListIterStruct*)iter)->node->data;
   return 0;
}

INLINE static void* ListCall
list_iter_next_dir(ListIter iter,int dir){
   void* ret;
   if(iter == 0) return 0;
   ret = list_iter_data(iter);
   if( ((ListIterStruct*)iter)->node->UDirArray[ dir ] ){
      ((ListIterStruct*)iter)->node = ((ListIterStruct*)iter)->node->UDirArray[ dir ];
      
   }
   ((ListIterStruct*)iter)->UDirEnd.UEnd[ dir^1 ] = (  ((ListIterStruct*)iter)->node->UDirArray[ dir^1 ] == 0);
   ((ListIterStruct*)iter)->UDirEnd.UEnd[ dir ] = (  ((ListIterStruct*)iter)->node->UDirArray[ dir ] == 0);
   return ret;
}

void* ListCall
list_iter_prev(ListIter iter){
   return list_iter_next_dir(iter,DIRPrev);
}

void* ListCall
list_iter_next(ListIter iter){
   return list_iter_next_dir(iter,DIRNext);
}

ListIter ListCall
list_iter_rewind(ListIter iter){
   if(iter == 0) return 0;
   ((ListIterStruct*)iter)->node = ((ListIterStruct*)iter)->list->front;
   return iter;
}

void ListCall
list_iter_delete(ListIter iter){
   free(iter);
}
