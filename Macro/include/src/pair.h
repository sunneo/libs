#ifndef PAIR_H_
#define PAIR_H_

#define DEF_PAIR(TYPE1,TYPE2)\
  struct                     \
  {                          \
     TYPE1 first;            \
     TYPE2 second;           \
  }
#define pair_def(type1,type2) typedef DEF_PAIR(type1,type2) pair_##type1##_##type2
#define pair_decl(type1,type2) pair_##type1##_##type2

#endif
