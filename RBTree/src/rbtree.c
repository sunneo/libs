#include "rbtree.h"
#include <stdlib.h>
#define BLACK 0
#define DBLBLACK -1
#define RED 1
typedef enum BIAS_STATUS{ 
    NORMAL,
    LLBIAS,
    RRBIAS,
    LRBIAS,
    RLBIAS 
}BIAS_STATUS;
#ifndef uchar
#  define uchar unsigned char
#endif
#ifndef uint
#  define uint unsigned int
#endif
typedef struct SiblingBiasBits{
   union {
     struct{
       uchar pad1            : 5;
       uchar LeftRightSibling: 1;
       uchar LeftRightChild  : 1;
       uchar pad2            : 1;
     }Bits;
     uchar byteValue;
   }uBits;
}SiblingBiasBits;
/*
   xxxxx0x1   left  sibling 
   xxxxx1x1   right sibling 
   xxxxxx01   left  red     
   xxxxxx11   right red     
   so there are combinations as follow
   xxxxx00x  LL 
   xxxxx01x  LR 
   xxxxx10x  RL 
   xxxxx11x  RR 
   00000000  recoloring 
*/
#define SIBLING_BIAS_LL 1
#define SIBLING_BIAS_LR 3
#define SIBLING_BIAS_RL 5
#define SIBLING_BIAS_RR 7
#define SIBLING_BIAS_RECOLORING 0
typedef void (*FunctionPtr)(void*);
typedef struct DupRBNode{
   RBNode* node;
   struct DupRBNode* left,*right;
   short visited,build;
}DupRBNode;
__inline static DupRBNode*      __fastcall dup_node(const RBNode* node);
__inline static void            __fastcall duprbnode_visit(DupRBNode* n,FunctionPtr fn);
__inline static void            __fastcall duprbnode_dupchild(DupRBNode* n);
__inline static RBNode*         __fastcall rbnode_create(RBTree* rbtree,const void* key,const void* data,RBNode* parent);
__inline static void            __fastcall rbnode_create_child(RBTree* rbtree,RBNode* parent,uint idx,const void* key,const void* data);
         static RBNode*         __fastcall rbtree_search_node(const RBTree* rbtree,const void* key);
__inline static RBNode*         __fastcall rbtree_minmax_node(const RBTree* rbtree,int _max);
         static int             __fastcall rbnode_should_change_color(const RBNode* n);
__inline static void            __fastcall rbnode_change_color(const RBNode* root,RBNode* n);
__inline static int             __fastcall rbnode_father_idx(const RBNode* n);
__inline static BIAS_STATUS     __fastcall rbnode_should_rotate(const RBNode* node);
         static void            __fastcall rbnode_left_rotate(RBTree* tree,RBNode* n);
         static void            __fastcall rbnode_right_rotate(RBTree* tree,RBNode* n);
         static void            __fastcall rbtree_insert_rotate_handler(RBTree* rbtree,RBNode* parent);
__inline static int             __fastcall sibling_idx(const RBNode* n);
__inline static RBNode*         __fastcall rbtree_sibling(const RBNode* n);
__inline static RBNode*         __fastcall rbnode_inorder_predecessor(const RBNode* n);
__inline static RBNode*         __fastcall rbnode_inorder_sucessor(const RBNode* n);
__inline static RBNode*         __fastcall rbnode_inorder_next_node(const RBNode* n);
__inline static int             __fastcall rbnode_is_leaf(const RBNode* n);
__inline static SiblingBiasBits __fastcall rbnode_should_rotate_sibling_color(const RBNode* sibling_node,uint sibling_idx);
__inline static void            __fastcall rbnode_recoloring(RBNode* parent,uint idx,uint sib_idx);
         static void            __fastcall rbnode_erase_dblblack_handler(RBTree* rbtree,RBNode* parent,RBNode* p);
__inline static void            __fastcall rbtree_erase_node(RBTree* rbtree,RBNode* p);
         static void            __fastcall rbtree_bfs_dfs_foreach(RBTree* rbtree, FunctionPtr fn,int dfs,int bNode);
__inline static void            __fastcall rbtree_fn_node_or_data(FunctionPtr fn,RBNode* node,int bNode);
__inline static void            __fastcall m__rbnode_print_information(const RBNode* p,const char* varname);
#define  rbnode_print_information(NODE) m__rbnode_print_information(NODE,#NODE)
/** 
 * create new node.
 */
