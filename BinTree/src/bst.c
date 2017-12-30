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

typedef int (*CompareFunctionPtr)(const void*,const void*);
typedef void (*FunctionPtr)(void*,void*);
#define CASE_LEAF 0
#define CASE_NO_PREDECESSOR 1
#define CASE_NO_RIGHT_CHILD 2
#define CASE_HAVE_LEFT_CHILD 3

typedef struct DupBinNode{
   BinNode* node;
   struct DupBinNode* left,*right;
   short visited,build;
}DupBinNode;

__inline static DupBinNode* __fastcall dup_node(const BinNode* node);
__inline static void        __fastcall dupbinnode_visit(DupBinNode* n,FunctionPtr fn,void* closure);
__inline static void        __fastcall dupbinnode_dupchild(DupBinNode* n);
__inline static BinNode*    __fastcall binnode_create(const BST* bst,const BinNode* parent,const void* key,const void* data);
__inline static void        __fastcall binnode_create_child(BST* bst,BinNode* parent,unsigned int idx,const void* key,const void* data);
__inline static BinNode*    __fastcall bst_search_node(const BST* tree,const void* key);
__inline static BinNode*    __fastcall bst_minmax_node(const BST* tree,int _max);
__inline static BinNode*    __fastcall bst_predecessor(const BinNode* n);
__inline static unsigned int  __fastcall binnode_parent_idx(const BinNode* n);
__inline static void        __fastcall function_ptr_to_node_or_data(FunctionPtr fn,int bNode,BinNode* node,void* closure);
         static void        __fastcall bst_bfs_dfs_foreach(BST* tree,FunctionPtr fn,int dfs,int bNode,void* closure);
__inline static void                   binnode_free(void* n);
__inline static BinNode* __fastcall    bst_minmax_node_from_node(const BinNode* nodeRoot,int _max);
static void* malloc_s(unsigned int size){
   void* ret = malloc(size);
   if(!ret){
	  __traceFunction("bad alloc, program terminated");
	  exit(-1);
   }
   return ret;
}

__inline static BinNode* __fastcall 
binnode_create(const BST* bst,const BinNode* parent,const void* key,const void* data){
   BinNode* n;
   __traceFunction("enter");
   n = (BinNode*)malloc_s(sizeof(BinNode));
   n->child[ 0 ] = n->child[ 1 ] = 0;
   n->parent = (BinNode*)parent;
   if(bst->elesize == 0){
      n->data = (void*)data;
   }
   else{
      n->data = malloc_s(bst->elesize);
	  memcpy(n->data,data,bst->elesize);
   }
   if(bst->keysize == 0){
      n->key = (void*)key;
   }
   else{
      n->key = memcpy(malloc_s(bst->keysize),data,bst->keysize);
   }
   __traceFunction("leave");
   return n;
}

__inline static void __fastcall
binnode_create_child(BST* bst,BinNode* parent,unsigned int idx,const void* key,const void* data){
   __traceFunction("enter");
   parent->child[ idx ] = binnode_create(bst,parent,key,data);
   __traceFunction("leave");
}

static int 
defaultCompare(const void* a,const void* b){
   return -(a-b);
}

BST* BINTreeCallConv
bst_create(int keysize,int elesize, CompareFunctionPtr comp ){
   BST* t;
   __traceFunction("enter");
   t = (BST*)malloc_s(sizeof(BST));

   t->size = 0;
   t->root = 0;
   t->compare = comp?comp:defaultCompare;
   t->elesize = elesize<=0?0:elesize;
   t->keysize = keysize<=0?0:keysize;
   __traceFunction("leave");
   return t;
}

unsigned int BINTreeCallConv
bst_size(const BST* tree){
   return tree->size;
}

int BINTreeCallConv
bst_empty(const BST* tree){
   return (tree->size == 0);
}

__inline static BinNode* __fastcall
bst_search_node(const BST* tree,const void* key){
   BinNode* n;
   __traceFunction("enter");
   int cmp;
   if(tree->size == 0) return 0; /* no node exist */
   n = tree->root;
   while(n){
      cmp = tree->compare(n->key,key);
      if(!cmp) {
		 __traceFunction("[found] leave");
		 return n;
	  }
      n = n->child[ cmp>0 ];
   }
   __traceFunction("[not found] leave");
   return 0;
}

