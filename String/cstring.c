#include "cstring.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

__inline static unsigned  cstring_length ( const char* str );
__inline static unsigned  find_approxiate_length ( unsigned begin,unsigned len );
__inline static void   copy_string ( char* dst,const char* src );
__inline static void   copy_mem ( void* dst,const void* src,unsigned size );
__inline static void   string_realloc ( String* str,unsigned newlen );

__inline static void
copy_mem ( void* dst,const void* src,unsigned size ) {
   while ( size>=4 ) {
      * ( unsigned* ) dst = * ( unsigned* ) src;
      dst = ( void* ) ( ( char* ) dst + 4 );
      src = ( const void* ) ( ( char* ) src + 4 );
      size -= 4;
   }
   while ( size ) {
      * ( unsigned char* ) dst = * ( unsigned char* ) src;
      dst = ( void* ) ( ( char* ) dst + 1 );
      src = ( const void* ) ( ( char* ) src + 1 );
      size -= 1;
   }
}


String* StringCall string_append_format ( String* dst,const char* fmt,... ) {
   char* tmpbuf;
   int tmpsize = 4095;
   int len = 0;
   va_list parg;
   va_start ( parg,fmt );
   tmpbuf = ( char* ) malloc ( tmpsize );
   if ( tmpbuf == 0 ) {
      perror ( "nomem" );
      exit ( 0 );
   }
   while ( 1 ) {
      len = vsnprintf ( tmpbuf,tmpsize,fmt,parg );
      if ( -1 == len ) {
         tmpsize = tmpsize * 2 + 1;
         tmpbuf = ( char* ) realloc ( ( void* ) tmpbuf,tmpsize );
         if ( tmpbuf == 0 ) {
            perror ( "nomem" );
            exit ( 0 );
         }
      } else {
         break;
      }
   }
   string_append_cstr ( dst,tmpbuf );
   va_end ( parg );
   free ( tmpbuf );
   return dst;
}


__inline static unsigned
cstring_length ( const char* str ) {
   const char* end = str;
   while ( *end ) ++end;
   return ( unsigned ) ( end - str );
}

__inline static unsigned
find_approxiate_length ( unsigned begin,unsigned len ) {
   if ( !begin ) begin = 1;
   while ( begin < len ) begin<<=1;
   return begin;
}

__inline static void
copy_string ( char* dst,const char* src ) {
   while ( *dst++ = *src++ );
}


String* StringCall
string_create ( const char* str ) {
   String* s = ( String* ) malloc ( sizeof ( String ) );
   s->capacity = 0;
   s->a = 0;
   string_realloc ( s,cstring_length ( str ) );
   copy_mem ( s->a,str,s->length );
   return s;
}

unsigned StringCall
string_length ( const String* str ) {
   return str->length;
}

int StringCall
string_empty ( const String* str ) {
   return str->length == 0;
}

char* StringCall
string_buffer ( String* str ) {
   if ( *string_end ( str ) != 0 ) {
      string_append_char ( str,0 );
      --str->length;
   }
   return str->a;
}

char* StringCall
string_begin ( const String* str ) {
   return str->a;
}

char* StringCall
string_end ( const String* str ) {
   return str->a + str->length;
}

String* StringCall
string_copy ( String* dst,const String* src ) {
   string_realloc ( dst,src->length );
   copy_string ( dst->a,src->a );
   return dst;
}

String* StringCall
string_copy_cstr ( String* dst,const char* src ) {
   string_realloc ( dst,cstring_length ( src ) );
   copy_string ( dst->a,src );
   return dst;
}

void  StringCall
string_clear ( String* str ) {
   str->length = 0;
   str->capacity = 0;
   free ( str->a );
   str->a = 0;
}

String* StringCall
string_append_cstr ( String* dst,const char* src ) {
   unsigned len = cstring_length ( src );
   unsigned end_off = dst->length;
   string_realloc ( dst,dst->length + len );
   copy_mem ( dst->a + end_off,src,len );
   return dst;
}

__inline static void
string_realloc ( String* str,unsigned newlen ) {
   unsigned oldcap = str->capacity;
   str->length = newlen;
   str->capacity = find_approxiate_length ( oldcap,str->length );
   str->a = ( char* ) realloc ( str->a,str->capacity );
}

String* StringCall
string_append ( String* dst,const String* src ) {
   unsigned end_off = dst->length;
   string_realloc ( dst,dst->length  + src->length );
   copy_mem ( dst->a + end_off,src->a,src->length );
   return dst;
}

String* StringCall
string_append_char ( String* dst,char c ) {
   string_realloc ( dst,dst->length +1 );
   * ( string_end ( dst )-1 ) = c;
   return dst;
}

void StringCall
string_delete ( String* str ) {
   string_clear ( str );
   free ( str );
}

char*   StringCall
string_findfirst ( const String* str,int idx_token,const char* token ) {
    if (idx_token >= string_length(str) || idx_token < 0) return NULL;
    return strstr (string_buffer(str) + idx_token, token);
}