static RBNode* __fastcall 
rbnode_create(RBTree* rbtree,const void* key,const void* data,RBNode* parent){
   RBNode* n = (RBNode*)malloc(sizeof(RBNode));
   n->childNode.child[0] = n->childNode.child[1] = 0;
   n->childNode.color[0] = n->childNode.color[1] = BLACK;
   n->key = (rbtree->keysize >0 ? memcpy(malloc(rbtree->keysize),key,rbtree->keysize) : (void*)key);   
   n->data = (rbtree->elesize > 0 ? memcpy(malloc(rbtree->elesize),data,rbtree->elesize) : (void*)data);
   n->parent = parent;
   return n;
}

/**
 * create child for the node.
 */
static void __fastcall 
rbnode_create_child(RBTree* rbtree,RBNode* parent,uint idx,const void* key,const void* data){
   parent->childNode.child[ idx ] = rbnode_create(rbtree,key,data,parent);
   parent->childNode.color[ idx ] = RED;
}

/**
 * search for node with key.
 */
static RBNode* __fastcall 
rbtree_search_node(const RBTree* rbtree,const void* key){
   RBNode* n;
   int cmp;
   if(rbtree->size == 0) /* no key exist */
      return 0; 
   n = rbtree->root;
   while(n){
      cmp = rbtree->compare(key,n->data);
      if(!cmp) /* found node */
         return n; 
      n = n->childNode.child[ cmp>0 ]; /* right if greater ,left otherwise*/
   }
   return 0;
}

RBTree* RBTCONV
rbtree_create(int elesize,int keysize,int(*cmp)(const void*,const void*)){
   RBTree* t = (RBTree*)malloc(sizeof(RBTree));
   t->size = 0;
   t->root = 0;
   t->elesize = elesize;
   t->keysize = keysize;
   t->compare = cmp;
   return t;
}

uint RBTCONV
rbtree_size(const RBTree* rbtree){   
   return rbtree->size; 
}

int RBTCONV
rbtree_empty(const RBTree* rbtree){    
   return rbtree->size == 0;  
}

void* RBTCONV
rbtree_root(const RBTree* rbtree){
   if(rbtree->size == 0)
       return 0; /* no key exist */
   return rbtree->root->data;
}

__inline static RBNode* __fastcall
rbtree_minmax_node(const RBTree* rbtree,int _max){
   RBNode* n,*prev;
   if(rbtree->size == 0)
      return 0;
   n = rbtree->root;
   while(n){
      prev = n;
      n = n->childNode.child[ _max ];
   }
   return prev;
}

void* RBTCONV
rbtree_min(const RBTree* rbtree){
   RBNode* n = rbtree_minmax_node(rbtree,0);
   if(!n) 
      return 0;
   return n->data;
}

void* RBTCONV
rbtree_max(const RBTree* rbtree){
   RBNode* n = rbtree_minmax_node(rbtree,1);
   if(!n) 
      return 0;
   return n->data;
}

static void __fastcall 
rbnode_left_rotate(RBTree* tree,RBNode* n){
   uint father_idx;
   RBNode* right_left_child;
   if(!n->parent)                                   /* n.parent.child = n.right         */
      tree->root = n->childNode.child[ 1 ];
   else{
      father_idx = !(n->parent->childNode.child[ 0 ] == n); /* 0 if left,1 if right*/
      n->parent->childNode.child[ father_idx ] = n->childNode.child[ 1 ];
   }
   right_left_child = n->childNode.child[ 1 ]->childNode.child[ 0 ];
   n->childNode.child[ 1 ]->parent = n->parent;      /* n.right.parent = n.parent        */
   if(right_left_child) right_left_child->parent = n;                     /* n.right.left.parent = n          */
   n->parent = n->childNode.child[ 1 ];              /* n.parent = n.right               */
   n->childNode.child[ 1 ]->childNode.child[ 0 ] = n;/* n.right.left = n                 */
   n->childNode.child[ 1 ]->childNode.color[ 0 ] ^=1;/* n.right.left.color changed       */
   n->childNode.child[ 1 ] = right_left_child;       /* n.right = original n.right.left  */
   n->childNode.color[ 1 ] ^= 1;                     /* n.right.color change             */
}

