#include "hashtbl.h"
#include "list.h"
#include "hash_pvt_gbl.h"
#include <stdlib.h>
HashKeyIter
hashtbl_getkeyiter(const HashTable* hashtbl,const void* key){
   return list_get_iter(hashtbl_find_list_with_key(hashtbl,key));
}

int
hashtbl_keyiter_hasNext(const HashKeyIter keyiter){
   return list_iter_hasNext((ListIter)keyiter);
}

void*
hashtbl_keyiter_data(const HashKeyIter keyiter){
   ListDataPair* pair = (ListDataPair*)list_iter_data((const ListIter)keyiter);
   return pair?pair->data:0;

}

void*
hashtbl_keyiter_next(HashKeyIter keyiter){
   ListDataPair* pair = list_iter_next((const ListIter)keyiter);
   return pair?pair->data:0;
}


void
hashtbl_keyiter_delete(HashKeyIter iter){
   list_iter_delete(iter);
}

HashIter
hashtbl_getiter(const HashTable* hashtbl){
   HashIterStruct* ret = 0;
   if(hashtbl == 0) return 0;
   ret = (HashIterStruct*)malloc(sizeof(HashIterStruct));
   ret->listIter = list_get_iter(hashtbl->keyset); /* a keyset is a linkedlist */
   ret->hashtbl = (HashTable*)hashtbl;
   if( list_iter_hasNext(ret->listIter) ){
	  int idx = ((List**)hashtbl->data)[(int)list_iter_next(ret->listIter)];
      ret->listItemIter=list_get_iter( idx );
   }
   return ret;
}

int
hashtbl_iter_hasNext(const HashIter iter){
   if(!iter) return 0;
   if( hashtbl_empty( ((HashIterStruct*)iter)->hashtbl) )  return 0;
   if(list_iter_hasNext( ((HashIterStruct*)iter)->listItemIter )  ) return 1;
   return list_iter_hasNext( ((HashIterStruct*)iter)->listIter );
}



void*
hashtbl_iter_next(HashIter iter){  
   if(!iter) return 0;
   if(!hashtbl_iter_hasNext(iter)) return hashtbl_keyiter_data(((HashIterStruct*)iter)->listItemIter);
   if(!list_iter_hasNext(((HashIterStruct*)iter)->listItemIter) ){
      list_iter_delete(((HashIterStruct*)iter)->listItemIter);
      ((HashIterStruct*)iter)->listItemIter = list_get_iter( ((List**)((HashIterStruct*)iter)->hashtbl->data)[ (unsigned int)list_iter_next(((HashIterStruct*)iter)->listIter) ]);
   }
   return hashtbl_keyiter_next(((HashIterStruct*)iter)->listItemIter);
}

void
hashtbl_iter_delete(HashIter iter){
   list_iter_delete(((HashIterStruct*)iter)->listItemIter);
   list_iter_delete(((HashIterStruct*)iter)->listIter);
   free(iter);
}
