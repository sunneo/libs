#ifndef _macro_sort_
#define _macro_sort_


#define swap_with_type(TYPE,a,b)     \
do                                   \
{                                    \
   TYPE tmp_to_a_ = a;               \
   a = b;                            \
   b= tmp_to_a_;                     \
}while(0)

#define bbsort(array,size)                                             \
do                                                                     \
{                                                                      \
   __typeof__(size) _i_##size;                                         \
   __typeof__(size) _j_##size;                                         \
   for(_i_##size=0;_i_##size<size; ++_i_##size)                        \
     for(_j_##size=0;_j_##size<size-1; ++_j_##size)                    \
        if(array[ _j_##size ] > array[ _j_##size+1 ] )                 \
          swap_with_type(                                              \
                           __typeof__(*array),                         \
                           array[ _j_##size ] ,                        \
                           array[ _j_##size +1 ]                       \
                         );                                            \
}while(0)

typedef struct 
{
  enum typeno
  { char_n=48,str_n,int_n,float_n,uint_n,double_n }typeno;
}typeno;

#define showArray(TYPENO,array,size)                  \
do                                                    \
{                                                     \
   unsigned int _typeno_ = TYPENO;                    \
   unsigned int _index_to_##size = 0;                 \
   while(_index_to_##size < size)                     \
     switch(_typeno_)                                 \
     {                                                \
         default:                                     \
         case typeno::int_n :                         \
           printf("%d ",array[ _index_to_##size++ ]); \
           break;                                     \
         case typeno::float_n :                       \
           printf("%f ",array[ _index_to_##size++ ]); \
           break;                                     \
         case typeno::uint_n :                        \
           printf("%u ",array[ _index_to_##size++ ]); \
           break;                                     \
         case typeno::double_n:                       \
           printf("%lf ",array[ _index_to_##size++ ]);\
           break;                                     \
         case typeno::char_n:                         \
           printf("%c ",array[ _index_to_##size++ ]); \
           break;                                     \
         case typeno::str_n:                          \
           printf("%c",array[ _index_to_##size++ ]);  \
           break;                                     \
     }                                                \
}while(0)

#endif