void RBTCONV 
rbtree_erase_min(RBTree* rbtree){
   RBNode* n;
   n = rbtree_minmax_node(rbtree,0);
   if(!n) 
      return;
   rbtree_erase_node(rbtree,n);
}

void RBTCONV 
rbtree_erase_max(RBTree* rbtree){
   RBNode* n;
   n = rbtree_minmax_node(rbtree,1);
   if(!n) 
      return;
   rbtree_erase_node(rbtree,n);
}

void RBTCONV rbtree_erase_root(RBTree* rbtree){
   rbtree_erase_node(rbtree,rbtree->root);
}

__inline static int __fastcall 
rbnode_father_idx(const RBNode* n){
   return (n->parent->childNode.child[ 1 ] == n);  /* 0 if left,1 if right*/
}

static void __fastcall 
rbnode_right_rotate(RBTree* tree,RBNode* n){
   RBNode* left_right_child;
   if(!n->parent)                                    /* n.parent.child = n.left         */
      tree->root = n->childNode.child[ 0 ]; 
   else
      n->parent->childNode.child[ rbnode_father_idx(n) ] = n->childNode.child[ 0 ];
   left_right_child = n->childNode.child[ 0 ]->childNode.child[ 1 ];
   n->childNode.child[ 0 ]->parent = n->parent;      /* n.left.parent = n.parent        */
   if(left_right_child) 
      left_right_child->parent = n;                     /* n.left.right.parent = n         */
   n->parent = n->childNode.child[ 0 ];              /* n.parent = n.left               */
   n->childNode.child[ 0 ]->childNode.child[ 1 ] = n;/* n.left.right = n                */
   n->childNode.child[ 0 ]->childNode.color[ 1 ] ^=1;/* n.left.right.color changed      */
   n->childNode.child[ 0 ] = left_right_child;       /* n.left = original n.left.right  */
   n->childNode.color[ 0 ] ^= 1;                     /* n.left.color change             */
}


/**
 * rbnode_should_change_color(private)
 * @brief return 1 if the two child are red
 */
__inline static int __fastcall 
rbnode_should_change_color(const RBNode* n){
   /* left child and right child are red */
   return (n->childNode.color[ 0 ] + n->childNode.color[ 1 ] == 2);
}


/**
 * rbnode_change_color(private)
 * @brief change the child color into black,and change the parent color
 */
__inline static void __fastcall 
rbnode_change_color(const RBNode* root,RBNode* n){
   n->childNode.color[ 0 ] = n->childNode.color[ 1 ] = 0;
   if(n->parent)  
      n->parent->childNode.color[ rbnode_father_idx(n) ] ^= 1;
}

void* RBTCONV
rbtree_search(const RBTree* rbtree,const void* key){
   RBNode* n = rbtree_search_node(rbtree,key);
   return n ? n->data : 0;
}


/**
 * rbnode_should_rotate(private)
 * @brief return the bias of the color status.
 * Red left child with red left child   : LLBIAS ( left left case   )
 * Red left child with red right child  : LRBIAS ( left right case  )
 * Red right child with red left child  : RLBIAS ( right left case  )
 * Red right child with red right child : RRBIAS ( right right case )
 * otherwise                            : NORMAL ( no rotate        )
 */
__inline static BIAS_STATUS __fastcall 
rbnode_should_rotate(const RBNode* node){
   short l = node->childNode.color[ 0 ];
   short r = node->childNode.color[ 1 ];  
   RBNode* left = node->childNode.child[ 0 ];
   RBNode* right = node->childNode.child[ 1 ];
   if(left){
      if(l+left->childNode.color[ 0 ] == 2) 
         return LLBIAS;
      if(l+left->childNode.color[ 1 ] == 2) 
         return LRBIAS;
   }
   else if(right){
      if(r+right->childNode.color[ 0 ] == 2) 
         return RLBIAS;
      if(r+right->childNode.color[ 1 ] == 2) 
         return RRBIAS;
   }
   return NORMAL;
}


