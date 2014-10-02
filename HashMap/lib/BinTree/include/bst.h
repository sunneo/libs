#ifndef BINTREE_H_
#define BINTREE_H_

#  ifdef DLLLIB
#    ifdef BUILD_DLL
#      define DLLIMPORT __declspec(dllexport)
#    else
#      define DLLIMPORT __declspec(dllimport)
#    endif
#  else
#    define DLLIMPORT
#  endif

typedef void* BSTIter;

typedef struct DLLIMPORT BinNode {
   struct BinNode* child[2];
   struct BinNode* parent;
   void *key,*data;
}BinNode;

typedef struct DLLIMPORT BST {
   BinNode* root;
   unsigned int size;
   int elesize,keysize;
   int ( *compare ) ( const void*,const void* );
   void (*node_release_fn)(void*);
}BST;
#   ifdef __cplusplus
extern "C" {
#   endif

#   ifndef BINTreeCallConv
#      define BINTreeCallConv 
#   endif

   /**
    * keysize : the size of key, keep 0 if store pointer only.
    * elesize : the size of instance , keep 0 if store pointer only.
    * compare : the compare function. use to compare the key, to search for key.
    *           function should return >0 if greater, 0 if equal,<0 if less.
    *
    */
   DLLIMPORT BST*         BINTreeCallConv bst_create ( int keysize,int elesize,int ( *compare ) ( const void*,const void* ) );
   /**
    * get count of elements in BST.
    */
   DLLIMPORT unsigned int BINTreeCallConv bst_size ( const BST* tree );
   /**
    * true if BST is empty.
    */
   DLLIMPORT int          BINTreeCallConv bst_empty ( const BST* tree );
   /**
    * search for the data associate with key.
    */
   DLLIMPORT void*        BINTreeCallConv bst_search ( const BST* tree,const void* key );

   /**
    * get the root of BST.
    */
   DLLIMPORT void*        BINTreeCallConv bst_root ( const BST* tree );
   /**
    * get the minimul value of BST.
    */
   DLLIMPORT void*        BINTreeCallConv bst_min ( const BST* tree );
   /**
    * get the maximul value of BST.
    */
   DLLIMPORT void*        BINTreeCallConv bst_max ( const BST* tree );
   /**
    * insert <key,value> pair into BST, order by key.
    */
   DLLIMPORT void         BINTreeCallConv bst_insert ( BST* tree,const void* key,const void* value );
   /**
    * erase one node associate with key from BST.
    */
   DLLIMPORT void         BINTreeCallConv bst_erase ( BST* tree,const void* key );
   /**
    * clear BST.
    */
   DLLIMPORT void         BINTreeCallConv bst_clear ( BST* tree );
   /**
    * delete BST.
    */
   DLLIMPORT void         BINTreeCallConv bst_delete ( BST* tree );
   DLLIMPORT void         BINTreeCallConv bst_bfs_foreach ( BST* tree,void ( *fn ) ( void* ) );
   DLLIMPORT void         BINTreeCallConv bst_dfs_foreach ( BST* tree,void ( *fn ) ( void* ) );
   DLLIMPORT void         BINTreeCallConv bst_preorder ( BST* tree,void ( *fn ) ( void* ) );
   DLLIMPORT void         BINTreeCallConv bst_inorder ( BST* tree,void ( *fn ) ( void* ) );
   DLLIMPORT void         BINTreeCallConv bst_postorder ( BST* tree,void ( *fn ) ( void* ) );


   DLLIMPORT BSTIter      BINTreeCallConv bst_next_iter ( BSTIter iter );
   DLLIMPORT int          BINTreeCallConv bst_iter_isbegin ( const BSTIter iter );
   DLLIMPORT int          BINTreeCallConv bst_iter_isend ( const BSTIter iter );

   DLLIMPORT BSTIter      BINTreeCallConv bst_get_iter ( BST* tree );
   DLLIMPORT void*        BINTreeCallConv bst_iter_data ( const BSTIter iter );
   DLLIMPORT void         BINTreeCallConv bst_iter_rewind ( BSTIter iter );
   DLLIMPORT int          BINTreeCallConv bst_iter_hasNext ( const BSTIter iter );
   DLLIMPORT void*        BINTreeCallConv bst_iter_next ( BSTIter iter );
   DLLIMPORT BSTIter      BINTreeCallConv bst_iter_delete ( BSTIter iter );

#  ifdef __cplusplus
}
#  endif



#endif
