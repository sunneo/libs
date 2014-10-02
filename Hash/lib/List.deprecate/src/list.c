#include "list.h"
#include <stdlib.h>
#include <string.h>
#include "../include/dbl_list_node.h"




static int
default_equal_compare(const void* a,const void* b){
   return a==b;
}

List* ListCall
list_create(unsigned int elesize,EqualCompare eql){
   List* list = (List*)malloc(sizeof(List));
   list->front = list->back = 0;
   list->elesize = elesize;
   list->size = 0;
   list->eql = eql?eql:default_equal_compare;
   return list;
}

DblListNode* ListCall
dbllistnode_create(const void* data,DblListNode* prev,DblListNode* next,unsigned int elesize){
   DblListNode* n = (DblListNode*)malloc(sizeof(DblListNode));
   n->data = (elesize?memcpy(malloc(elesize),data,elesize):(void*)data);
   n->UDirNode.prev = prev;
   if(n->UDirNode.prev != 0) n->UDirNode.prev->UDirNode.next = n;
   n->UDirNode.next = next;
   if(n->UDirNode.next != 0) n->UDirNode.next->UDirNode.prev = n;
   return n;
}

unsigned int  ListCall
list_size(const List* list){
   return list->size;
}

int   ListCall
list_empty(const List* list){
   return (list== 0) || (list->size == 0);
}

void  ListCall
list_clear(List* list){
   DblListNode* next,*iter;
   if(list->size == 0) return;
   iter = ((DblListNode*)list->front);
   next = iter->UDirNode.next;
   while(next!=0){
      if(list->elesize) free(iter->data);
      free(iter);
      iter = next;
      next = next->UDirNode.next;
   }
   list->size = 0;
   list->front = list->back = 0;
}

void  ListCall
list_delete(List* list){
   list_clear(list);
   free(list);
}

static INLINE int ListCall
list_first_push(List* list,const void* data){
   if(list->size == 0){
      list->back = list->front = dbllistnode_create(data,0,0,list->elesize);
      list->size = 1;
      return 1;
   }
   return 0;
}

void  ListCall
list_push_back(List* list,const void* data){
   if(list_first_push(list,data) ) return;
   ((DblListNode*)list->back)->UDirNode.next = dbllistnode_create(data,((DblListNode*)list->back),0,list->elesize);
   list->back = ((DblListNode*)list->back)->UDirNode.next;
   ++list->size;
}

void  ListCall
list_push_front(List* list,const void* data){
   if(list_first_push(list,data) ) return;
   ((DblListNode*)list->front)->UDirNode.prev = dbllistnode_create(data,0,((DblListNode*)list->front),list->elesize);
   list->front = ((DblListNode*)list->front)->UDirNode.prev;
   ++list->size;
}

static INLINE void ListCall
list_pop_helper(List* list,int isBack,DblListNode* topop){
   DblListNode* n;
   if(list->size == 0) return;
   n = topop->UDirArray[ isBack^1 ];
   if(n) n->UDirArray[ isBack ] = 0;
   if(list->elesize > 0) free(topop->data);
   free(topop);
   if(isBack)
      list->back = n;
   else
      list->front = n;
   --list->size;
}


void  ListCall
list_pop_back(List* list){
   list_pop_helper(list,1,((DblListNode*)list->back));
}

void  ListCall
list_pop_front(List* list){
   list_pop_helper(list,0,((DblListNode*)list->front));
}

void* ListCall
list_front(const List* list){
   if(list->size == 0) return 0;
   return ((DblListNode*)list->front)->data;
}

void* ListCall
list_back(const List* list){
   if(list->size == 0) return 0;
   return ((DblListNode*)list->back)->data;
}

void  ListCall list_foreach(List* list, void (*fn)(void*) ){
   DblListNode* iter;
   for(iter = ((DblListNode*)list->front); iter!= 0; iter=iter->UDirNode.next)
      fn(iter->data);
}
#include <stdio.h>
void ListCall
list_erase(List* list,const void* data){
   DblListNode* n = list_find_first_node(list,data);
   if(!n) return;
   list_erase_node(list,n);
}

