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
   list_foreach(list,printInt);
   printf("\n");
   list_foreach(list,incInt);
   n = list_find_last_not_node(list,createInt(&t,24465) );
   list_erase_node(list,n);
   list_foreach(list,printInt);
   list_delete(list);
   system("PAUSE");	
   return 0;
}