void* BINTreeCallConv
bst_search(const BST* tree,const void* key){
   BinNode* n = 0;
   __traceFunction("enter");
   n = bst_search_node(tree,key);
   if(!n) {
	  __traceFunction("[not found] leave");
	  return 0;
   }
   __traceFunction("[found] leave");
   return n->data;
}

void* BINTreeCallConv
bst_root(const BST* tree){
   __traceFunction("enter");
   if(tree->size == 0) {
	  __traceFunction("[empty] leave");
	  return 0;
   }
   __traceFunction("[get] leave");
   return tree->root->data;
}

__inline static BinNode* __fastcall
bst_minmax_node(const BST* tree,int _max){
   BinNode* n,*prev;
   __traceFunction("enter");
   if(tree->size == 0) {
	  __traceFunction("[empty] leave");
	  return 0;
   }
   n = tree->root;
   while(n){
      prev = n;
      n = n->child[ _max ];
   }
   __traceFunction("[get] leave");
   return prev;
}

__inline static BinNode* __fastcall
bst_minmax_node_from_node(const BinNode* nodeRoot,int _max){
   BinNode* n,*prev;
   __traceFunction("enter");
   if(tree->size == 0) {
	  __traceFunction("[empty] leave");
	  return 0;
   }
   n = nodeRoot;
   while(n){
      prev = n;
      n = n->child[ _max ];
   }
   __traceFunction("[get] leave");
   return prev;
}


void* BINTreeCallConv
bst_min(const BST* tree){
   BinNode* n = 0;
   __traceFunction("enter");
   n = bst_minmax_node(tree,0);
   if(!n) {
	  __traceFunction("leave");
	  return 0;
   }
   __traceFunction("leave");
   return n->data;
}

void* BINTreeCallConv
bst_max(const BST* tree){
   BinNode* n ;
   __traceFunction("enter");
   n = bst_minmax_node(tree,1);
   if(!n) {
	  __traceFunction("leave");
	  return 0; 
   }
   __traceFunction("leave");
   return n->data;
}



void BINTreeCallConv
bst_insert(BST* tree,const void* key,const void* value){
   BinNode* prev,*n;
   __traceFunction("enter");
   int cmp;
   if(tree->size == 0) {
	  tree->root = binnode_create(tree,0,key,value );
   }
   else {
      n = tree->root;
      while(n){
         prev = n;
         cmp = tree->compare(n->key,key);
         if(!cmp) {
			__traceFunction("[data exists] leave");
			return ; /* exist */
		 }
         n = n->child[ cmp>0 ];
      }
      binnode_create_child(tree,prev,cmp>0,key,value);
   }
   ++tree->size;
   __traceFunction("leave");
}

__inline static BinNode* __fastcall
bst_predecessor(const BinNode* n){
   __traceFunction("enter");
   if(n->child[ 0 ]) {
	  n = n->child[ 0 ];
   }
   while(n->child[ 1 ]) {
	  n = n->child[ 1 ];
   }
   __traceFunction("leave");
   return (BinNode*)n;
}

__inline static unsigned int __fastcall 
binnode_parent_idx(const BinNode* n){
   return (n->parent->child[ 1 ] == n);
}

__inline static void
binnode_free(void* n){
   if(n){
      free(((BinNode*)n)->data);
      free(n);
   }
}

__inline static int 
binnode_isleaf(const BinNode* n){
  return (0 == n->child[ 0 ] && 0 == n->child[ 1 ]);
}

__inline static unsigned int __fastcall
binnode_child_size(const BinNode* n){
   return (0!= n->child[ 0 ]) + (0!= n->child[ 1 ]);
}

__inline static BinNode* __fastcall
binnode_one_child(const BinNode* n){
   return n->child[ n->child[ 0 ] == 0 ];
}

