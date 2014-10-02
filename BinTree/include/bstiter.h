#ifndef BST_ITER_H_
#define BST_ITER_H_
#include "bst.h"
#include "stack.h"
typedef struct BSTIterator{
   BinNode* prev;
   BinNode* n;
   Stack* stk;
   BST* bst;
}BSTIterator;

BSTIterator* bstiter_create(BST* bst,const BinNode* n);
void         bstiter_delete(BSTIterator* iter);
#endif