static void __fastcall
rbtree_insert_rotate_handler(RBTree* rbtree,RBNode* parent){
   do{ 
      if(rbnode_should_change_color(parent) ) 
         rbnode_change_color(rbtree->root,parent);
      switch( rbnode_should_rotate(parent) ){
         case NORMAL:  /* no rotate need, keep upward */
         break;
         
         case LLBIAS: 
            rbnode_right_rotate(rbtree,parent);  
         break;
         
         case RRBIAS: 
            rbnode_left_rotate(rbtree,parent);   
         break;
         
         case LRBIAS: 
            rbnode_left_rotate(rbtree,parent->childNode.child[ 0 ]);   
            rbnode_right_rotate(rbtree,parent);  
         break;
         
         case RLBIAS: 
            rbnode_right_rotate(rbtree,parent->childNode.child[ 1 ]);
            rbnode_left_rotate(rbtree,parent);  
         break;
      }
      parent = parent->parent;          
   }while( parent != 0 );
}

void RBTCONV
rbtree_insert(RBTree* rbtree,const void* key,const void* value){
   RBNode* prev,*n;
   int cmp;
   if(rbtree->size == 0)  
      rbtree->root = rbnode_create(rbtree,key,value,0);
   else{
      n = rbtree->root;
      do{
         prev = n;
         cmp = rbtree->compare(key,n->key);
         if(!cmp)
            return; /* key has been exist */
         n = n->childNode.child[ cmp>0 ];
      }while(n!=0);
      rbnode_create_child(rbtree,prev,cmp>0,key,value);
      rbtree_insert_rotate_handler(rbtree,prev);
   }
   ++rbtree->size;
}

__inline static int __fastcall 
sibling_idx(const RBNode* n){
   return (n == n->parent->childNode.child[ 0 ]);
}


__inline static RBNode* __fastcall 
rbtree_sibling(const RBNode* n){
   /* n is left child, then bool == 1(right child) else 0(left child) */
   return n->parent->childNode.child[ (n == n->parent->childNode.child[ 0 ]) ];
}

__inline static RBNode* __fastcall 
rbnode_inorder_predecessor(const RBNode* n){
   RBNode* r = (RBNode*)n;
   n = n->childNode.child[ 0 ];
   while( n!=0 ){
      r = (RBNode*)n;
      n = n->childNode.child[ 1 ];
   }
   return r;
}

__inline static RBNode* __fastcall
rbnode_inorder_sucessor(const RBNode* n){
   RBNode* r = (RBNode*)n;
   n = n->childNode.child[ 1 ];
   while(n != 0){
      r = (RBNode*)n;
      n = n->childNode.child[ 0 ];
   }
   return r;
}

__inline static int __fastcall 
rbnode_is_leaf(const RBNode* n){
   return ( !n->childNode.child[ 0 ]  && !n->childNode.child[ 1 ] );
}


__inline static RBNode* __fastcall
rbnode_inorder_next_node(const RBNode* n){
   RBNode* ret;
   if( rbnode_is_leaf(n) ) 
      return (RBNode*)n;
   ret = rbnode_inorder_predecessor(n);
   if(ret == n) 
      return rbnode_inorder_sucessor(n);
   return ret;
}


__inline static SiblingBiasBits __fastcall 
rbnode_should_rotate_sibling_color(const RBNode* sibling_node,uint sibling_idx){
   short l;
   short r;
   SiblingBiasBits ret = {1}; /* pad2 */
   if(sibling_node){
      l = sibling_node->childNode.color[ 0 ];
      r = sibling_node->childNode.color[ 1 ];  
      if(l + r == 0) 
         return (SiblingBiasBits){0};/* recoloring */
      ret.uBits.Bits.LeftRightSibling = sibling_idx;
      ret.uBits.Bits.LeftRightChild = (r == 1);
   }
   else 
      return (SiblingBiasBits){0};/* recoloring */
   return ret;
}

static void __fastcall 
rbnode_recoloring(RBNode* parent,uint idx,uint sib_idx){
  if(parent->childNode.color[ idx ] == DBLBLACK)
     parent->childNode.color[ idx ] = BLACK;
  parent->childNode.color[ sib_idx ] ^= 1; 
  --parent->parent->childNode.color[ rbnode_father_idx(parent) ];
}


