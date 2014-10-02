
#include <stdio.h>
#include <stdlib.h>
#include "list.h"

int EqualInt(const void* a,const void* b){
   return *(int*)a == *(int*)b;
}

void* createInt(int* i,int data){
   *i = data;
   return i;
}

void incInt(void* data){
   ++*(int*)data;
}

void printInt(void* data){
   printf("%d ",*(int*)data);
}

int main(int argc, char *argv[])
{
   List* list = list_create(sizeof(int),EqualInt);
   int t;
   ListNode* n;
   int i;
   for(i=0; i<10; ++i)
     list_push_back(list,createInt(&t,rand() ));
   /*list_foreach(list,printInt);*/
   ListIter iter = list_get_iter(list);
   while( list_iter_hasNext(iter) ){
      printInt(list_iter_next(iter));
   }
   printf("\n");  
   list_pop_back(list);
   list_foreach(list,incInt);
   list_foreach(list,printInt);
   printf("\n");
   n = list_find_first_node(list,createInt(&t,846930887) );
   list_erase_node(list,n);
   list_foreach(list,printInt);
   list_delete(list);
   return 0;
}

