#ifndef MACROBASE_H_
#define MACROBASE_H_

#ifndef MACROBegin 
#  define MACROBegin do{
#endif
#ifndef MACROEnd
#  define MACROEnd }while(0)
#endif
#ifndef MACROEscape
#  define MACROEscape break
#endif

union 
{
   int _int;
   short _short;
   long _long;
   char *_lpstr;
   void *_lpvoid;
   char _char;
   float _float;
   double _double;
}___g__basetype__lv,___g__basetype__rv;



#ifndef _min
#  define _min(PTR__Result,A,B)                                           \
   MACROBegin                                                             \
      __typeof__(A) _tmp_A = (A);                                         \
      __typeof__(B) _tmp_B = (B);                                         \
      if((PTR__Result))*(PTR__Result) = _tmp_A > _tmp_B ? _tmp_B :_tmp_A ;\
   MACROEnd
#endif

#ifndef _max
#  define _max(PTR__Result,A,B)                                          \
   MACROBegin                                                            \
      __typeof__(A) _tmp_A = (A);                                        \
      __typeof__(B) _tmp_B = (B);                                        \
      if(PTR__Result) *(PTR__Result) = _tmp_A > _tmp_B ? _tmp_A :_tmp_B ;\
   MACROEnd
#endif

#ifndef minimum
#  define minimum(RefA,RefB) ((  *(__typeof__((RefA))*)(___g__basetype__lv._lpvoid = &(RefA)) ) > (*(__typeof__((RefB))*)(___g__basetype__rv._lpvoid = &(RefB)))? (*   (__typeof__((RefB))*)(___g__basetype__rv._lpvoid) ): (*(__typeof__((RefA))*)(___g__basetype__lv._lpvoid) ))
#endif

#ifndef maximum
#  define maximum(RefA,RefB) ((  *(__typeof__((RefA))*)(___g__basetype__lv._lpvoid = &(RefA)) )> (*(__typeof__((RefB))*)(___g__basetype__rv._lpvoid = &(RefB)))? (*   (__typeof__((RefA))*)(___g__basetype__lv._lpvoid) ): (*(__typeof__((RefB))*)(___g__basetype__rv._lpvoid) ))
#endif


#endif
