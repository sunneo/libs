#include <stdio.h>
#include <stdlib.h>
#include "bst.h"

int icmp ( const void* a,const void* b ) {
   return  -( ( int ) a - ( int ) b );
}

void* write_int ( int* i,int d ) {
   *i = d;
   return ( void* ) i;
}

void printkey ( void* key ) {
   static int i = 0;
   printf ( "[%d] address = %-8x ",i++,key );
   if ( key ) printf ( "data = %d\n",* ( int* ) key );
}

int main ( int argc, char *argv[] ) {
   int a[ 100 ] = {};
   int i;
   printf ( "a is at %x\n",a );
   BST* bst = bst_create ( 0,0,icmp );
   BinNode* n;
   BSTIter iter;

   for ( i=0; i<100; ++i ) {
      write_int ( &a[ i ],rand() &0x7fff );
      bst_insert ( bst,a[ i ],&a[ i ] );
   }
   /*for(i=0; i<100; ++i) {
      bst_erase(bst,&a[ i ]);
   }
   printf("size = %d\n",bst_size(bst));
   system("pause");*/
   //bst_inorder(bst,printkey);
   iter = bst_get_iter ( bst );
   while ( bst_iter_hasNext ( iter ) ) {
      /*iter = bst_next_iter(iter);*/
      printkey ( bst_iter_next ( iter ) );
   }
   //bst_iter_delete(iter);
   bst_delete ( bst );
   return 0;
}
