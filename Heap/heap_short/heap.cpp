template< typename ty >
inline void swap(ty& a,ty& b){
   ty t = a;
   a = b;
   b = t;
}

template< typename ty, typename compare >
inline bool compare_and_swap(ty& a,ty& b,compare& c){
   if( !c(a,b) ) return false;
   swap(a,b);
   return true;
}

inline int get_parent( int i ){ return (i>>1)-((i&1)^1); }
inline int get_left_child( int i ){  return ( i<<1 ) + 1;  }
inline int get_right_child( int i ){  return ( i<<1 ) + 2; }

template< typename ty, typename compare >
inline void heap_push( ty a[], int p, const ty& data, compare& c ){
   a[ p ] = data;
   for(int parent = get_parent(p); p > 0; p = parent, parent = get_parent( p ) )
     if( !compare_and_swap( a[ p ],a[ parent ],c ) ) 
        return;
}

template< typename ty, typename compare >
inline void heap_pop( ty a[], int p, compare& cmp ){
   if( !p ) return;
   a[ 0 ] = a[ p-- ];   
   for( int cur=0, left=1, right=2, maxidx=0; 
     left < p; 
     cur = maxidx, left = get_left_child( cur ), right = get_right_child( cur ) 
   ){
      maxidx = ( (right >= p)? left : (cmp(a[ left ],a[ right ])?left:right ) );
      if( !compare_and_swap( a[ maxidx ], a[ cur ], cmp ) ) 
         return;
   }
}

template< typename ty, typename compare >
inline void heap_make(ty a[], int size, compare& c){
    for(int i=0; i<size; ++i) 
      heap_push(a,i,a[ i ],c);
}

template< typename ty, typename compare >
void shiftdown(ty t[],int n,int k,compare& c)
{
   ty up = t[ k ];
   for(int n2 = n>>1,j = 1 + (k<<1); k < n2;k = j, j = 1+(k<<1))
   {
      if( j+1 < n && !c(t[j] , t[j+1]) ) ++j;
      if( c(up ,t[j]) ) break;
      t[k] = t[j];  
   }
   t[k] = up;
}

template< typename ty, typename compare >
void buildheap(ty t[],int n,compare& c)
{
   for(int k = (n>>1)-1; k>=0; --k)
     shiftdown(t,n,k,c);
}

template< typename ty, typename compare >
void heapsort(ty t[],int n,compare& c)
{
   buildheap(t,n,c);
   while(n-->0)
   {
      swap(t[ n ],t[ 0 ]);
      shiftdown(t,n,0,c);
   }
}

#include<stdlib.h>
#include<stdio.h>
#define N 10000
int intcmp(int a,int b){   return a>b; }

int main()
{
   int a[N];
   for(int i=0; i<N; ++i)
      a[i]=rand();
   heapsort(a,N,intcmp);
   for(int i=0; i<N; ++i)
     printf("%d ",a[ i ]);
   system("pause");
   return 0;
}
