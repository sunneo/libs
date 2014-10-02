#include "stack.h"
#include <stdlib.h>
#include <stdio.h>
#ifdef DEBUG
#  define __traceFunction(X) fprintf(stderr,"%s:%s[line:%-5d]:%s\n",__FILE__,__FUNCTION__,__LINE__,X)
#else
#  define __traceFunction(X)
#endif
__inline static void  __fastcall copy_mem ( void* dst,const void* src,long size );
__inline static void  __fastcall stack_realloc ( Stack* stack );
__inline static void* __fastcall stack_array_at ( const Stack* stack,long idx );

static void* malloc_s ( unsigned int s ) {
   void* ret = malloc ( s );
   if ( !ret ) {
      __traceFunction ( "[bad alloc] program terminate" );
      exit ( -1 );
   }
   return ret;
}
static void   StackCall stack_push_ptr ( Stack* stack,const void* data );
static void   StackCall stack_push_data ( Stack* stack,const void* data );
static void*  StackCall stack_top_ptr ( const Stack* stack );
static void*  StackCall stack_top_data ( const Stack* stack );

typedef struct FptrTbl {
   void ( StackCall *fn_push ) ( Stack*,const void* );
   void* ( StackCall *fn_top ) ( const Stack* );
}FptrTbl;

FptrTbl stk_vtbl[2] = {
   {stack_push_data , stack_top_data },
   {stack_push_ptr  , stack_top_ptr}
};

__inline static void __fastcall
copy_mem ( void* dst,const void* src,long size ) {
   while ( size>=4 ) {
      * ( long* ) dst = * ( long* ) src;
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

__inline static void* __fastcall
stack_array_at ( const Stack* stack,long idx ) {
   return ( void* ) ( ( char* ) stack->a ) + idx * stack->elesize;
}

__inline static void __fastcall
stack_realloc ( Stack* stack ) {
   stack->size = stack->size * 2 + 1;
   stack->a = realloc ( stack->a, ( stack->size * stack->elesize ) );
}

Stack* StackCall
stack_create ( long elesize ) {
   Stack* stack = ( Stack* ) malloc_s ( sizeof ( Stack ) );
   stack->a = 0;
   stack->size = 0;
   stack->back = 0;
   stack->elesize = elesize<=0?sizeof ( void* ) :elesize;
   stack->vtbl = &stk_vtbl[ elesize<=0 ];
   return stack;
}

int StackCall
stack_empty ( const Stack* stack ) {
   return ( stack==0 ) || ( stack->back == 0 );
}

static void StackCall
stack_push_data ( Stack* stack,const void* data ) {
   copy_mem ( stack_array_at ( stack,stack->back ),data,stack->elesize );
   ++stack->back;
}

static void StackCall
stack_push_ptr ( Stack* stack,const void* data ) {
   ( ( void** ) stack->a ) [ stack->back++ ] = ( void* ) data;
}


void StackCall
stack_push ( Stack* stack,const void* data ) {
   if ( stack->back == stack->size )
      stack_realloc ( stack );
   ( ( FptrTbl* ) stack->vtbl )->fn_push ( stack,data );
}

void* StackCall
stack_top ( const Stack* stack ) {
   return ( ( FptrTbl* ) stack->vtbl )->fn_top ( stack );
}

void StackCall
stack_pop ( Stack* stack ) {
   if ( stack->back == 0 ) return;
   --stack->back;
   if ( stack->back == 0 ) {
      stack_clear ( stack );
   }
}

static void*  StackCall
stack_top_data ( const Stack* stack ) {
   return ( void* ) ( ( char* ) stack->a + ( stack->back-1 ) * stack->elesize );
}

static void*  StackCall
stack_top_ptr ( const Stack* stack ) {
   return ( ( void** ) stack->a ) [ stack->back-1 ];
}

DLLIMPORT long  StackCall
stack_size ( const Stack* stack ) {
   return stack->back;
}

DLLIMPORT void   StackCall
stack_clear ( Stack* stack ) {
   if ( stack->size == 0 ) return;
   stack->back = 0;
   stack->size = 0;
   free ( stack->a );
   stack->a = 0;
}

DLLIMPORT void   StackCall
stack_delete ( Stack* stack ) {
   stack_clear ( stack );
   free ( stack );
}