static void __fastcall
rbnode_erase_dblblack_handler(RBTree* rbtree,RBNode* parent,RBNode* p){
   RBNode* sibling;
   uint node_idx,sibling_idx;    
   --parent->childNode.color[ rbnode_father_idx(p) ];
   do{
      node_idx = rbnode_father_idx(p);
      if(DBLBLACK != parent->childNode.color[ node_idx ])  
         return;
      parent->childNode.color[ (node_idx) ] = BLACK;
      sibling_idx = node_idx^1;
      if(parent->childNode.color[ sibling_idx ] == RED){/* case 6 : red sibling*/
         if(sibling_idx == 0) 
            rbnode_right_rotate(rbtree,parent);
         else
            rbnode_left_rotate(rbtree,parent);
      }
      sibling = rbtree_sibling(p);
      switch(rbnode_should_rotate_sibling_color(sibling,sibling_idx).uBits.byteValue){
         case SIBLING_BIAS_LL:/* case 1 : left black sibling has left red child */
            sibling->childNode.color[ 0 ] = BLACK;
            rbnode_right_rotate(rbtree,parent);
         break;
         
         case SIBLING_BIAS_RR:/* case 2 : right black sibling has right red child */
            sibling->childNode.color[ 1 ] = BLACK;
            rbnode_left_rotate(rbtree,parent);
         break;
         
         case SIBLING_BIAS_LR:/* case 3 : left black sibling has right red child */
            sibling->childNode.color[ 1 ] = BLACK;
            rbnode_left_rotate(rbtree,sibling);
            rbnode_right_rotate(rbtree,parent);
         break;
         
         case SIBLING_BIAS_RL:/* case 4 : right black sibling has left red child */
            sibling->childNode.color[ 0 ] = BLACK;
            rbnode_right_rotate(rbtree,sibling);
            rbnode_left_rotate(rbtree,parent);
         break;
            
         case SIBLING_BIAS_RECOLORING: /* case 5 : black sibling has black childs */
            rbnode_recoloring(parent,node_idx,sibling_idx);
         break;
      }
      p = parent;
      parent = p->parent;
   }while(parent != 0);
}

__inline static void __fastcall
m__rbnode_print_information(const RBNode* p,const char* varname){
    static const char* colorname[] = { "\"DBLBLACK\"","\"BLACK\"","\"RED\"" };
    printf("____information about [%-11s]___________________\n",varname);
    printf("\"%s\" = %-3d\n",varname,p->data);
    if(p->parent)
       printf(".parent = %d\n",p->parent->data);
    if( rbnode_is_leaf(p) )
       printf("\"%s\" is leaf\n",varname);
    if(p->childNode.child[ 0 ]){
       printf(" .left color = %s\n",colorname[ p->childNode.color[ 0 ]+1 ]);
       printf(" .left = %-3d\n",p->childNode.child[ 0 ]->data);
       printf(" .left.parent = %-3d\n",p->childNode.child[ 0 ]->parent->data);
    }
    if(p->childNode.child[ 1 ]){
       printf("    .right color = %s\n",colorname[ p->childNode.color[ 0 ]+1 ]);
       printf("    .right = %-3d\n",p->childNode.child[ 1 ]->data);
       printf("    .right.parent = %-3d\n",p->childNode.child[ 1 ]->parent->data);
    }
    printf("______________________________________________________\n");
}


__inline static void __fastcall 
rbtree_erase_node(RBTree* rbtree,RBNode* p){
   RBNode* next = rbnode_inorder_next_node(p);
   p->data = next->data; /* replace node */
   if(next->parent){  
      rbnode_erase_dblblack_handler(rbtree,next->parent,next);
      next->parent->childNode.child[ rbnode_father_idx(next) ] = 0;
   }
   --rbtree->size;
   free(next);
}

void RBTCONV
rbtree_erase(RBTree* rbtree,const void* key){
   RBNode* p;
   if(rbtree->size == 0) 
      return;
   p = rbtree_search_node(rbtree,key);
   if(!p) 
      return;/* not found */
   rbtree_erase_node(rbtree,p);
}


__inline static void __fastcall
rbtree_fn_node_or_data(FunctionPtr fn,RBNode* node,int bNode){
    if(!node) return;
    fn(bNode?(void*)node:node->data);
}


static void __fastcall
rbtree_bfs_foreach_helper(RBTree* tree,FunctionPtr fn,int bNode){
   RBNode** n;
   int back,front;
   back = front = 0;
   n = (RBNode**)malloc(sizeof(RBNode*) * tree->size);
   n[ back++ ] = tree->root;
   while(front != back){
      if(n[ front ]->childNode.child[ 0 ]) {
         n[ back++ ] = n[ front ]->childNode.child[ 0 ];
      }
      if(n[ front ]->childNode.child[ 1 ]) {
         n[ back++ ] = n[ front ]->childNode.child[ 1 ];
      }
      rbtree_fn_node_or_data(fn,n[ front++ ],bNode);
   }
   free(n);
}

