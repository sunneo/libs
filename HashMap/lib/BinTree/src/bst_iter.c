#include "bst.h"
#include "bstiter.h"
#include <stdlib.h>
#include <stdio.h>
#ifdef DEBUG
#  define __traceFunction(X) fprintf(stderr,"%s:%s[line%-5d]:%s\n",__FILE__,__FUNCTION__,__LINE__,(X))
#  define __debug(X) X
#else
#  define __traceFunction(X)
#  define __debug(X)
#endif

static void* malloc_s ( unsigned int s ) {
   void* ret = malloc ( s );
   if ( !ret ) {
      __traceFunction ( "[bad alloc] program terminate" );
      exit ( -1 );
   }
   return ret;
}

BSTIterator*
bstiter_create ( BST* bst,const BinNode* n ) {
   BSTIterator* iter = 0;
   __traceFunction ( "enter" );
   iter = ( BSTIterator* ) malloc_s ( sizeof ( BSTIterator ) );
   iter->n = ( BinNode* ) n;
   iter->stk = stack_create ( sizeof ( BinNode* ) );
   iter->bst = bst;
   __traceFunction ( "leave" );
   return iter;
}

void bstiter_delete ( BSTIterator* iter ) {
   __traceFunction ( "enter" );
   if ( !iter ) {
      __traceFunction ( "[null pointer] leave" );
      return;
   }
   stack_delete ( iter->stk );
   free ( iter );
   __traceFunction ( "leave" );
}

BSTIter BINTreeCallConv
bst_get_iter ( BST* tree ) {
   return bstiter_create ( tree,tree->root );
}

BSTIter BINTreeCallConv bst_next_iter ( BSTIter iter ) {
   BSTIterator* iterator = iter;
   __traceFunction ( "enter" );
   if ( !iter ) {
      __traceFunction ( "[null pointer] leave" );
      return 0;
   }
   while ( iterator->n ) {
      stack_push ( iterator->stk,&iterator->n );
      iterator->n = iterator->n->child[ 0 ];
   }
   if ( !stack_empty ( iterator->stk ) ) {
      iterator->n = * ( BinNode** ) stack_top ( iterator->stk );
      stack_pop ( iterator->stk );
      iterator->prev = iterator->n;
      iterator->n = iterator->n->child[ 1 ];
   } else {
      iterator->n =   0;
   }
   __traceFunction ( "leave" );
   return iterator;
}

void *BINTreeCallConv
bst_iter_data ( const BSTIter iter ) {
   void* ret;
   __traceFunction ( "enter" );
   if ( !iter ) {
      __traceFunction ( "[null pointer] leave" );
      return 0;
   }
   ret = ( ( BSTIterator* ) iter )->prev != 0? ( ( BSTIterator* ) iter )->prev->data : 0;
   __traceFunction ( "leave" );
   return ret;
}

int BINTreeCallConv
bst_iter_isbegin ( const BSTIter iter ) {
   return ( ( BSTIterator* ) iter )->n == ( ( BSTIterator* ) iter )->bst->root;
}


int BINTreeCallConv
bst_iter_isend ( const BSTIter iter ) {
   return iter == 0 || ( ( ( BSTIterator* ) iter )->n == 0 && stack_empty ( ( ( BSTIterator* ) iter )->stk ) );
}

void BINTreeCallConv
bst_iter_rewind ( BSTIter iter ) {
   __traceFunction ( "enter" );
   stack_clear ( ( ( BSTIterator* ) iter )->stk );
   ( ( BSTIterator* ) iter )->n = ( ( BSTIterator* ) iter )->bst->root;
   __traceFunction ( "leave" );
}

void* BINTreeCallConv
bst_iter_next ( BSTIter iter ) {
   return bst_iter_data ( bst_next_iter ( iter ) );
}

int BINTreeCallConv
bst_iter_hasNext ( const BSTIter iter ) {
   return !bst_iter_isend ( iter );
}

BSTIter BINTreeCallConv
bst_iter_delete ( BSTIter iter ) {
   __traceFunction ( "enter" );
   bstiter_delete ( iter );
   __traceFunction ( "leave" );
}
