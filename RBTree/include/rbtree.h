#ifndef RBTREE_H_
#   define RBTREE_H_
#   ifndef RBTCONV
#      define RBTCONV __fastcall
#   endif
#   ifdef DLLLIB
#      ifdef BUILD_DLL
#         define DLLIMPORT __declspec(dllexport)
#      else 
#         define DLLIMPORT __declspec(dllimport)
#      endif
#   else
#      define DLLIMPORT
#   endif
#include <stddef.h>
typedef struct DLLIMPORT RBNode{
   void *key,*data;
   struct RBNode *parent;
   struct {
      struct RBNode *child[2];
      short color[2]; 
      /* assume every node have 2 child with color */
      /* [color on edge] */
   }childNode;
}RBNode;

typedef struct DLLIMPORT RBTree{
   RBNode* root;
   size_t size;
   int (*compare)(const void*,const void*);
   RBNode* min,*max;
   int elesize,keysize;
}RBTree;


#   ifdef __cplusplus
extern "C" {
#   endif

DLLIMPORT RBTree*  RBTCONV rbtree_create(int elesize,int keysize,int(*cmp)(const void*,const void*));
DLLIMPORT void     RBTCONV rbtree_insert(RBTree* rbtree,const void* key,const void* value);
DLLIMPORT size_t   RBTCONV rbtree_size(const RBTree* rbtree);
DLLIMPORT int      RBTCONV rbtree_empty(const RBTree* rbtree);
DLLIMPORT void     RBTCONV rbtree_erase(RBTree* rbtree,const void* key);
DLLIMPORT void     RBTCONV rbtree_erase_min(RBTree* rbtree);
DLLIMPORT void     RBTCONV rbtree_erase_max(RBTree* rbtree);
DLLIMPORT void     RBTCONV rbtree_erase_root(RBTree* rbtree);
DLLIMPORT void*    RBTCONV rbtree_root(const RBTree* rbtree);
DLLIMPORT void*    RBTCONV rbtree_min(const RBTree* rbtree);
DLLIMPORT void*    RBTCONV rbtree_max(const RBTree* rbtree);
DLLIMPORT void*    RBTCONV rbtree_search(const RBTree* rbtree,const void* key);
DLLIMPORT void     RBTCONV rbtree_bfs_foreach(RBTree* rbtree, void(*)(void*) );
DLLIMPORT void     RBTCONV rbtree_dfs_foreach(RBTree* rbtree, void(*)(void*) );
DLLIMPORT void     RBTCONV rbtree_preorder(RBTree* tree,void(*)(void*));
DLLIMPORT void     RBTCONV rbtree_inorder(RBTree* tree,void(*)(void*));
DLLIMPORT void     RBTCONV rbtree_postorder(RBTree* tree,void(*)(void*));
DLLIMPORT void     RBTCONV rbtree_clear(RBTree* rbtree);
DLLIMPORT void     RBTCONV rbtree_delete(RBTree* rbtree);


#   ifdef __cplusplus
};
#   endif
#endif
