#ifndef MACRO_Array_h_
#define MACRO_Array_h_
#include <stdlib.h>
#include <stdio.h>
/************************************MacroHeaderFile **************************
 *  Built By IceCold 2007 Nov.27.Sat 02:05:00
 *  
 *  At First, Call    
 *                         SequenceDefine(TYPE);         at global scope
 *  so that you could use: 
 *                         SequenceFunctionCall(Type,Behavior) ;
 *                         FunctionCall(Sequence,Type,Behavior );
 *  to complete some behavior defined
 *  
 *  for example:
 * ______________________________________________________________________________
       SequenceDefine(int);
 
       int main()
       {
          SequenceDeclare(int)* s = SequenceFunctionCall(int,Create)(10,10);
          SequenceFunctionCall(int,Show)(s);
          SequenceFunctionCall(int,Destroy)(s);
          return 0;
       }
   ________________________________________________________________________________
 *  Behavior defined: Create,Show,Destroy
 *
 *
 *  and so do `Array`
 *
 **********************************************************************************/
#define Array(TYPE)      \
struct Dimension1_##TYPE \
{                        \
   TYPE* data;           \
   unsigned long size;   \
}


#define __ArrayDefine(TYPE) typedef Array(TYPE) def_Array_##TYPE
#define ArrayDeclare(TYPE) def_Array_##TYPE
#define __ArrayFunctionDeclare(TYPE)                                                   \
ArrayDeclare(TYPE)* ArrayCreate_##TYPE(unsigned long size)                             \
{                                                                                      \
   ArrayDeclare(TYPE)* ret = (ArrayDeclare(TYPE)*)malloc(sizeof(ArrayDeclare(TYPE)));  \
   ret->data = (TYPE*)calloc((ret->size = size),sizeof(TYPE));                         \
   return ret;                                                                         \
}                                                                                      \
                                                                                       \
void ArrayDestroy_##TYPE(ArrayDeclare(TYPE)* array)                                    \
{                                                                                      \
   free(array->data);                                                                  \
   free(array);                                                                        \
}                                                                                      \
                                                                                       \
void ArrayShow_##TYPE(ArrayDeclare(TYPE)* array)                                       \
{                                                                                      \
   unsigned long i=0;                                                                  \
   for(i=0; i<array->size; ++i) printf("%u ",(unsigned long)array->data[ i ]);         \
   printf("\n");                                                                       \
}                                                                                      \
                                                                                       \
ArrayDeclare(TYPE)* ArrayResize_##TYPE(ArrayDeclare(TYPE)* array,unsigned long newSize)\
{                                                                                      \
   array->data = (TYPE*)realloc(array->data,sizeof(TYPE)*(array->size=newSize));       \
   return array;                                                                       \
}                                                                                      \
                                                                                       \
ArrayDeclare(TYPE)* ArrayErase_##TYPE(ArrayDeclare(TYPE)* array)                       \
{                                                                                      \
   array->data = (TYPE*)memset(array->data,0,sizeof(TYPE)*array->size);                \
   return array;                                                                       \
}



#define Sequence(TYPE)    \
struct Dimension2_##TYPE  \
{                         \
   TYPE** data;           \
   unsigned long row;     \
   unsigned long col;     \
}

#define __SequenceDefine(TYPE) typedef Sequence(TYPE) def_Sequence_##TYPE
#define SequenceDeclare(TYPE) def_Sequence_##TYPE
#define __SequenceFunctionDeclare(TYPE)                                                       \
SequenceDeclare(TYPE)* SequenceCreate_##TYPE(unsigned long row,unsigned long col)             \
{                                                                                             \
   unsigned long index = 0;                                                                   \
   SequenceDeclare(TYPE)* ret;                                                                \
   ret = (SequenceDeclare(TYPE)*)malloc(sizeof(  SequenceDeclare(TYPE) ));                    \
   ret->data = malloc(sizeof(TYPE*)*(ret->row=row));                                          \
   for(index=0; index<row; ++index)                                                           \
     ret->data[ index ] = (TYPE*)calloc((ret->col=col),sizeof(TYPE));                         \
   return ret;                                                                                \
}                                                                                             \
                                                                                              \
void SequenceDestroy_##TYPE(SequenceDeclare(TYPE)* sequence)                                  \
{                                                                                             \
   unsigned long index = 0;                                                                   \
   for(index = 0; index < sequence->row; ++index) free(sequence->data[ index ]);              \
   free(sequence->data);                                                                      \
   free(sequence);                                                                            \
}                                                                                             \
                                                                                              \
void SequenceShow_##TYPE(SequenceDeclare(TYPE)* sequence)                                     \
{                                                                                             \
   unsigned long i = 0;                                                                       \
   unsigned long j = 0;                                                                       \
   for(i=0; i<sequence->row; ++i)                                                             \
   {                                                                                          \
      for(j=0; j<sequence->col; ++j)                                                          \
        printf("%u ",(unsigned long)sequence->data[ i ][ j ]);                                \
      printf("\n");                                                                           \
   }                                                                                          \
}                                                                                             \
                                                                                              \
SequenceDeclare(TYPE)* SequenceErase_##TYPE(SequenceDeclare(TYPE)* sequence)                  \
{                                                                                             \
   unsigned long i=0;                                                                         \
   for(i=0; i<sequence->row; ++i)                                                             \
     sequence->data[ i ] = (TYPE*)memset(sequence->data[ i ],0,sizeof(TYPE)*sequence->col);   \
   return sequence;                                                                           \
}


#define ArrayFunctionCall(TYPE,BEHAVIOR) Array##BEHAVIOR##_##TYPE
#define ArrayDefine(TYPE) __ArrayDefine(TYPE); __ArrayFunctionDeclare(TYPE);
#define SequenceDefine(TYPE) __SequenceDefine(TYPE); __SequenceFunctionDeclare(TYPE);
#define SequenceFunctionCall(TYPE,BEHAVIOR) Sequence##BEHAVIOR##_##TYPE

#define StructBehavior(STRUCT,BEHAVIOR) STRUCT##BEHAVIOR
#define FunctionCall(STRUCT,BASETYPE,BEHAVIOR) STRUCT##FunctionCall(BASETYPE,BEHAVIOR)



#endif
