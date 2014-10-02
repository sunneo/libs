#include "bst.h"
#include <stdlib.h>
#include <stdio.h>
#include "bstiter.h"
#include <string.h>

#ifdef DEBUG
#  define __traceFunction(X) fprintf(stderr,"%s:%s[line:%-5d]:%s\n",__FILE__,__FUNCTION__,__LINE__,(X))
#  define __debug(X) X
#else
#  define __traceFunction(X)
#  define __debug(X)
#endif

typedef int ( *CompareFunctionPtr ) ( const void*,const void* );
typedef void ( *FunctionPtr ) ( void* );
#define CASE_LEAF 0
#define CASE_NO_PREDECESSOR 1
#define CASE_NO_RIGHT_CHILD 2
#define CASE_HAVE_LEFT_CHILD 3

typedef struct DupBinNode {
   BinNode* node;
   struct DupBinNode* left,*right;
   short visited,build;
}DupBinNode;

 static DupBinNode*   dup_node ( const BinNode* node );
 static void          dupbinnode_visit ( DupBinNode* n,FunctionPtr fn );
 static void          dupbinnode_dupchild ( DupBinNode* n );
 static BinNode*      binnode_create ( const BST* bst,const BinNode* parent,const void* key,const void* data );
 static void          binnode_create_child ( BST* bst,BinNode* parent,unsigned int idx,const void* key,const void* data );
 static BinNode*      bst_search_node ( const BST* tree,const void* key );
 static BinNode*      bst_minmax_node ( const BST* tree,int _max );
 static BinNode*      bst_predecessor ( const BinNode* n );
 static unsigned int    binnode_parent_idx ( const BinNode* n );
 static void          function_ptr_to_node_or_data ( FunctionPtr fn,int bNode,BinNode* node );
static void          bst_bfs_dfs_foreach ( BST* tree,FunctionPtr fn,int dfs,int bNode );
 static void                   binnode_free_data ( void* n );
 static void                   binnode_free_key(void* n);
 static void                   binnode_free_key_data(void* n);
 static void                   binnode_free_ptr ( void* n );


 /*
void ___malloc_s ( void** ret,unsigned int size ) {
   __traceFunction ( "enter" );
   if(ret){
      *ret = calloc (1, size );
   }
   if ( !(*ret) ) {
      __traceFunction ( "bad alloc, program terminated" );
      exit ( -1 );
   }
   __traceFunction ( "leave" );
}
*/
#define malloc_s(P,S) ___malloc_s((void**)(P),(unsigned int)(S))

 static BinNode*
binnode_create ( const BST* bst,const BinNode* parent,const void* key,const void* data ) {
   BinNode* n = 0;
   __traceFunction ( "enter" );
   n = malloc (sizeof ( BinNode ) );
   __debug ( fprintf ( stderr,"n = %p\n",n ) );
   n->child[ 0 ] = 0;
   n->child[ 1 ] = 0;
   n->parent = ( BinNode* ) parent;
   __debug ( fprintf ( stderr,"bst = %p\n",bst ) );
   if ( bst->elesize == 0 ) {
      n->data = ( void* ) data;
   } else {
      n->data = malloc(bst->elesize );
      memcpy ( n->data,data,bst->elesize );
   }
   if ( bst->keysize == 0 ) {
      n->key = ( void* ) key;
   } else {
      n->key = malloc (bst->keysize );
      memcpy ( n->key,data,bst->keysize );
   }
   __traceFunction ( "leave" );
   return n;
}

 static void
binnode_create_child ( BST* bst,BinNode* parent,unsigned int idx,const void* key,const void* data ) {
   __traceFunction ( "enter" );
   parent->child[ idx ] = binnode_create ( bst,parent,key,data );
   __traceFunction ( "leave" );
}

static int
defaultCompare ( const void* a,const void* b ) {
   return - ( ( int ) a- ( int ) b );
}

static void (*BinNodeFreeTable[4])(void*)  = 
{binnode_free_ptr,binnode_free_data,binnode_free_key,binnode_free_key_data};

BST* BINTreeCallConv
bst_create ( int keysize,int elesize, CompareFunctionPtr comp ) {
   BST* t;
   int fnTblIdx = 0;
   __traceFunction ( "enter" );
   t = malloc (sizeof ( BST ) );

   t->size = 0;
   t->root = 0;
   t->compare = (comp?comp:&defaultCompare);
   t->elesize = ( elesize<=0?0:elesize );
   t->keysize = ( keysize<=0?0:keysize );
   fnTblIdx |= ((elesize>0)|((keysize>0)<<1));
   t->node_release_fn = BinNodeFreeTable[fnTblIdx];
   __traceFunction ( "leave" );
   return t;
}

unsigned int BINTreeCallConv
bst_size ( const BST* tree ) {
   return tree->size;
}