void BINTreeCallConv
bst_erase(BST* tree,const void* key){
   BinNode* n,*predecessor;
   __traceFunction("enter");
   if(tree->size == 0)  {
	  __traceFunction("[empty] leave");
	  return;
   }
   n = bst_search_node(tree,key);
   if(!n)  {
	  __traceFunction("[not found] leave");
	  return; /* not found */
   }
   switch(binnode_child_size(n)){
     case 0:
        if(n == tree->root){
           tree->root = 0;
        }
        else{
           n->parent->child[ binnode_parent_idx(n) ] = 0;
        }
        binnode_free(n);
     break;
     
     case 1:
        predecessor =  binnode_one_child(n);
        if( n == tree->root ){
           tree->root = predecessor;
           tree->root->parent = 0;
        }
        else{
           n->parent->child[ binnode_parent_idx(n) ] = predecessor;
           predecessor->parent = n->parent;
        }
        binnode_free(n);
     break;
     
     case 2:
        predecessor = bst_predecessor(n);
        free(n->data);
        n->data = predecessor->data;
        predecessor->parent->child[ binnode_parent_idx(predecessor) ] = predecessor->child[ 0 ];
        if(predecessor->child[ 0 ]) {
		   predecessor->child[ 0 ]->parent = predecessor->parent;
		}
        free(predecessor);
     break;
   } 
   --tree->size;
   __traceFunction("leave");
}

typedef struct TreeNodeCollector{
	BinNode** node;
	unsigned int size;
	int idx;
}TreeNodeCollector;

static void tree_node_collector_functor(BinNode* node, void* closure){
	TreeNodeCollector* collector = (TreeNodeCollector*)closure;
	collector->node[collector->idx]=node;
	++collector->idx;
}

static TreeNodeCollector* create_collector_from_bst(BST* tree){
	TreeNodeCollector* ret = (TreeNodeCollector*)malloc(sizeof(TreeNodeCollector));
	ret->node = (BinNode**)malloc(sizeof(BinNode*)*tree->size);
	ret->size=tree->size;
	ret->idx = 0;
	return ret;
}




void BINTreeCallConv
bst_clear(BST* tree){
   __traceFunction("enter");
   
   if(tree->elesize){
      bst_bfs_dfs_foreach(tree,binnode_free,0,1,NULL);  
   }
   tree->size = 0;
   __traceFunction("leave");
}

void BINTreeCallConv
bst_delete(BST* tree){
   __traceFunction("enter");
   bst_clear(tree);
   free(tree);
   __traceFunction("leave");
}


__inline static void __fastcall 
function_ptr_to_node_or_data(FunctionPtr fn,int bNode,BinNode* node,void* closure){
   if(!node) {
	  return;
   }
   fn(bNode?(void*)node:node->data,closure);
}

static void __fastcall
bst_bfs_foreach_helper(BST* tree,FunctionPtr fn,int bNode,void* closure){
   BinNode** n;
   int back,front;
   __traceFunction("enter");
   back = front = 0;
   n = (BinNode**)malloc_s(sizeof(BinNode*) * tree->size);
   n[ back++ ] = tree->root;
   while(front != back){
      if(n[ front ]->child[ 0 ]) {
         n[ back++ ] = n[ front ]->child[ 0 ];
      }
      if(n[ front ]->child[ 1 ]) {
         n[ back++ ] = n[ front ]->child[ 1 ];
      }
      function_ptr_to_node_or_data(fn,bNode,n[ front++ ],closure);
   }
   free(n);
   __traceFunction("leave");
}

static void __fastcall
bst_dfs_foreach_helper(BST* tree,FunctionPtr fn,int bNode,void* closure){
   DupBinNode** n;
   DupBinNode* cur;
   int back = 0;
   __traceFunction("enter");
   if(tree->size == 0) {
	  __traceFunction("[empty] leave");
	  return;
   }
   n = (DupBinNode**)malloc_s(sizeof(DupBinNode*) * tree->size);
   n[ back++ ] = dup_node(tree->root);
   while(back){
      cur = n[ --back ];
      if(!cur->visited){
         dupbinnode_visit(cur,fn,closure);
         dupbinnode_dupchild(cur);
         if(cur->right) {
            n[ back++ ] = cur->right;
         }
         if(cur->left) {
            n[ back++ ] = cur->left;
         }
      }
      else {
		 free( cur );
	  }
   }
   free(n);
   __traceFunction("leave");
}

