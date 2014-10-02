#include <stddef.h>
#include "hashtbl.h"
#include "list.h"
#include "hash_pvt_gbl.h"
#include <stdlib.h>

int
listDataPair_equal(const void* a,const void* b){
   int (*equal)(const void*,const void*) = ((ListDataPair*)a)->parentHash->equal_function;
   return equal( ( (ListDataPair*)a )->key , ( (ListDataPair*)b )->key );
}

static int
hashDefaultEqualFunction(const void* a,const void* b){
   return a == b;
}


void*
listDataPair_new(HashTable* parent,void* key,const void* data){
   ListDataPair* ret = (ListDataPair*)malloc(sizeof(ListDataPair));
   ret->key = key;
   ret->parentHash = parent;
   ret->data = (void*)data;
   return (void*)ret;
}

HashTable*
hashtbl_create(int size, unsigned int(*hashFunction)(const void*),int (*keyMatchFunction)(const void*,const void*) ){
   int iterList;
   HashTable* tbl = (HashTable*)malloc(sizeof(HashTable));
   /**
    * int refSizeTable[4] = {size,HASHTABLE_DEFAULT_SIZE-1,HASHTABLE_MINIMAL_SIZE - 1,HASHTABLE_DEFAULT_SIZE-1};
    * size = refSizeTable[ (((size < HASHTABLE_MINIMAL_SIZE)<<1) | (size <= 0)) ];
    *
    * comments:
    *   if size <= 0 then size = HASHTABLE_DEFAULT_SIZE-1
    *   else if size > 0 and size < HASHTABLE_MINIMAL_SIZE then size = HASHTABLE_MINIMAL_SIZE
    *   else keep the size.
    */
   int refSizeTable[4] = {size,HASHTABLE_DEFAULT_SIZE-1,HASHTABLE_MINIMAL_SIZE - 1,HASHTABLE_DEFAULT_SIZE-1};
   size = refSizeTable[ (((size <= HASHTABLE_MINIMAL_SIZE)<<1) | (size <= 0)) ];
   tbl->equal_function = keyMatchFunction?keyMatchFunction:hashDefaultEqualFunction;
   tbl->keyset = (void*)list_create(0,0);
   tbl->count = 0;
   tbl->size = size;
   tbl->data = malloc(sizeof(void*) * size );
   for(iterList = 0; iterList < size; ++iterList){
      ((void**)tbl->data)[ iterList ] = list_create(sizeof(ListDataPair),listDataPair_equal);
   }
   tbl->hash_function = hashFunction?hashFunction:defaultHashFunction;
   return tbl;
}

__inline static int
iabs(int i)
{
   return i<0?-i:i;
}

int 
hashtbl_empty(const HashTable* hashtbl){
   return hashtbl?hashtbl_count(hashtbl)==0:1;
}


int
hashtbl_genkey(const HashTable* hashtbl,unsigned int* iptrKey,const void* key){
   if(hashtbl == NULL) {
      return 0;
   }
   *iptrKey = (iabs( hashtbl->hash_function(key) ) %hashtbl->size);
   return 1;
}

int
hashtbl_put(HashTable* hashtbl,const void* key,const void* data){
   unsigned int ikey;
   List* list;
   ListDataPair tmpData = { hashtbl,(void*)key,(void*)data };
   if(!hashtbl_genkey(hashtbl,&ikey,key)) return 0;
   list = ((List**)hashtbl->data)[ ikey ];
   if( list_empty(list) ){ /* push the first key to keyset */
      list_push_back( hashtbl->keyset , (const void*)ikey );
   }
/*   if(list_find_first_node(list,&tmpData) != NULL )   return 0; */
   list_push_back(list,&tmpData);
   ++hashtbl->count;
   return 1;
}

List*
hashtbl_find_list_with_key(const HashTable* hashtbl,const void* key){
   unsigned int ikey;
   if(!hashtbl_genkey(hashtbl,&ikey,key)) return 0;
   return ((List**)hashtbl->data)[ ikey ];
}

int
hashtbl_exist_key(const HashTable* hashtbl,const void* key){
   List* list;
   ListDataPair tmpData = { (HashTable*)hashtbl,(void*)key,(void*)key };
   if( hashtbl_empty(hashtbl) ) return 0;
   list  = hashtbl_find_list_with_key(hashtbl,key);
   return list_find_first_node(list,&tmpData) != NULL;
}

void*
hashtbl_get(HashTable* hashtbl,const void* key){
   List* list = hashtbl_find_list_with_key(hashtbl,key);
   ListDataPair tmpData = { hashtbl,(void*)key,0 };
   ListNode* node = list_find_first_node(list,&tmpData);
   if(!node) return NULL;
   return ((ListDataPair*)list_node_get_data(node))->data;
}
#include <stdio.h>
int
hashtbl_remove(HashTable* hashtbl,const void* key){
   int ikey;
   ListDataPair tmpData = { hashtbl,(void*)key,(void*)key };
   if( hashtbl_empty(hashtbl) || !hashtbl_genkey(hashtbl,&ikey,key) ) return 0;
   if(!list_erase(((List**)hashtbl->data)[ ikey ],&tmpData)) return 0;
   if( list_empty(((List**)hashtbl->data)[ ikey ]) ){
      list_erase(hashtbl->keyset,(const void*)ikey);
   }
   --hashtbl->count;
   return 1;
}

void
hashtbl_clear(HashTable* hashtbl){
   int i;
   if(hashtbl_empty(hashtbl)) return ;
   list_clear(hashtbl->keyset);
   for(i=0; i<hashtbl->size; ++i){
      list_clear(((List**)hashtbl->data)[ i ]);
   }
   hashtbl->count = 0;
}

int
hashtbl_count(const HashTable* hashtbl){
   return hashtbl?hashtbl->count:0;
}

int
hashtbl_itemcount(const HashTable* hashtbl,const void* key){
   int ikey;
   if(hashtbl_empty(hashtbl)) return 0;
   if(!hashtbl_genkey(hashtbl,&ikey,key) ) return 0;
   return list_size( ((List**)hashtbl->data)[ ikey ] );
}

void
hashtbl_delete(HashTable* hashtbl){
   int i;
   if(hashtbl == NULL) return ;
   list_delete(hashtbl->keyset);
   for(i=0; i<hashtbl->size; ++i){
      list_delete(((List**)hashtbl->data)[ i ]);
   }
   free(hashtbl);
}

int
hashtbl_get_keyset_size(const HashTable* hashtbl){
   if( hashtbl_empty(hashtbl) ) return 0;
   return list_size((List*)hashtbl->keyset);
}

void
hashtbl_get_keyset(const HashTable* hashtbl,void** dstKeySet,int bufsize){
   int keycnt;
   ListIter iter;
   if( ( (size_t)hashtbl | (size_t)dstKeySet) == 0 ) return ;
   keycnt = list_size(hashtbl->keyset);
   if(keycnt > bufsize) keycnt = bufsize;
   iter = list_get_iter(hashtbl->keyset);
   while( list_iter_hasNext(iter) && keycnt-- ){
      *dstKeySet ++ = list_iter_next(iter);
   }
   list_iter_delete(iter);
}
