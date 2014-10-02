#include "hashmap.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bst.h"

#define HASH_ENTRY_SIZE 1024
#ifdef DEBUG
#  define __debug(x) x
#else
#  define __debug(x)
#endif
#define debugEnter __debug(fprintf(stderr,"%s:%s[line:%d]: enter %s\n",__FILE__,__FUNCTION__,__LINE__,__FUNCTION__);)
#define debugLeave __debug(fprintf(stderr,"%s:%s[line:%d]: exit  %s\n",__FILE__,__FUNCTION__,__LINE__,__FUNCTION__);)
#define __traceMSG(X) __debug(fprintf(stderr,"%s:%s[line:%d]: %s\n",__FILE__,__FUNCTION__,__LINE__,(X)))
typedef struct HashNode {
   const void* key;
   const void* value;
   struct HashNode *prev;
   struct HashNode *next;
}HashNode;


static void* __malloc_s (unsigned int size ) {
   void* ret;
   ret = calloc (1,size);
   if ( !(ret) ) {
      __debug ( fprintf ( stderr,"[bad alloc], program terminated\n" ) );
      exit ( -1 );
   }
   return ret;
}

#define malloc_s(S) __malloc_s((unsigned int)(S))

static HashNode* hashnode_new ( const void* key,const void* data ) {
   HashNode* ret = malloc_s(sizeof(HashNode));
   ret->key = key;
   ret->value = data;
   ret->prev = 0;
   ret->next = 0;
   return ret;
}

static int isPrime ( int n ) {
   int i = 3;
   if ( n < 2 ) {
      return 0;
   }
   if ( n == 2 ) {
      return 1;
   }
   if ( ( n&1 ) == 0 ) {
      return 0;
   }
   for ( i=3; i< n; i+=2 ) {
      if ( n % i == 0 ) {
         return 0;
      }
   }
   return 1;
}

static int get_hash_entry_size() {
   static int first = 1;
   static int result = HASH_ENTRY_SIZE;
   if ( !first ) {
      return result;
   }
   first = 0;
   if ( ( result & 1 ) == 0 ) {
      --result;
   }
   while ( result > 0 && !isPrime ( result ) ) {
      --result;
   }
   return result;
}

static int defaultHashFnc ( const void* p ) {
   return ( ( int ) p ) % get_hash_entry_size();
}

static int defaultKeyEQL ( const void* a,const void* b ) {
   return a == b;
}


struct HashMap* hashmap_new ( int ( *hashfnc ) ( const void* ),int ( *keyEQL ) ( const void*,const void* ) ) {
   struct HashMap* ret = 0;
   int hash_entry_size = get_hash_entry_size();
   debugEnter
   ret = (struct HashMap*)malloc_s(sizeof(struct HashMap));

   ret->size = 0;
   ret->entry = (void**)malloc_s(hash_entry_size * sizeof(void*));
   if ( hashfnc == 0 ) {
      ret->hashfnc = defaultHashFnc;
   } else {
      ret->hashfnc = hashfnc;
   }
   if ( keyEQL == 0 ) {
      ret->keyEQL = defaultKeyEQL;
   } else {
      ret->keyEQL = keyEQL;
   }

   debugLeave
   return ret;
}

static HashNode* 
__hash_list_search_key(const struct HashMap* map,HashNode* n,const void* key,void** tail){
   *tail = 0;
   while ( n != 0) {                                          
	  if(n->next == 0){
		 *tail = n;
	  }
	  if((*map->keyEQL)(key,n->key)){
		 return n;
	  }
      n = n->next;
   }
   return 0;
}
#define hash_list_search_key(MAP,NODE,KEY,TAIL) \
   __hash_list_search_key((const struct HashMap*)(MAP),(HashNode*)(NODE),(const void*)(KEY),(void**)(TAIL))

#define HASHGET_FOUND 0
#define HASHGET_NOT_IN_HASHLIST 1
#define HASHGET_NOT_IN_BINNODE 2
int
__hashmap_get_internel (
   const struct HashMap* map,
   HashNode** ret,
   BST** ret_bst,
   const void* key,
   int idx,
   int origHash
) {
   HashNode* ret_hash = 0;
   HashNode* search_result = 0;
   HashNode* tail;
   BST* bst;
   debugEnter
   bst = map->entry[ idx ] ; /* get a binTree */
   *ret_bst = bst;
   if ( map->size == 0 ) {
      debugLeave
      *ret = 0;
      return HASHGET_NOT_IN_BINNODE;
   }
   if( bst == 0 ){
	  search_result = 0;
   }
   else{
      search_result = bst_search ( bst, ( const void* ) origHash );
   }
   if ( search_result == 0 ) {
      debugLeave
      __debug ( fprintf ( stderr,"__hashmap_get_internal: reason: no such key in tree (%d)\n",origHash ) );
      *ret = ret_hash;
      return HASHGET_NOT_IN_BINNODE;
   }
   ret_hash = hash_list_search_key(map,search_result,key,&tail);
   debugLeave
   if ( ret_hash == 0 ) {
      /*
       * not found, write the tail of the list back to return.
       */
      *ret = tail;
      debugLeave
      __debug ( fprintf ( stderr,"reason: HASHGET_NOT_IN_HASHLIST\n" ) );
      return HASHGET_NOT_IN_HASHLIST;
   }
   *ret = ret_hash;
   debugLeave
   return HASHGET_FOUND;
}