int BINTreeCallConv
bst_empty ( const BST* tree ) {
   return ( tree->size == 0 );
}

 static BinNode*
bst_search_node ( const BST* tree,const void* key ) {
   BinNode* n;
   __traceFunction ( "enter" );
   int cmp;
   if ( tree == 0 || tree->size == 0 ) return 0; /* no node exist */
   __traceFunction ( " tree is not null nor empty " );
   n = tree->root;
   while ( n ) {
      __traceFunction ( "before compare" );
      __debug ( fprintf ( stderr,"n = %p, key = %p\n",n,key ) );
      __debug ( fprintf ( stderr,"n->key = %p\n",n->key ) );
      __debug ( fprintf ( stderr,"tree = %p, tree->compare = %p\n",tree,tree->compare ) );
      cmp = (*tree->compare) ( n->key,key );
      __traceFunction ( "after compare" );
      if ( !cmp ) {
         __traceFunction ( "[found] leave" );
         return n;
      }
      __traceFunction ( "[not hit/found] move to next" );
      n = n->child[ (cmp>0) ];
   }
   __traceFunction ( "[not found] leave" );
   return 0;
}

void* BINTreeCallConv
bst_search ( const BST* tree,const void* key ) {
   BinNode* n = 0;
   __traceFunction ( "enter" );
   n = bst_search_node ( tree,key );
   if ( !n ) {
      __traceFunction ( "[not found] leave" );
      return 0;
   }
   __traceFunction ( "[found] leave" );
   return n->data;
}

void* BINTreeCallConv
bst_root ( const BST* tree ) {
   __traceFunction ( "enter" );
   if ( tree->size == 0 ) {
      __traceFunction ( "[empty] leave" );
      return 0;
   }
   __traceFunction ( "[get] leave" );
   return tree->root->data;
}

 static BinNode*
bst_minmax_node ( const BST* tree,int _max ) {
   BinNode* n,*prev;
   __traceFunction ( "enter" );
   if ( tree->size == 0 ) {
      __traceFunction ( "[empty] leave" );
      return 0;
   }
   n = tree->root;
   while ( n ) {
      prev = n;
      n = n->child[ _max ];
   }
   __traceFunction ( "[get] leave" );
   return prev;
}

void* BINTreeCallConv
bst_min ( const BST* tree ) {
   BinNode* n = 0;
   __traceFunction ( "enter" );
   n = bst_minmax_node ( tree,0 );
   if ( !n ) {
      __traceFunction ( "leave" );
      return 0;
   }
   __traceFunction ( "leave" );
   return n->data;
}

void* BINTreeCallConv
bst_max ( const BST* tree ) {
   BinNode* n ;
   __traceFunction ( "enter" );
   n = bst_minmax_node ( tree,1 );
   if ( !n ) {
      __traceFunction ( "leave" );
      return 0;
   }
   __traceFunction ( "leave" );
   return n->data;
}



void BINTreeCallConv
bst_insert ( BST* tree,const void* key,const void* value ) {
   BinNode *prev = 0;
   BinNode *n = 0;
   int cmp = 0;
   __traceFunction ( "enter" );
   if ( tree->size == 0 ) {
      tree->root = binnode_create ( tree,0,key,value );
	  ++tree->size;
   	  __traceFunction ( "leave" );
	  return;
   } else {
      n = tree->root;
      while ( n ) {
         prev = n;
         cmp = (*tree->compare) ( n->key,key );
         if ( !cmp ) {
            __traceFunction ( "[data exists] leave" );
            return ; /* exist */
         }
         n = n->child[ (cmp>0) ];
      }
      binnode_create_child ( tree,prev,(cmp>0),key,value );
   }
   ++tree->size;
   __traceFunction ( "leave" );
}

 static BinNode*
bst_predecessor ( const BinNode* n ) {
   __traceFunction ( "enter" );
   if ( n->child[ 0 ] ) {
      n = n->child[ 0 ];
   }
   while ( n->child[ 1 ] ) {
      n = n->child[ 1 ];
   }
   __traceFunction ( "leave" );
   return ( BinNode* ) n;
}



 static unsigned int
binnode_parent_idx ( const BinNode* n ) {
   return ( n->parent->child[ 1 ] == n );
}

static void binnode_free_key(void* n){
   if(n){
      free ( ( ( BinNode* ) n )->key );
	  free(n);
   }
}

static void binnode_free_key_data(void* n){
   if(n){
      free ( ( ( BinNode* ) n )->key );
      free ( ( ( BinNode* ) n )->data );
      free ( n );
   }
}

 static void
binnode_free_data ( void* n ) {
   if ( n ) {
      free ( ( ( BinNode* ) n )->data );
      free ( n );
   }
}

static void
binnode_free_ptr ( void* n ) {
   if ( n ) {
      free ( n );
   }
}



 static int
