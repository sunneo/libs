#include "hashmap.h"
#include <stdio.h>
int streq ( const char* str1,const char* str2 ) {
   return strcmp ( str1,str2 ) == 0;
}
int str_hash ( const char* str ) {
   int ret = 0;
   while ( *str ) {
      ret+=*str;
      ++str;
   }
   return ret;
}
int main() {
   struct HashMap* hash = hashmap_new((int(*)(const void*))str_hash,(int(*)(const void*,const void*))streq);
   //struct HashMap* hash = hashmap_new ( 0,0 );
   int value[ 10 ];
   //int key[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
   const char* key[ 10 ] = { "one","two","three","four","five","six","seven","eight","nine","ten"};
   int i;
   printf ( "hash is %p\n",hash );
   for ( i=0; i<10; ++i ) {
      value[ i ] = rand();
      printf ( "put %s=%d\n",key[ i ],value[ i ] );
      {
         printf ( "BEGIN OF PUT(%s)_________________________________________________________________________\n",key[i] );
         hashmap_put ( hash, ( const void* ) key[ i ], ( const void* ) value[ i ] );
         printf ( "END OF PUT----------------------------------------------------------------------------\n" );
      }
   }
   for ( i=0; i<10; ++i ) {
      printf ( "BEGIN OF REMOVE(%s)_______________________________________________________________________\n", key[i]);
      printf ( "remove %s=%d\n",key[i], ( int ) hashmap_remove ( hash, ( const void* ) key[ i ] ) );
      printf ( "END OF REMOVE-------------------------------------------------------------------------\n" );
   }
   printf ( "_____________________________________________________________________________\n" );
   //hashmap_clear ( hash );
   for ( i=0; i<10; ++i ) {
      value[ i ] = rand();
      printf ( "BEGIN OF PUT2(%s)_________________________________________________________________________\n",key[i] );
	  {
         printf ( "put %s=%d\n",key[ i ],value[ i ] );
         hashmap_put ( hash, ( const void* ) key[ i ], ( const void* ) value[ i ] );
         printf ( "END OF PUT2---------------------------------------------------------------------------\n" );
	  }
   }
   for ( i=10-1; i>=0; --i ) {
      printf ( "BEGIN OF REMOVE2(%d)______________________________________________________________________\n",key[i] );
      printf ( "remove %s=%d\n",key[i], ( int ) hashmap_remove ( hash, ( const void* ) key[ i ] ) );
      printf ( "END OF REMOVE2-----------------------------------------------------------------------\n" );

   }

   hashmap_delete ( hash );
   return 0;
}

