
#define INLINE
#define PRIVATE static

typedef int (*Comparator)(const void* a,const void* b);
INLINE void heap_push( void* a, unsigned int p,const void* data,unsigned int size, Comparator c );
INLINE void heap_pop( void* a, unsigned int p, unsigned int size, Comparator cmp );
INLINE void heap_make(void* a, unsigned int cnt,unsigned int size, Comparator c);

INLINE PRIVATE void 
swap(void* a,void* b,unsigned int size){
   unsigned char t;
   unsigned long tl;
   while(size>=4){
      tl = *(unsigned long*)a;
      *(unsigned long*)a = *(unsigned long*)b;
      *(unsigned long*)b = tl;
      a = (unsigned char*)a + 4;
      b = (unsigned char*)b + 4;
      size -= 4;
   }
   while(size--){
      t = *(unsigned char*)a;
      *(unsigned char*)a++ = *(unsigned char*)b;
      *(unsigned char*)b++ = t;
   }
}

INLINE PRIVATE int 
compare_and_swap(void* a,void* b,unsigned int size,Comparator c){
   if( !c(a,b) ) return 0;
   swap(a,b,size);
   return 1;
}

INLINE PRIVATE 
int get_parent( int i ){ 
    return (i>>1) - ((i&1)^1);
}

INLINE PRIVATE int 
get_left_child( int i ){  
   return ( i<<1 ) + 1;  
}

INLINE PRIVATE int 
get_right_child( int i ){  
   return ( i<<1 ) + 2; 
}

INLINE PRIVATE void 
copy_memory(void* a,const void* b,int size){
   while(size >= 4){
      *(unsigned long*)a = *(unsigned long*)b;
      a = (void*)(unsigned char*)a + 4;
      b = (void*)(unsigned char*)b + 4;
      size -= 4;
   }
   while(size--){
      *(unsigned char*)a++ = *(unsigned char*)b++;
   }
}

INLINE PRIVATE void* 
ptr_arr_offset(void* p,unsigned idx,unsigned int size){
   return (unsigned char*)p + idx * size;
}


#define ADDR(POS) ptr_arr_offset(a,POS,size)
#define ASSIGN(DEST,SRC) copy_memory(DEST,SRC,size)
INLINE void 
heap_push( void* a, unsigned int p,const void* data,unsigned int size, Comparator c ){
   int parent;
   ASSIGN(ADDR(p),data);
   for(parent = get_parent(p); p > 0; p = parent, parent = get_parent( p ) )
     if( !compare_and_swap( ADDR(p),ADDR(parent),size,c ) ) 
        return;
}

INLINE void 
heap_pop( void* a, unsigned int p, unsigned int size, Comparator cmp ){
   int cur,left,right,maxidx;
   if( !p ) return;
   copy_memory( a, ADDR(p--) , size);   
   for( cur=0, left=1, right=2, maxidx=0; 
     left < p; 
     cur = maxidx, left = get_left_child( cur ), right = left + 1
   ){
      maxidx = ( (right >= p)? left : ( cmp( ADDR(left),ADDR(right) ) ?left:right ) );
      if( !compare_and_swap( ADDR(maxidx) , ADDR(cur), size,cmp ) ) 
         return;
   }
}

INLINE void 
heap_make(void* a, unsigned int cnt,unsigned int size, Comparator c){
   int i;
   for(i=0; i<cnt; ++i){
      heap_push(a,i,ADDR( i ),size,c);
   }
}
#undef ADDR


#include<stdlib.h>
#include<stdio.h>
#define N 100000
int intcmp(const void* a,const void* b){   return *(int*)a<*(int*)b; }
int dblcmp(const void* a,const void* b){   return *(double*)a<*(double*)b; }

int main()
{
   float* a = (float*)malloc(N* 4);
   float t;
   int i;
   for(i=0; i<N; ++i){
      /*heap_push(a,i,&(t=rand(),t),sizeof(int),intcmp );*/
      a[ i ] = (double)rand()/RAND_MAX;
   }
   heap_make(a,N,sizeof(float),intcmp);
   i = N;
   while(--i>=0){
     heap_pop(a,i,sizeof(float),intcmp);
     printf("%lf\n",a[ 0 ]);
   }
   system("pause");
   return 0;
}
