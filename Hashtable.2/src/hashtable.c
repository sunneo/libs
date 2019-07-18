#include "hashtable.h"
#include <stdlib.h>
#include <stdio.h>
#define HASH_ENTRY_SIZE 4096
#define HASHNODE_BUCKET_LENGTH 32
#ifdef DEBUG
#  define __debug(x) x
#else 
#  define __debug(x)
#endif
#define debugEnter __debug(fprintf(stderr,"enter %s\n",__FUNCTION__);)
#define debugLeave __debug(fprintf(stderr,"exit  %s\n",__FUNCTION__);)

typedef struct HashNode{
   int size;
   const void* key[HASHNODE_BUCKET_LENGTH];
   const void* value[HASHNODE_BUCKET_LENGTH];
   struct HashNode *prev;
   struct HashNode *next;
}HashNode;

__inline static void __swap(const void** a,register int idx,register int idx2){
   const void* t = a[ idx ];
   a[ idx ] = a[ idx2 ];
   a[ idx2 ] = t;
}

__inline static void __hashnode_swap_idx(HashNode* n,register int idx1,register int idx2){
   __swap(n->value,idx1,idx2);
   __swap(n->key,idx1,idx2);
}

static HashNode* hashnode_new(const void* key,const void* data){
   HashNode* ret = (HashNode*)malloc(sizeof(HashNode));
   ret->size = 1;
   ret->key[0] = key;
   ret->value[0] = data;
   ret->prev = 0;
   ret->next = 0;
   return ret;
}

static int isPrime(int n){
   int i = 3;
   if(n < 2){
	  return 0;
   }
   if(n == 2){
	  return 1;
   }
   if((n&1) == 0){
	  return 0;
   }
   for(i=3; (i*i)<= n; i+=2){
	  if(n % i == 0){
		 return 0;
	  }
   }
   return 1;
}

static unsigned int get_hash_entry_size(){
   static int first = 1;
   static int result = HASH_ENTRY_SIZE;
   if(!first){
	  return result;
   }
   first = 0;
   if((result & 1) == 0) {
 	  --result;
   }
   while( result > 0 && !isPrime(result) ){
	  --result;
   }
   return result;
}

static unsigned int defaultHashFnc(const void* p){
   return ((unsigned int)p) % get_hash_entry_size();
}

static int defaultKeyEQL(const void* a,const void* b){
   return a == b;
}

struct Hashtable* 
hashtbl_new(unsigned int (*hashfnc)(const void*),int (*keyEQL)(const void*,const void*) ){
   struct Hashtable* ret = 0;   
   debugEnter
   ret = (struct Hashtable*)malloc(sizeof(struct Hashtable));

   ret->size = 0;
   int entrySize = get_hash_entry_size();
   ret->entry = (void**)malloc( sizeof(void*) * entrySize );
   memset(ret->entry, 0, sizeof(void*)*entrySize);
   ret->hashfnc = hashfnc?hashfnc:defaultHashFnc;
   ret->keyEQL = keyEQL?keyEQL:defaultKeyEQL;

   debugLeave
   return ret;
}

__inline static HashNode*
__hashtbl_get_internel(int* nodeIdx,const struct Hashtable* tbl,const void* key,int idx){
   HashNode* ret = 0;
   int findIdx = -1;
   int i;
   debugEnter
   *nodeIdx = -1;
   if(hashtbl_empty(tbl)){
	  return 0;
   }
   ret = tbl->entry[ idx ] ;
   if(ret == 0){
	  debugLeave
	  return 0;
   }

   while(ret != 0 ){
	  findIdx = -1;
#pragma unroll HASHNODE_BUCKET_LENGTH 
      for(i=0; i<ret->size; ++i){
         if( tbl->keyEQL(key,ret->key[ i ]) ){
         	 findIdx = i;
			 i = ret->size;
	     }
      }
	  if(findIdx != -1){
		 *nodeIdx = findIdx;
         debugLeave
		 return ret;
	  }
	  ret = ret->next;
   }
   return 0;
}

__inline static int 
__hashnode_pushback(HashNode* n,const void* key,const void* value){
   int back = n->size;
   if(back >= HASHNODE_BUCKET_LENGTH){
	  return 0;
   }
   n->key[ back ] = key;
   n->value[ back ] = value;
   ++n->size; 
   return 1;
}

