#include <stdio.h>
#include <stdlib.h>
#include "vector.h"

static void
printptr ( void* v )
{
   printf ( "%d ",* ( int* ) v );
}

static void
show_vector ( Vector* vector )
{
   VectorIter iter = vector_get_iter ( vector );
   while ( vector_iter_hasNext ( iter ) )
   {
      printptr ( vector_iter_next ( iter ) );
   }
   vector_iter_delete ( iter );
   printf ( "\n" );
}

static int*
create_int ( int i )
{
   int *a =  ( int* ) malloc ( sizeof ( int ) );
   *a = i;
   return a;
}

static void
vector_push_back_int ( Vector* vector,int i )
{
   /*vector_push_back(vector,&i);*/
   vector_push_back ( vector,create_int ( i ) );
}

static void
vector_insert_int ( Vector* vector,int idx,int val )
{

   vector_insert ( vector,idx,create_int ( val ) );
}

static void
pushback_test ( Vector* vector )
{
   int i = 0;
   printf ( "%s\n",__FUNCTION__ );
   for ( i=0; i<10; ++i )
      vector_push_back_int ( vector,rand() );
   show_vector ( vector );

}

static void
erase_test ( Vector* vector )
{
   printf ( "%s\n",__FUNCTION__ );
   vector_erase ( vector,2 );
   show_vector ( vector );
}

static void
erase_range_test ( Vector* vector )
{
   printf ( "%s\n",__FUNCTION__ );
   vector_erase_range ( vector,5,101 );
   show_vector ( vector );
}

static void
insert_test ( Vector* vector )
{
   int i = 0;
   printf ( "%s\n",__FUNCTION__ );
   for ( i=0; i<100; ++i )
   {
      vector_insert_int ( vector,1,86 );
   }
   show_vector ( vector );
}

static void
safe_swap_test ( Vector* vector )
{
   printf ( "%s\n",__FUNCTION__ );
   vector_swap_s ( vector,-5,100 );
   show_vector ( vector );
}

int main ( int argc, char *argv[] )
{
   Vector* vector = vector_create ( 0 );
   pushback_test ( vector );
   erase_test ( vector );
   insert_test ( vector );
   erase_range_test ( vector );
   safe_swap_test ( vector );
   vector_delete ( vector );
   return 0;
}
