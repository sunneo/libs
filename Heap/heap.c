#include <stdlib.h>

#define _REG register
#ifndef __fastcall
#  define __fastcall __attribute__((__fastcall__))
#endif
/***
 * Heap Operation written by IceCold, 2009/01/01.
 * heap_push(void* array,int backidx,int elesiz,const void* data,HeapCompare Compare);
 * heap_pop(void* array,int backidx,int elesiz,HeapCompare compare);
 * heap_sort(void* array,int cnt,int elesiz,HeapCompare compare);
 *
 * array     : the entry of the array.
 * backidx   : the back/rear index of the array,which represent the last item of the array.
 * elesiz    : size per data.
 * data(push): data to push.
 * compare   : compare function.
 * cnt(sort) : count of the array to sort.
 *
 * example:
 * int intcmp(const void* a,const void* b){    return *(int*)a > *(int*)b;   }
 * #define N 1000
 * int main(){
 *    int a[N];
 *    int i;
 *    for(i=0; i<N; ++i) a[ i ] = rand()%N;
 *    heap_sort(a,N,sizeof(int),intcmp);
 *    while(i-- > 0) heap_pop(a,i,sizeof(int),intcmp);
 *    return 0;
 * }
 */

typedef int (__stdcall *HeapCompare)(const void*,const void*);
____fastcall void heap_push( void* array, int bkidx, int elesiz,const void* data,HeapCompare compare);
____fastcall void heap_pop( void* array, int bkidx, int elesiz,HeapCompare compare);
____fastcall void heap_sort( void* array, int cnt, int elesiz,HeapCompare compare);




static __inline void __fastcall 
swap_s(_REG unsigned char* a,_REG unsigned char* b,_REG int length)
{
   unsigned char t;
   unsigned long tl;
   if(a==b) return;
   while(length>=4)
   {
      tl = *(unsigned long*)a;
      *(unsigned long*)a = *(unsigned long*)b;
      *(unsigned long*)b = tl;
      a+=4;
      b+=4;
      length -=4;
   }
   while(length--) *a++ = *b++;
}

static __inline void __fastcall 
copy_data(_REG unsigned char *a,_REG const unsigned char *b,int siz)
{
   if(a == b) return;
   while(siz>=4){
      *(unsigned long*)a = *(unsigned long*)b;
      siz-=4;
      a+=4;
      b+=4;
   }
   while(siz--) *a++ = *b++;
}

static __inline int __fastcall 
compare_and_swap( int elesiz,HeapCompare compare,void* a,void *b){
   if(compare(a,b) <= 0) return 0;
   swap_s(a,b,elesiz);
   return 1;
}

static __inline int __fastcall 
get_parent_idx(int i){
    return (i>>1)-((i&1)^1);
}


__fastcall void 
heap_push( void* array, int backidx, int elesiz,const void* data,HeapCompare compare)
{
   _REG int idxparent;
   unsigned char* pidx = ( unsigned char* )array + elesiz* backidx;
   unsigned char* pidxparent;
   copy_data( pidx, data, elesiz );
   if(!compare) return;
   for( ;backidx != 0; backidx = idxparent,pidx = pidxparent )
   {
      idxparent = get_parent_idx(backidx);
      pidxparent = ( unsigned char* )array + elesiz* idxparent;
      if( !compare_and_swap(elesiz,compare, pidx, pidxparent )) return;
   }
}


__fastcall
void heap_sort( void* array, int cnt, int elesiz,HeapCompare compare)
{
   int i = 0;
   unsigned char* pdata = (unsigned char*)array;
   while( (++i < cnt) && (pdata += elesiz) )
      heap_push(array,i,elesiz,pdata,compare);
}

#define COMPARETYPE(TYPE) int(__stdcall *)(const TYPE,const TYPE)
#define DECLCOMP(TYPE,VARNAME) int(*__stdcall VARNAME)(const TYPE,const TYPE)
#define M_SWAP(TYPE,A,B) \
do{                    \
   TYPE __swaptmp = (A); \
   (A) = (B);              \
   (B) = __swaptmp;      \
}while(0)