static INLINE DblListNode* ListCall
list_find_node_helper(const List* list,DblListNode* iter, const void* data,int dir,int ifnotequal){
   if(list_empty(list)) return 0;
   while(iter!= 0){
      if((list->eql(iter->data,data)!=0) ^ ifnotequal )
         return iter;
      iter=iter->UDirArray[ dir ];
   }
   return 0;
}

ListNode ListCall
list_find_first_node(List* list, const void* data){
   return list_find_node_helper(list,((DblListNode*)list->front),data,1,0);
}

ListNode ListCall
list_find_last_node(List* list, const void* data){
   return list_find_node_helper(list,((DblListNode*)list->back),data,0,0);
}

ListNode ListCall
list_find_first_not_node(List* list, const void* data){
   return list_find_node_helper(list,((DblListNode*)list->front),data,1,1);
}

ListNode ListCall
list_find_last_not_node(List* list, const void* data){
   return list_find_node_helper(list,((DblListNode*)list->back),data,0,1);
}

void ListCall
list_erase_node(List* list,ListNode node){
   if(list == 0 || list->size == 0 || node == 0) return;
   if(node == ((DblListNode*)list->front)){
      list_pop_front(list);
      return;
   }
   else if(node == ((DblListNode*)list->back)){
      list_pop_back(list);
      return;
   }
   if(((DblListNode*)node)->UDirNode.prev) {
      ((DblListNode*)node)->UDirNode.prev->UDirNode.next = ((DblListNode*)node)->UDirNode.next;
   }
   if(((DblListNode*)node)->UDirNode.next) {
      ((DblListNode*)node)->UDirNode.next->UDirNode.prev = ((DblListNode*)node)->UDirNode.prev;
   }
   if(list->elesize) free(((DblListNode*)node)->data);
   free(node);
   --list->size;
}

static INLINE void ListCall
list_remove_helper(List* list,const void* data,int ifnotequal){
   DblListNode* iter = ((DblListNode*)list->front);
   DblListNode* next;
   while(list->size && iter){
       next = iter->UDirNode.next;
       if( (list->eql(iter->data,data) != 0) ^ ifnotequal ){
          list_erase_node(list,iter);
       }
       iter = next;
   }
}

void ListCall
list_remove_equal(List* list,const void* data){
   list_remove_helper(list,data,0);
}

void ListCall
list_remove_not_equal(List* list,const void* data){
   list_remove_helper(list,data,1);
}


void* ListCall
list_find_first(List* list, const void* data){
   DblListNode* n = list_find_first_node(list,data);
   if(!n) return 0;
   return n->data;
}

void* ListCall
list_find_last(List* list, const void* data){
   DblListNode* n = list_find_last_node(list,data);
   if(!n) return 0;
   return n->data;
}

void* ListCall
list_find_first_not(List* list, const void* data){
   DblListNode* n = list_find_first_not_node(list,data);
   if(!n) return 0;
   return n->data;
}

void* ListCall
list_find_last_not(List* list, const void* data){
   DblListNode* n = list_find_last_not_node(list,data);
   if(!n) return 0;
   return n->data;
}

static ListNode  ListCall
list_find_next_node_helper(List* list,const ListNode node,const void* data,int dir){
   DblListNode* current = (DblListNode*)node;
   if( list == NULL || node == NULL || current->UDirArray[ dir ] == NULL)
      return NULL;
   while( (current = current->UDirArray[ dir ]) != NULL){
      if( list->eql(data,current->data) )
         return current;
   }
   return NULL;
}

DLLIMPORT ListNode  ListCall
list_find_next_node(List* list,const ListNode node,const void* data){
   return list_find_next_node_helper(list,node,data,DIRNext);
}

DLLIMPORT ListNode  ListCall
list_find_prev_node(List* list,const ListNode node,const void* data){
   return list_find_next_node_helper(list,node,data,DIRPrev);
}

DLLIMPORT void*  ListCall
list_node_get_data(const ListNode node){
   if(node == NULL)
      return NULL;
   return ((DblListNode*)node)->data;
}

