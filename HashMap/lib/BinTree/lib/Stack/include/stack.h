#ifndef STACK_H_
#  define STACK_H_

#  ifdef DLLLIB
#    ifdef BUILDING_DLL
#      define DLLIMPORT __declspec(dllexport)
#    else
#      define DLLIMPORT __declspec(dllimport)
#    endif
#  else
#    define DLLIMPORT
#  endif
#  ifndef __fastcall
#    define __fastcall __attribute__((__fastcall__))
#  endif
#  ifndef StackCall
#    define StackCall __fastcall
#  endif


typedef struct DLLIMPORT Stack {
   void* a;
   long size;
   long elesize;
   long back;
   void* vtbl;
}Stack;


#  ifdef __cplusplus
extern "C" {
#  endif

   DLLIMPORT Stack* StackCall stack_create ( long elesize );
   DLLIMPORT int    StackCall stack_empty ( const Stack* stack );
   DLLIMPORT void   StackCall stack_push ( Stack* stack,const void* data );
   DLLIMPORT void   StackCall stack_pop ( Stack* stack );
   DLLIMPORT void*  StackCall stack_top ( const Stack* stack );
   DLLIMPORT long  StackCall stack_size ( const Stack* stack );
   DLLIMPORT void   StackCall stack_clear ( Stack* stack );
   DLLIMPORT void   StackCall stack_delete ( Stack* stack );
#  ifdef __cplusplus
}
#  endif
#endif
