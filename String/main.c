#include <stdio.h>
#include <stdlib.h>
#include "cstring.h"
int main ( int argc, char *argv[] ) {
   String* str = string_create ( "Hello World" );
   str = string_concat_format ( str,"KerKer %d\n%d\n",1,2 );
   printf ( "%s\n",string_buffer ( str ) );
   printf ( "capacity = %u\n",str->capacity );
   printf ( "length = %u\n",string_length ( str ) );
   string_delete ( str );
   return 0;
}
