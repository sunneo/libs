#ifndef HEAP_TEMPLATE_HPP
#define HEAP_TEMPLATE_HPP
#include <vector>
#include <algorithm>
template<typename ty,typename Compare>
class Heap{
   std::vector<ty> a;
   inline size_t get_parent(size_t i) const {
       return (i>>1) - ((i&1)^1);
   }
   ty& NullData() const{
      const static ty _NullData = ty();
      return _NullData;
   } 
public:
   inline size_t size() const { return a.size(); }
   inline bool empty() const { return a.empty(); }
   inline const ty& top() const { 
       return empty()?NullData():a[ 0 ]; 
   }
   inline ty& top() { 
       return empty()?NullData():a[ 0 ]; 
   }
   inline void push(const ty& t){
      a.push_back(t);
      for(size_t cur = size()-1,parent = get_parent(cur); cur; cur = parent,parent = get_parent(cur) ){
        if(!Compare()(a[cur],a[parent]) ) return;
        std::swap(a[cur],a[parent]);
      }
   }
   inline void pop(){
      if(empty()) return;
      a[ 0 ] = a.back();
      a.pop_back();
      for(size_t cur=0,left=1,right=2,maxidx; left<a.size(); cur=maxidx,left=(cur<<1)+1,right=left+1){
         maxidx = (right>=a.size()?left:(Compare()(a[ left ],a[ right ])?left:right) );
         if(!Compare()(a[maxidx],a[cur]) ) return;
         std::swap(a[maxidx],a[cur]);
      }
   }
   inline ty get() {
      ty r = top();
      pop();
      return r;
   }
   template<typename fn>inline void traverse(fn& f){
      for(int i=0; i<size(); ++i)
        fn(a[ i ]);
   }
   inline void readjust(){
      for(size_t p = 1; p < size(); ++p){
         for(size_t parent = get_parent(p),cur=p; cur; cur = parent,parent = get_parent(cur)){
            if(!Compare()(a[cur],a[parent]) ) break;
            std::swap(a[cur],a[parent]);
         }
      }
   }
   inline void clear(){
      a.clear();
   }
};

#endif
