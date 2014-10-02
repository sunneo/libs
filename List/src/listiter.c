#include "list.h"
#include "../include/dbl_list_node.h"
#include <string.h>
#include <stdlib.h>
typedef struct
{
   List* list;
   ListNode* node;
   short isBegin;
   short isEnd;
} ListIterStruct;

ListIter  
list_get_iter ( const List* list )
{
   ListIterStruct* ret;
   if ( list == 0 ) return 0;
   ret = ( ListIterStruct* ) malloc ( sizeof ( ListIterStruct ) );
   if ( ret == 0 ) return 0;
   ret->list = ( List* ) list;
   ret->node = list->front;
   ret->isBegin = 0;
   ret->isEnd = 0;
   return ( ListIter ) ret;
}

int 
list_iter_hasNext ( const ListIter listiter )
{
   return ( listiter != 0 ) && !list_empty ( ( ( ListIterStruct* ) listiter )->list )  &&
          (  (  ( ListIterStruct* ) listiter )->node != 0 ) &&
          (  (  ( ListIterStruct* ) listiter )->isEnd == 0 );
}

int 
list_iter_hasPrev ( ListIter listiter )
{
   return ( listiter != 0 ) &&  !list_empty ( ( ( ListIterStruct* ) listiter )->list ) &&
          (  (  ( ListIterStruct* ) listiter )->node != 0 ) &&
          ( ( ( ListIterStruct* ) listiter )->isBegin == 0 ) ;
}

void*
 list_iter_data ( const ListIter iter )
{
   if ( iter && ( ( ListIterStruct* ) iter )->node )
      return ( ( ListIterStruct* ) iter )->node->data;
   return 0;
}

INLINE static void* 
list_iter_next_dir ( ListIter iter,int dir )
{
   void* ret;
   ListNode* node;
   if ( iter == 0 ) return 0;
   node = ( ( ListIterStruct* ) iter )->node;
   ( ( ListIterStruct* ) iter )->isEnd = ( node ==0 || node->next == 0 );
   ( ( ListIterStruct* ) iter )->isBegin = ( node ==0 || node->prev == 0 );
   if ( !node ) return 0;
   ret = list_iter_data ( iter );
   if ( dir )
   {
      if ( node->next )
      {
         ( ( ListIterStruct* ) iter )->node = node->next;
      }
   }
   else
   {
      if ( node->prev )
      {
         ( ( ListIterStruct* ) iter )->node = node->prev;
      }
   }
   return ret;
}

void* 
list_iter_prev ( ListIter iter )
{
   return list_iter_next_dir ( iter,0 );
}

void* 
list_iter_next ( ListIter iter )
{
   return list_iter_next_dir ( iter,1 );
}

ListIter 
list_iter_rewind ( ListIter iter )
{
   if ( iter == 0 ) return 0;
   ( ( ListIterStruct* ) iter )->node = ( ( ListIterStruct* ) iter )->list->front;
   return iter;
}

void 
list_iter_delete ( ListIter iter )
{
   free ( iter );
}