#define M_ParentIdx(CURIDX) ((CURIDX>>1)-((CURIDX&1)^1))
#define M_heap_push(TYPE,__ARRAY,__BACK__IDX,DATA,COMPARE_FUNC_PTR)                     \
do{                                                                                     \
   int __IDXPARENT;                                                                     \
   int __IDX;                                                                           \
   int __IDXBACK = __BACK__IDX;                                                         \
   DECLCOMP(TYPE,Compare) = ( COMPARETYPE(TYPE) )COMPARE_FUNC_PTR;                        \
   __ARRAY[ __IDXBACK ] = DATA;                                                         \
   if(Compare == NULL) break;                                                           \
   for( ;__IDXBACK; __IDXBACK = __IDXPARENT)                                            \
   {                                                                                    \
      __IDXPARENT = M_ParentIdx(__IDXBACK);                                            \
      if( Compare( __ARRAY[ __IDXBACK ],__ARRAY[ __IDXPARENT ]) <= 0) break;            \
      M_SWAP(TYPE,__ARRAY[ __IDXBACK ],__ARRAY[ __IDXPARENT ]);                         \
   }                                                                                    \
}while(0)


#define M_heap_sort(TYPE,__ARRAY,COUNT,COMPARE_FUNC_PTR)            \
do{                                                                 \
   int __I = 0;                                                     \
   while(++__I < COUNT)                                             \
      M_heap_push(TYPE,__ARRAY,__I,__ARRAY[ __I ],COMPARE_FUNC_PTR);\
}while(0)

#define M_heap_pop(TYPE,__ARRAY,__BACKIDX,COMPARE_FUNC_PTR)                                            \
do{                                                                                                    \
   int MAXARR[ 2 ]={1,2};                                                                              \
   int __IDXCURR,__IDXMAX,__IDXBACK = 0;                                                               \
   if(__BACKIDX == 0) break;                                                                           \
   M_SWAP(TYPE,__ARRAY[ 0 ],__ARRAY[ __BACKIDX ]);                                                     \
   for(__IDXCURR = 0,__IDXBACK=__BACKIDX ;MAXARR[ 0 ] < (__IDXBACK);__IDXCURR = __IDXMAX, MAXARR[ 0 ] = 1 + (__IDXCURR<<1),MAXARR[ 1 ] = MAXARR[ 0 ] + 1)                                                                                             \
   {                                                                                                   \
      __IDXMAX = (MAXARR[ 1 ] >= __IDXBACK ? MAXARR[ 0 ] : (MAXARR[ (COMPARE_FUNC_PTR( __ARRAY[ MAXARR[ 1 ] ],__ARRAY[ MAXARR[ 0 ] ]) > 0) ]));   \
      if( COMPARE_FUNC_PTR( __ARRAY[ __IDXMAX ],__ARRAY[ __IDXCURR ] ) <= 0 ) break;                   \
      M_SWAP(TYPE,__ARRAY[ __IDXCURR ],__ARRAY[ __IDXMAX ]);                                           \
   }                                                                                                   \
}while(0)


#define PTR2IDX(PTR,START,ELESIZ) (( (unsigned char*)(PTR) - (unsigned char*)(START) ) / (ELESIZ) )

__fastcall
void heap_pop( void* array, int backidx, int elesiz,HeapCompare compare)
{
   unsigned char* pmax[2] = { (unsigned char*)array + elesiz,(unsigned char*)array + elesiz*2 };
   _REG unsigned char* pidx_cur = array;
   unsigned char* pend = pidx_cur + backidx * elesiz;
   _REG unsigned char* pmax_idx = 0;
   copy_data(array,pend,elesiz);
   if( backidx == 0 ) return;
   while(pmax[ 0 ] < pend){
      if( pmax[ 1 ] >= pend ) pmax_idx = pmax[ 0 ];
      else  pmax_idx = pmax[ ( compare( pmax[ 1 ], pmax[ 0 ] ) > 0 ) ];
      if( !compare_and_swap(elesiz,compare,pmax_idx,pidx_cur)) return;
      pidx_cur = pmax_idx;
      pmax[ 0 ] = (unsigned char*)array + ( ( PTR2IDX( pidx_cur,array,elesiz ) <<1 ) + 1 ) * elesiz;
      pmax[ 1 ] = pmax[ 0 ] + elesiz;
   }
}
#undef PTR2IDX


#endif
