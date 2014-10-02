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

ListNode* ListCall
listnode_create(const void* data,ListNode* prev,ListNode* next,unsigned int elesize){
   ListNode* n = (ListNode*)malloc(sizeof(ListNode));
   n->data = (elesize?memcpy(malloc(elesize),data,elesize):(void*)data);
   n->prev = prev;
   if(n->prev != 0) n->prev->next = n;
   n->next = next;
   if(n->next != 0) n->next->prev = n;
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
   ListNode* next,*iter;
   if(list->size == 0) return;
   iter = list->front;
   next = iter->next;
   while(next!=0){
      if(list->elesize) free(iter->data);
      free(iter);
      iter = next;
      next = next->next;
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
      list->back = list->front = listnode_create(data,0,0,list->elesize);
      list->size = 1;
      return 1;
   }
   return 0;
}

void  ListCall
list_push_back(List* list,const void* data){
   if( list_first_push(list,data) ) return;
   list->back->next = listnode_create(data,list->back,0,list->elesize);
   list->back = list->back->next;
   ++list->size;
}

void  ListCall
list_push_front(List* list,const void* data){
   if( list_first_push(list,data) ) return;
   list->front->prev = listnode_create(data,0,list->front,list->elesize);
   list->front = list->front->prev;
   ++list->size;
}

static INLINE void ListCall
list_pop_helper(List* list,int isBack,ListNode* topop){
   ListNode* n;
   if(list->size == 0) return;
   if( isBack ){
	  n = topop->prev;
	  if(n) n->next = 0;
   }
   else {
	  n = topop->next;
	  if(n) n->prev = 0;
   }
   if(list->elesize > 0) free(topop->data);
   free(topop);
   if(isBack){
      list->back = n;
   }
   else{
      list->front = n;
   }
   --list->size;
}


void  ListCall
list_pop_back(List* list){
   list_pop_helper(list,1,list->back);
}

void  ListCall
list_pop_front(List* list){
   list_pop_helper(list,0,list->front);
}

void* ListCall
list_front(const List* list){
   if(list->size == 0) return 0;
   return list->front->data;
}

void* ListCall
list_back(const List* list){
   if(list->size == 0) return 0;
   return list->back->data;
}

void  ListCall list_foreach(List* list, void (*fn)(void*) ){
   ListNode* iter;
   for(iter = list->front; iter!= 0; iter=iter->next)
      fn(iter->data);
}

#include <stdio.h>
int ListCall
list_erase(List* list,const void* data){
   ListNode* n = list_find_first_node(list,data);
   if(!n) return 0;
   list_erase_node(list,n);
   return 1;
}

static INLINE ListNode* ListCall
list_find_node_helper(const List* list,ListNode* iter, const void* data,int dir,int ifnotequal){
   if(list_empty(list)) return 0;
   while(iter!= 0){
      if((list->eql(iter->data,data) !=0 ) ^ ifnotequal )
         return iter;
	  if(!dir){
         iter = iter->prev;
	  }
	  else{
         iter = iter->next;
	  }
   }
   return 0;
}

ListNode* ListCall
list_find_first_node(List* list, const void* data){
   return list_find_node_helper(list,list->front,data,1,0);
}

ListNode* ListCall
list_find_last_node(List* list, const void* data){
   return list_find_node_helper(list,list->back,data,0,0);
}

ListNode* ListCall
list_find_first_not_node(List* list, const void* data){
   return list_find_node_helper(list,list->front,data,1,1);
}

ListNode* ListCall
list_find_last_not_node(List* list, const void* data){
   return list_find_node_helper(list,list->back,data,0,1);
}

int  ListCall
list_erase_node(List* list,ListNode* node){
   if(list == 0 || list->size == 0 || node == 0) return 0;
   if(node == list->front){
	  list_pop_front(list);
	  return 1;
   }
   else if( node == list->back ){
	  list_pop_back(list);
	  return 1;
   }
   else{
      if(node->prev) {
         node->prev->next = node->next;
      }
      if(node->next) {
         node->next->prev = node->prev;
      }
      if(list->elesize) {
	     free(node->data);
	  }
      free(node);
      --list->size;
	  return 1;
   }
}

static INLINE void ListCall
list_remove_helper(List* list,const void* data,int ifnotequal){
   ListNode* iter = list->front;
   ListNode* next;
   while(list->size && iter){
       next = iter->next;
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
   ListNode* n = list_find_first_node(list,data);
   if(!n) return 0;
   return n->data;
}

void* ListCall
list_find_last(List* list, const void* data){
   ListNode* n = list_find_last_node(list,data);
   if(!n) return 0;
   return n->data;
}

void* ListCall
list_find_first_not(List* list, const void* data){
   ListNode* n = list_find_first_not_node(list,data);
   if(!n) return 0;
   return n->data;
}

void* ListCall
list_find_last_not(List* list, const void* data){
   ListNode* n = list_find_last_not_node(list,data);
   if(!n) return 0;
   return n->data;
}

static ListNode*  ListCall
list_find_next_node_helper(List* list,const ListNode* node,const void* data,int dir){
   ListNode* current = (ListNode*)node;
   if( list == NULL || node == NULL || (dir?current->next:current->prev) == NULL)
      return NULL;
   while( (current = (dir?current->next:current->prev)) != NULL){
      if( list->eql(data,current->data) )
         return current;
   }
   return NULL;
}

DLLIMPORT ListNode*  ListCall
list_find_next_node(List* list,const ListNode* node,const void* data){
   return list_find_next_node_helper(list,node,data,1);
}

DLLIMPORT ListNode*  ListCall
list_find_prev_node(List* list,const ListNode* node,const void* data){
   return list_find_next_node_helper(list,node,data,0);
}

DLLIMPORT void*  ListCall
list_node_get_data(const ListNode* node){
   if(node == NULL)
      return NULL;
   return node->data;
}

