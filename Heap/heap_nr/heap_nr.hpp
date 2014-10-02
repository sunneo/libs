#ifndef HEAP_WITH_NO_REALLOCATION_HPP_
#   define HEAP_WITH_NO_REALLOCATION_HPP_
#include <new>
#include <algorithm>
namespace Sunneo {
template<
   typename ty,
   typename Compare,
   bool DeleteNoUseNode=false 
>
class Heap{
   struct Node{
        ty* x;
        size_t _back,_size;
        Node *prev,*next;
        Node(size_t size,Node *const _prev,Node* const _next) 
           : x( new ty[ _size = size ]),_back(0),prev(_prev),next(_next){
        }
        virtual ~Node(){
           if(prev != 0) prev->next = next;
           if(next != 0) next->prev = prev;
           delete[] x;
        }
   };
   Node* root,*last;
   size_t _size;
   ty& NullData() const{
      const static ty _NullData = ty();
      return _NullData;
   }
public:  
   ty top(){
      return empty() ?  NullData() : (const ty&) *root->x;
   }
   const ty& top() const {
      return empty() ?  NullData() : (const ty&) *root->x;
   }
   ty get(){
      ty t = top();
      pop();
      return t;          
   }          
   size_t size() const {      return _size;    }
   bool empty() const {      return _size == 0;    }
   void push(const ty& data){
      Node* n = last;
      Node* prev;
      size_t index = last->_back++;
      ++_size;
      last->x[ index ] = data;
      if( last->_back >= last->_size ){
         if(!n->next) n->next = new Node(n->_size << 1,n,0);
         last = n->next;
      }
      while( (prev = n->prev) !=0 ){
         if(!Compare()(n->x[ index ],prev->x[ index>>1 ]) ) 
            return ;
         std::swap(n->x[ index ],prev->x[ index>>1 ]);
         n = prev;
         index>>=1;
      }
   }
   void pop(){
      if( _size <= 0 ) return;
      --_size;
      if( last->_back == 0 ) {
         Node* p = last->prev;
         if(DeleteNoUseNode) delete last;
         last = p;
      }
      *root->x = last->x[ --last->_back ];
      Node* next;
      Node* current = root;
      size_t cur = 0;
      size_t maxidx,nsize;
      while( (next = current->next)!= 0 && (cur<<1) < (nsize = next->_back)  ){
          maxidx = ( (cur<<1)+1 >= nsize? (cur<<1) : (Compare()(next->x[ (cur<<1) ],next->x[ (cur<<1)+1 ])? (cur<<1):(cur<<1)+1 ));
          if(!Compare()(next->x[ maxidx ],current->x[ cur ]) ) return;
          std::swap(next->x[ maxidx ],current->x[ cur ]);
          cur = maxidx;
          current = next;
      }
   }
   void clear(){
       Node* p;
       while(last!=root){
          p = last->prev;
          delete last;
          last = p;
       }
   }
   void reorder(){  // not checked yet...
      if(_size < 2) return;
      size_t index;
      Node* level = root->next;
      Node* prev,*n;
      while(level){
         for(size_t i = 0; i<level->_back; ++i){
            index = i;
            n = level;
            while( (prev = n->prev) !=0 ){
               if( !Compare()(n->x[ index ],prev->x[ index>>1 ]) ) break ;
               std::swap(n->x[ index ],prev->x[ index>>1 ]);
               n = prev;
               index>>=1;
            }
         }
         level = level->next;
      }
   }
   template<typename functor> void traverse(functor& fn){
       Node* r = root;
       while( r!=0 ){
          for(size_t i=0; i<r->_back; ++i)
            fn(r->x[ i ]);
          r = r->next;
       }
   }
   Heap(): root( new Node(1,0,0) ),last(root),_size(0){}
   virtual ~Heap(){
       clear();
       delete root;
   }
};

} // namespace sunneo
#endif
