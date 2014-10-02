#include "hashtbl.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "list.h"

typedef struct tagTuple{
   int x,y,z;
}Tuple;

Tuple* tuple_create(int x,int y,int z){
   Tuple* ret = (Tuple*)malloc(sizeof(Tuple));
   ret->x = x;
   ret->y = y;
   ret->z = z;
   return ret;
}

static void tuple_print(const Tuple* tpl){
   printf("(%d,%d,%d)\n",tpl->x,tpl->y,tpl->z);
}

static int tuple_equal(const Tuple* tpl1,const Tuple* tpl2){
   return tpl1 == tpl2;
}

static int tuple_hash(void* pKey){
   return (int)pKey;
}

static void
hashtbl_foreach(HashTable* hash,void(*fn)(const void*)){
   HashIter iter;
   int cnt = 0;
   if(hashtbl_empty(hash)) return;
   iter = hashtbl_getiter(hash);
   printf("iter = %d\n",iter);
   printf("hasnext ?%d\n",hashtbl_iter_hasNext(iter));
   while(hashtbl_iter_hasNext(iter)){
	  fn(hashtbl_iter_next(iter));
	  ++cnt;
   }
   printf("after foreach cnt = %d,hashtbl_cnt = %d\n",cnt,hashtbl_count(hash));
   hashtbl_iter_delete(iter);
}

static void
hashtbl_clearall(HashTable* hash){
   hashtbl_foreach(hash,free);
}

int main(){
   HashTable* hash = hashtbl_create(128,tuple_hash,tuple_equal);
   int i;
   int key;
   int problemSize = 20;
   int keyRange = 10;
   for(i=0; i<problemSize; ++i){
      key = rand()%2;
      hashtbl_put(hash,1,tuple_create(i,key,rand()%100));
   }
   hashtbl_foreach(hash,tuple_print); 
   printf("end of insert\n");
   getchar();
   for(i=1; i<5; ++i){
      hashtbl_remove(hash,1);
   }
   
   
   hashtbl_foreach(hash,tuple_print); 
   /*hashtbl_clearall(hash);*/
   hashtbl_delete(hash);
   puts("Press any key to terminate");
   getchar();
   return 0;
}