void hashtbl_put(struct Hashtable* tbl,const void* key,const void* value){
   int hash = 0;
   int idx = 0;
   HashNode* exists = 0;
   HashNode* cur = 0;
   int nodeIdx;
   debugEnter
   if(key == 0 || value == 0){
	  return;
   }
   hash = tbl->hashfnc(key);
   idx = hash % get_hash_entry_size();
   exists = __hashtbl_get_internel(&nodeIdx,tbl,key,idx);
   if(exists != 0){
	  if(tbl->keyEQL(exists->key,key)){
		 debugLeave
		 return;
	  }
   }
   if(tbl->entry[ idx ] == 0){
	  tbl->entry[ idx ] = hashnode_new(key,value);
   }
   else{
	  cur = (HashNode*)tbl->entry[ idx ];
	  if(! __hashnode_pushback(cur,key,value) ){
         cur->prev = hashnode_new(key,value);
         cur->prev->next =  cur;
         tbl->entry[ idx ] =  cur->prev;
	  }
   }
   ++tbl->size;
   debugLeave
}

void* hashtbl_get(const struct Hashtable* tbl,const void* key){
   HashNode* ret = 0;
   int idx = 0;
   int nodeIdx = 0;
   debugEnter
   if(key == 0){
	  return 0;
   }
   idx = tbl->hashfnc(key) % get_hash_entry_size();
   ret = __hashtbl_get_internel(&nodeIdx,tbl,key,idx);
   if(!ret){
	  debugLeave
	  return 0;
   }
   debugLeave
   return (void*)ret->value[nodeIdx];
}

int hashtbl_size(const struct Hashtable* tbl){
   int ret = 0;
   debugEnter
   ret = tbl?tbl->size:0;
   debugLeave
   return ret;
}

int hashtbl_empty(const struct Hashtable* tbl){
   int ret = 0;
   debugEnter
   ret = (hashtbl_size(tbl) == 0);
   debugLeave
   return ret;
}



void* hashtbl_remove(struct Hashtable* tbl,const void* key){
   int hash = 0;
   int idx = 0;
   int nodeIdx;
   const void* value = 0;
   HashNode* node = 0;
   debugEnter
   if(tbl == 0 || key == 0){
	  debugLeave
	  return 0;
   }
   hash = tbl->hashfnc(key);
   idx = hash % get_hash_entry_size();
   node = __hashtbl_get_internel(&nodeIdx,tbl,key,idx);
   if(nodeIdx == -1 || node == 0){
	  debugLeave
	  return 0;
   }
   if(node->size == 1){
      if(node->prev == 0){
         tbl->entry[ idx ] = node->next;
		 if(node->next){
         	 node->next->prev = 0;
		 }
      }
	  else{
		 node->prev->next = node->next;
		 if(node->next){
		    node->next->prev = node->prev;
		 }
	  }
      value = node->value[ 0 ];
      free(node);
   }
   else{
	  value = node->value[ nodeIdx ];
	  __hashnode_swap_idx(node,nodeIdx,node->size-1);
	  --node->size;
   }
   --tbl->size;
   debugLeave
   return (void*)value;
}

int hashtbl_contains_key(const struct Hashtable* tbl,const void* key){
   int ret = 0;
   debugEnter
   if(hashtbl_empty(tbl)){
	  debugLeave
	  return 0;
   }
   ret = (hashtbl_get(tbl,key) != 0);
   debugLeave
   return ret;
}

static void hashtbl_clear_list(struct Hashtable* tbl,int idx){
   HashNode* n = 0;
   HashNode* del = 0;
   n = tbl->entry[ idx ];
   while(n != 0){
	  del = n;
	  n = n->next;
	  free(del);
   }
   if(n == 0){
	  return;
   }
   tbl->entry[ idx ] = 0;
}

void hashtbl_clear(struct Hashtable* tbl){
   int i = 0;
   int totalEntry = 0;
   debugEnter
   totalEntry = get_hash_entry_size();
   if(tbl == 0){
	  return ;
   }
   for(i=0; i<totalEntry; ++i){
	  hashtbl_clear_list(tbl,i);
   }
   tbl->size = 0;
   debugLeave
}

void hashtbl_delete(struct Hashtable* tbl){
   debugEnter
   if(tbl == 0){
	  debugLeave
	  return ;
   }
   hashtbl_clear(tbl);
   free(tbl);
   debugLeave
}

