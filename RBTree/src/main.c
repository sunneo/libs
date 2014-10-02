#include <stdio.h>
#include <stdlib.h>
#include "rbtree.h"
int icmp(const void* a,const void* b){
   if(a>b) return 1;
   else if(a == b) return 0;
   return -1;
}

int main(int argc, char *argv[])
{
   RBTree* rbtree = rbtree_create(0,0,icmp);
   int i;
   rbtree_insert(rbtree,5,(const void*)5);
   rbtree_insert(rbtree,1,(const void*)1);
   rbtree_insert(rbtree,4,(const void*)4);
   rbtree_insert(rbtree,3,(const void*)3);
   rbtree_insert(rbtree,0,(const void*)0);
   printf("size = %d\n",rbtree_size(rbtree));
   rbtree_erase(rbtree,(const void*)4);
   printf("size = %d\n",rbtree_size(rbtree));
   rbtree_delete(rbtree);
   system("PAUSE");	
   return 0;
}