static void __fastcall
rbtree_dfs_foreach_helper(RBTree* tree,FunctionPtr fn,int bNode){
   DupRBNode** n;
   DupRBNode* cur;
   int back = 0;
   if(tree->size == 0) return;
   n = (DupRBNode**)malloc(sizeof(DupRBNode*) * tree->size);
   n[ back++ ] = dup_node(tree->root);
   while(back){
      cur = n[ --back ];
      if(!cur->visited){
         duprbnode_visit(cur,fn);
         duprbnode_dupchild(cur);
         if(cur->right) {
            n[ back++ ] = cur->right;
         }
         if(cur->left) {
            n[ back++ ] = cur->left;
         }
      }
      else free( cur );
   }
   free(n);
}

static void __fastcall
rbtree_bfs_dfs_foreach(RBTree* rbtree, FunctionPtr fn,int dfs,int bNode){
   if(dfs)
      rbtree_dfs_foreach_helper(rbtree,fn,bNode);
   else
      rbtree_bfs_foreach_helper(rbtree,fn,bNode);
}


void RBTCONV 
rbtree_bfs_foreach(RBTree* rbtree, void(*fn )(void*) ){
   rbtree_bfs_dfs_foreach(rbtree,fn,0,0);
}

void RBTCONV 
rbtree_dfs_foreach(RBTree* rbtree, void(*fn)(void*) ){
   rbtree_bfs_dfs_foreach(rbtree,fn,1,0);
}

void RBTCONV
rbtree_clear(RBTree* rbtree){ /* bfs delete node */
   rbtree_bfs_dfs_foreach(rbtree,free,0,1);
   rbtree->size = 0;
}

void RBTCONV
rbtree_delete(RBTree* rbtree){
   rbtree_clear(rbtree);
   free(rbtree);
}

void RBTCONV
rbtree_preorder(RBTree* tree,FunctionPtr fn){
   rbtree_dfs_foreach(tree,fn);
}


__inline static DupRBNode* __fastcall
dup_node(const RBNode* node){
   DupRBNode* d = (DupRBNode*)malloc(sizeof(DupRBNode));
   d->node = (RBNode*)node;
   d->visited = 0;
   d->build = 0;
   d->left = d->right = 0;
   return d;
}

__inline static void __fastcall
duprbnode_visit(DupRBNode* n,FunctionPtr fn){
   if(!n->visited){
      fn(n->node->data);
      n->visited = 1;
   }
}

__inline static void __fastcall
duprbnode_dupchild(DupRBNode* n){
   if(n->node->childNode.child[ 0 ])
      n->left = dup_node(n->node->childNode.child[ 0 ]);
   if(n->node->childNode.child[ 1 ])
      n->right = dup_node(n->node->childNode.child[ 1 ]);
   n->build = 1;
}

void RBTCONV
rbtree_inorder(RBTree* tree,FunctionPtr fn){
   RBNode** s;
   uint back = 0;
   RBNode* cur = 0;
   if(tree->size == 0) return;
   cur = tree->root;
   s = (RBNode**)malloc(sizeof(RBNode*) * tree->size);
   while(1){
      while(cur){
         s[ back++ ] = cur;
         cur = cur->childNode.child[ 0 ];
      }
      if(back){
         cur = s[ --back ];
         fn(cur->data);
         cur = cur->childNode.child[ 1 ];
      }
      else break;
   }
   free(s);
}
   

void RBTCONV
rbtree_postorder(RBTree* tree,FunctionPtr fn){
   DupRBNode** s;
   uint back = 0;
   DupRBNode* cur = 0;
   int leftvisited,rightvisited;
   if(tree->size == 0) return;
   s = (DupRBNode**)malloc(sizeof(DupRBNode*) * tree->size);
   s[ back++ ] = dup_node(tree->root);
   while(back){
      cur = s[ back-1 ];
      duprbnode_dupchild(cur);
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
         if(!cur->visited) duprbnode_visit(cur,fn);
         --back;
      }
   }
   free(s);
}