binnode_isleaf ( const BinNode* n ) {
   return ( 0 == n->child[ 0 ] && 0 == n->child[ 1 ] );
}

 static unsigned int
binnode_child_size ( const BinNode* n ) {
   return ( 0!= n->child[ 0 ] ) + ( 0!= n->child[ 1 ] );
}

static void
binnode_replace(BinNode* n,BinNode* newnode){
   if(n->parent){
	  n->parent->child[ binnode_parent_idx(n) ] = newnode;
   }
}

 static BinNode*
binnode_one_child ( const BinNode* n ) {
   BinNode* ret = 0;
   if(n->child[ 0 ]){
	  ret = n->child[ 0 ];
	  while(ret->child[ 1 ]){
		 ret = ret->child [ 1 ];
	  }
   }
   else{
	  ret = n->child [ 1 ];
	  while(ret->child[ 0 ]){
		 ret = ret->child [ 0 ];
	  }
   }
   return ret;
}

void BINTreeCallConv
bst_erase ( BST* tree,const void* key ) {
   BinNode* n,*predecessor;
   __traceFunction ( "enter" );
   if ( tree->size == 0 )  {
      __traceFunction ( "[empty] leave" );
      return;
   }
   n = bst_search_node ( tree,key );
   if ( !n )  {
      __traceFunction ( "[not found] leave" );
      return; /* not found */
   }
   switch ( binnode_child_size ( n ) ) {
      case 0:
         __traceFunction ( "[ is leaf ] " );
         if ( n == tree->root ) {
            tree->root = 0;
         } else {
			binnode_replace(n,0);
         }
		 tree->node_release_fn(n);
         break;

      case 1:
         __traceFunction ( "[ 1 child ] " );
         predecessor =  binnode_one_child ( n );
         if ( n == tree->root ) {
            tree->root = predecessor;
            tree->root->parent = 0;
         } else {
            //n->parent->child[ binnode_parent_idx ( n ) ] = predecessor;
			binnode_replace(n,predecessor);
            predecessor->parent = n->parent;
         }
		 tree->node_release_fn(n);
         break;

      case 2:
         __traceFunction ( "[ 2 child ] " );
         predecessor = bst_predecessor ( n );
         if ( tree->elesize != 0 ) {
            free ( n->data );
         }
         n->data = predecessor->data;
         if ( tree->keysize ) {
            free ( n->key );
         }
         n->key = predecessor->key;
         if ( predecessor->parent ) {
			binnode_replace(predecessor,predecessor->child[ 0 ]);
            //predecessor->parent->child[ binnode_parent_idx ( predecessor ) ] = predecessor->child[ 0 ];
         }
         if ( predecessor->child[ 0 ] ) {
            predecessor->child[ 0 ]->parent = predecessor->parent;
         }
         free ( predecessor );
         break;
   }
   --tree->size;
   __traceFunction ( "leave" );
}

void BINTreeCallConv
bst_clear ( BST* tree ) {
   __traceFunction ( "enter" );
   bst_bfs_dfs_foreach(tree,tree->node_release_fn,0,1);
   tree->root = 0;
   tree->size = 0;
   __traceFunction ( "leave" );
}

void BINTreeCallConv
bst_delete ( BST* tree ) {
   __traceFunction ( "enter" );
   bst_clear ( tree );
   free ( tree );
   __traceFunction ( "leave" );
}


 static void
function_ptr_to_node_or_data ( FunctionPtr fn,int bNode,BinNode* node ) {
   if ( !node ) {
      return;
   }
   fn ( bNode? ( void* ) node:node->data );
}

static void
bst_bfs_foreach_helper ( BST* tree,FunctionPtr fn,int bNode ) {
   BinNode** n;
   int back,front;
   __traceFunction ( "enter" );
   back = front = 0;
   n = (BinNode**)malloc (sizeof ( BinNode* ) * tree->size );
   n[ back++ ] = tree->root;
   while ( front != back ) {
      if ( n[ front ]->child[ 0 ] ) {
         n[ back++ ] = n[ front ]->child[ 0 ];
      }
      if ( n[ front ]->child[ 1 ] ) {
         n[ back++ ] = n[ front ]->child[ 1 ];
      }
      function_ptr_to_node_or_data ( fn,bNode,n[ front++ ] );
   }
   free ( n );
   __traceFunction ( "leave" );
}