void hashmap_put ( struct HashMap* map,const void* key,const void* value ) {
   int hash = 0;
   int idx = 0;
   int result = 0;
   HashNode* exists = 0;
   BST* bst = 0;
   debugEnter
   hash = map->hashfnc ( key );
   idx = hash % get_hash_entry_size();
   if ( map->entry[ idx ] == 0 ) {
      map->entry[ idx ] = bst_create ( 0,0,0 );
      bst = (BST*)map->entry[ idx ];
      bst_insert ( bst, ( const void* ) hash,hashnode_new ( key,value ) );
      ++map->size;
      return;
   }
   result = __hashmap_get_internel ( map,&exists,&bst,key,idx,hash );
   /*
   * if exists.
   */
   switch ( result ) {
      case HASHGET_FOUND:
         debugLeave
         return;
         break;
      case HASHGET_NOT_IN_HASHLIST:
         __traceMSG ( "enter case HASHGET_NOT_IN_HASHLIST" );
         exists->next = hashnode_new ( key,value );
         exists->next->prev = exists;
         __traceMSG ( "leave case HASHGET_NOT_IN_HASHLIST" );
         break;
      case HASHGET_NOT_IN_BINNODE:
         __traceMSG ( "enter case HASHGET_NOT_IN_BINNODE" );
         bst_insert ( bst, ( const void* ) hash,hashnode_new ( key,value ) );
         __traceMSG ( "leave case HASHGET_NOT_IN_BINNODE" );

         break;
   }
   ++map->size;
   debugLeave
}

void* hashmap_get ( const struct HashMap* map,const void* key ) {
   HashNode* ret = 0;
   BST* bst;
   int idx = 0;
   int result = 0;
   int hash = 0;
   debugEnter
   hash =  map->hashfnc ( key ) ;
   idx = hash % get_hash_entry_size();
   result = __hashmap_get_internel ( map,&ret,&bst,key,idx,hash );
   if ( result != HASHGET_FOUND ) {
      debugLeave
      return 0;
   }
   debugLeave
   return ( void* ) ret->value;
}

int hashmap_size ( const struct HashMap* map ) {
   int ret = 0;
   debugEnter
   ret = map?map->size:0;
   debugLeave
   return ret;
}

int hashmap_empty ( const struct HashMap* map ) {
   int ret = 0;
   debugEnter
   ret = ( hashmap_size ( map ) == 0 );
   debugLeave
   return ret;
}

void* hashmap_remove ( struct HashMap* map,const void* key ) {
   int hash = 0;
   int idx = 0;
   int result;
   const void* value = 0;
   HashNode* node = 0;
   BST* bst = 0;
   debugEnter
   if ( map == 0 ) {
      debugLeave
      return 0;
   }
   if ( hashmap_empty ( map ) ) {
      debugLeave
      return 0;
   }
   hash = (*map->hashfnc) ( key );
   idx = hash % get_hash_entry_size();
   result = __hashmap_get_internel ( map,&node,&bst,key,idx,hash );
   __debug ( fprintf ( stderr,"get result\n" ) );
   if ( result != HASHGET_FOUND ) {
      __debug ( fprintf ( stderr,"result not found\n" ) );
      debugLeave
      return 0;
   }
   value = ( const void* ) node->value;
   __debug ( fprintf ( stderr,"found data(%d)...node=%p\n",result,node ) );
   if ( node->prev == 0 ) {
      __debug ( fprintf ( stderr,"node is head" ) );
      /* is head */
      if ( node->next == 0 ) {
         __debug ( fprintf ( stderr,"node is the only head" ) );
         /* it is the only node in the tree */
         bst_erase ( bst, ( const void* ) hash );
         //free ( node );
      } else {
         __debug ( fprintf ( stderr,"node is not the only head" ) );
         HashNode* todel = node->next;
         node->key = node->next->key;
         node->value = todel->value;
         node->next = todel->next;
         if ( node->next != 0 ) {
            node->next->prev = node;
         }
         free ( todel );
      }
   } else {
      __debug ( fprintf ( stderr,"node is not head/tail" ) );
      node->prev->next = node->next;
      if ( node->next ) {
         node->next->prev = node->prev;
      }
      free ( node );
   }
   --map->size;
   debugLeave
   return ( void* ) value;
}

int hashmap_contains_key ( const struct HashMap* map,const void* key ) {
   int ret = 0;
   debugEnter
   if ( hashmap_empty ( map ) ) {
      debugLeave
      return 0;
   }
   ret = ( hashmap_get ( map,key ) != 0 );
   debugLeave
   return 0;
}

static void hashnode_clear_foreach ( HashNode* n ) {
   HashNode* del = 0;
   if ( n == 0 ) {
      return ;
   }
   while ( n != 0 ) {
      del = n;
      n = n->next;
      free ( del );
   }

}

static void hashmap_clear_list ( struct HashMap* map,int idx,int isfinal ) {
   BST* n = 0;
   n = ( BST* ) map->entry[ idx ];
   if ( n != 0 ) {
      bst_bfs_foreach ( n, ( void ( * ) ( void* ) ) hashnode_clear_foreach );
      if ( isfinal ) {
         bst_delete ( n );
         map->entry[idx]=0;
      } else {
		 if(bst_empty(n)){
			return;
	     }
         bst_clear ( n );
      }
   }
}

static void __hashmap_clear ( struct HashMap* map,int isfinal ) {
   int i = 0;
   int totalEntry = 0;
   debugEnter
   totalEntry = get_hash_entry_size();
   if ( map == 0 ) {
      return ;
   }
   for ( i=0; i<totalEntry; ++i ) {
      hashmap_clear_list ( map,i,isfinal );
   }
   map->size = 0;
   debugLeave
}

void hashmap_clear ( struct HashMap* map ) {
   debugEnter
   __hashmap_clear ( map,0 );
   debugLeave
}


void hashmap_delete ( struct HashMap* map ) {
   debugEnter
   if ( map == 0 ) {
      debugLeave
      return ;
   }
   __hashmap_clear ( map,1 );
   free ( map );
   debugLeave
}

