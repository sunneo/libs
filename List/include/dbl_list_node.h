#ifndef DBL_LIST_NODE_H_
#define DBL_LIST_NODE_H_
#define INLINE __inline
#define UDirNode uDirect.StructNode
#define UDirArray uDirect.direct
#define DIRPrev 0
#define DIRNext 1

typedef struct DblListNode
{
   void* data;
   union
   {
      struct
      {
         struct DblListNode *prev,*next;
      } StructNode;
      struct DblListNode *direct[2];
   } uDirect;
} DblListNode;

DblListNode*  dbllistnode_create ( const void* data,DblListNode* prev,DblListNode* next,unsigned int elesize );

#endif