static void __fastcall
bst_bfs_dfs_foreach(BST* tree,FunctionPtr fn,int dfs,int bNode,void* closure){
   __traceFunction("enter");
   if(tree->size == 0) {
	  __traceFunction("[empty] leave");
	  return;
   }
   if(dfs){
      bst_dfs_foreach_helper(tree,fn,bNode,closure);
   }
   else{
      bst_bfs_foreach_helper(tree,fn,bNode,closure);
   }
   __traceFunction("leave");
}

void BINTreeCallConv 
bst_bfs_foreach(BST* tree,FunctionPtr fn,void* closure){
   __traceFunction("enter");
   bst_bfs_dfs_foreach(tree,fn,0,0,closure);
   __traceFunction("leave");
}

void BINTreeCallConv 
bst_dfs_foreach(BST* tree,FunctionPtr fn,void* closure){
   __traceFunction("enter");
   bst_bfs_dfs_foreach(tree,fn,1,0,closure);
   __traceFunction("leave");
}

void BINTreeCallConv 
bst_preorder(BST* tree,FunctionPtr fn,void* closure){
   __traceFunction("enter");
   bst_dfs_foreach(tree,fn,closure);
   __traceFunction("leave");
}

__inline static DupBinNode* __fastcall
dup_node(const BinNode* node){
   DupBinNode* d = 0;
   __traceFunction("enter");
   d = (DupBinNode*)malloc_s(sizeof(DupBinNode));
   d->node = (BinNode*)node;
   d->visited = 0;
   d->build = 0;
   d->left = d->right = 0;
   __traceFunction("leave");
   return d;
}

__inline static void __fastcall
dupbinnode_visit(DupBinNode* n,FunctionPtr fn,void* closure){
   if(!n->visited){
      if(n->node) {
		 fn(n->node->data,closure);
	  }
      n->visited = 1;
   }
}

__inline static void __fastcall
dupbinnode_dupchild(DupBinNode* n){
   __traceFunction("enter");
   if(n->build) {
	  __traceFunction("[has been built] leave");
	  return;
   }
   if(n->node->child[ 0 ]) {
	  n->left = dup_node(n->node->child[ 0 ]);
   }
   if(n->node->child[ 1 ]) {
	  n->right = dup_node(n->node->child[ 1 ]);
   }
   n->build = 1;
   __traceFunction("leave");
}

void BINTreeCallConv 
bst_inorder(BST* tree,FunctionPtr fn,void* closure){
   BinNode** s;
   unsigned int back = 0;
   BinNode* cur = 0;
   __traceFunction("enter");
   if(tree->size == 0) {
	  __traceFunction("[empty] leave");
	  return;
   }
   s = (BinNode**)malloc_s(sizeof(BinNode*) * tree->size);
   cur = tree->root;
   while(1){
      while(cur){
         s[ back++ ] = cur;
         cur = cur->child[ 0 ];
      }
      if(back) {
         cur = s[ --back ];
         fn(cur->data,closure);
         cur = cur->child[ 1 ];
      }
      else {
		 break;
	  }
   }
   free(s);
   __traceFunction("leave");
}


void BINTreeCallConv 
bst_postorder(BST* tree,FunctionPtr fn,void* closure){
   DupBinNode** s;
   unsigned int back = 0;
   DupBinNode* cur = 0;
   int leftvisited,rightvisited;
   __traceFunction("enter");
   if(tree->size == 0) {
	  __traceFunction("[empty] leave");
	  return;
   }
   s = (DupBinNode**)malloc_s(sizeof(DupBinNode*) * tree->size);
   s[ back++ ] = dup_node(tree->root);
   while(back){
      cur = s[ back-1 ];
      dupbinnode_dupchild(cur);
      if(cur->left){
         if(!cur->left->visited){
            s[ back++ ] = cur->left;
         }
         else{
            free(cur->left);
            cur->left = 0;
         }
      }
      else if(cur->right){ 
         if(!cur->right->visited){
            s[ back++ ] = cur->right;
         }
         else{
            free(cur->right);
            cur->right = 0;
         }
      }
      else {
         dupbinnode_visit(cur,fn,closure);
         --back;
      }
   }
   free(s);
   __traceFunction("leave");
}
