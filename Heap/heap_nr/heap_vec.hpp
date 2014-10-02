#ifndef HEAP_VEC_HPP
#define HEAP_VEC_HPP
#include <vector>
#include <stddef.h>
#include <algorithm>

template<typename ty,typename Compare>
class HeapVec{
  std::vector< std::vector<ty> > a;
  size_t _size;
  inline void push_back(const ty& t){
     
     if(_size && (a.back().size() >= a.back().capacity()) ){
        size_t oldsize = a.back().size();
        a.push_back(std::vector<ty>());
        a.back().reserve(oldsize << 1);
     }
     a.back().push_back(t);
     ++_size;
  }
  inline void pop_back(){
     a.back().pop_back();
     if(!a.back().size() ) a.pop_back();
     --_size;
  }
  inline ty& NullData() const {
     const static ty _NullData = ty();
     return _NullData; 
  }
public:
  inline void clear(){
     a.clear();
  }
  inline const ty& top() const{
     return (empty()?NullData():a[ 0 ][ 0 ]);
  }
  inline ty& top() {
     return (empty()?NullData():a[ 0 ][ 0 ]);
  }
  inline ty get(){
      ty t = top();
      pop();
      return t;
  }
  inline bool empty() const { return _size == 0; }
  inline size_t size() const { return _size; }
  inline void push(const ty& t){
     push_back(t);
     size_t cur = a.back().size()-1;
     size_t n = a.size()-1;
     while(n > 0){
        if(Compare()(a[ n-1 ][ cur>>1 ],a[ n ][ cur ]) ) return;
        std::swap(a[ n-1 ][ cur>>1 ],a[ n ][ cur ]);
        cur>>=1;
        --n;
     }
  }
  inline void pop(){
      if(!_size) return;
      size_t cur = 0;
      size_t child;
      a[ 0 ][ 0 ] = a.back().back();
      pop_back();
      if(!_size) return;
      for(size_t i = 0; i<a.size()-1 && cur*2 < a[ i+1 ].size(); ++i){
         child = ( cur*2+1 >= a[ i+1 ].size()? cur*2 : (Compare()(a[ i+1 ][ cur*2 ],a[ i+1 ][ cur*2+1 ])? cur*2: cur*2+1) ) ;
         if(Compare()(a[ i ][ cur ],a[ i+1 ][ child ]) )  return;
         std::swap(a[ i ][ cur ],a[ i+1 ][ child ]);
         cur = child;         
      }
  }
  template<typename func>inline void traverse(func& fn){
      size_t asize = a.size();
      size_t layer_size;
      for(size_t i=0; i<a.size(); ++i){
        for(size_t j=0; j<a[ i ].size(); ++j)
           fn(a[ i ][ j ]);
      }
  }
  HeapVec():_size(0),a(1){
    
  }
  virtual ~HeapVec(){}
};


#endif