static void
bst_dfs_foreach_helper ( BST* tree,FunctionPtr fn,int bNode ) {
   DupBinNode** n;
   DupBinNode* cur;
   int back = 0;
   __traceFunction ( "enter" );
   if ( tree->size == 0 ) {
      __traceFunction ( "[empty] leave" );
      return;
   }
   n =(DupBinNode**) malloc ( sizeof ( DupBinNode* ) * tree->size );
   n[ back++ ] = dup_node ( tree->root );
   while ( back ) {
      cur = n[ --back ];
      if ( !cur->visited ) {
         dupbinnode_visit ( cur,fn );
         dupbinnode_dupchild ( cur );
         if ( cur->right ) {
            n[ back++ ] = cur->right;
         }
         if ( cur->left ) {
            n[ back++ ] = cur->left;
         }
      } else {
         free ( cur );
      }
   }
   free ( n );
   __traceFunction ( "leave" );
}

static void
bst_bfs_dfs_foreach ( BST* tree,FunctionPtr fn,int dfs,int bNode ) {
   __traceFunction ( "enter" );
   if ( tree->size == 0 ) {
      __traceFunction ( "[empty] leave" );
      return;
   }
   if ( dfs ) {
      bst_dfs_foreach_helper ( tree,fn,bNode );
   } else {
      bst_bfs_foreach_helper ( tree,fn,bNode );
   }
   __traceFunction ( "leave" );
}

void BINTreeCallConv
bst_bfs_foreach ( BST* tree,FunctionPtr fn ) {
   __traceFunction ( "enter" );
   bst_bfs_dfs_foreach ( tree,fn,0,0 );
   __traceFunction ( "leave" );
}

void BINTreeCallConv
bst_dfs_foreach ( BST* tree,FunctionPtr fn ) {
   __traceFunction ( "enter" );
   bst_bfs_dfs_foreach ( tree,fn,1,0 );
   __traceFunction ( "leave" );
}

void BINTreeCallConv
bst_preorder ( BST* tree,FunctionPtr fn ) {
   __traceFunction ( "enter" );
   bst_dfs_foreach ( tree,fn );
   __traceFunction ( "leave" );
}

 static DupBinNode*
dup_node ( const BinNode* node ) {
   DupBinNode* d = 0;
   __traceFunction ( "enter" );
   d = malloc ( sizeof ( DupBinNode ) );
   d->node = ( BinNode* ) node;
   d->visited = 0;
   d->build = 0;
   d->left = d->right = 0;
   __traceFunction ( "leave" );
   return d;
}

 static void
dupbinnode_visit ( DupBinNode* n,FunctionPtr fn ) {
   if ( !n->visited ) {
      if ( n->node ) {
         fn ( n->node->data );
      }
      n->visited = 1;
   }
}

 static void
dupbinnode_dupchild ( DupBinNode* n ) {
   __traceFunction ( "enter" );
   if ( n->build ) {
      __traceFunction ( "[has been built] leave" );
      return;
   }
   if ( n->node->child[ 0 ] ) {
      n->left = dup_node ( n->node->child[ 0 ] );
   }
   if ( n->node->child[ 1 ] ) {
      n->right = dup_node ( n->node->child[ 1 ] );
   }
   n->build = 1;
   __traceFunction ( "leave" );
}

void BINTreeCallConv
bst_inorder ( BST* tree,FunctionPtr fn ) {
   BinNode** s;
   unsigned int back = 0;
   BinNode* cur = 0;
   __traceFunction ( "enter" );
   if ( tree->size == 0 ) {
      __traceFunction ( "[empty] leave" );
      return;
   }
   s = (BinNode**)malloc (sizeof ( BinNode* ) * tree->size );
   cur = tree->root;
   while ( 1 ) {
      while ( cur ) {
         s[ back++ ] = cur;
         cur = cur->child[ 0 ];
      }
      if ( back ) {
         cur = s[ --back ];
         fn ( cur->data );
         cur = cur->child[ 1 ];
      } else {
         break;
      }
   }
   free ( s );
   __traceFunction ( "leave" );
}


void BINTreeCallConv
bst_postorder ( BST* tree,FunctionPtr fn ) {
   DupBinNode** s;
   unsigned int back = 0;
   DupBinNode* cur = 0;
   int leftvisited,rightvisited;
   __traceFunction ( "enter" );
   if ( tree->size == 0 ) {
      __traceFunction ( "[empty] leave" );
      return;
   }
   s = (DupBinNode**)malloc (sizeof ( DupBinNode* ) * tree->size );
   s[ back++ ] = dup_node ( tree->root );
   while ( back ) {
      cur = s[ back-1 ];
      dupbinnode_dupchild ( cur );
      if ( cur->left ) {
         if ( !cur->left->visited ) {
            s[ back++ ] = cur->left;
         } else {
            free ( cur->left );
            cur->left = 0;
         }
      } else if ( cur->right ) {
         if ( !cur->right->visited ) {
            s[ back++ ] = cur->right;
         } else {
            free ( cur->right );
            cur->right = 0;
         }
      } else {
         dupbinnode_visit ( cur,fn );
         --back;
      }
   }
   free ( s );
   __traceFunction ( "leave" );
}
