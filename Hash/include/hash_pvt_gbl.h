#ifndef HASHTABLE_PRIVATE_GLOBAL_H_
#  define HASHTABLE_PRIVATE_GLOBAL_H_
#include "list.h"
/**
 * we may use string as key.
 * the external function with string key would be
 * int stringHashFunction(const void* ptr){
 *    int ret = 0;
 *    char* p = (char*)ptr;
 *    while(*p) ret+= *p++;
 *    return ret;
 * }
 *
 */
static int
defaultHashFunction(const void* ptr){
   return (int)ptr;
}

typedef struct ListDataPair{
   HashTable* parentHash;
   void* key;
   void* data;
}ListDataPair;

/**
 * each data in chain of hash table is a data pair.
 */
void*  listDataPair_new(HashTable* parent,void* key,const void* data);
int  hashtbl_genkey(const HashTable* hashtbl,unsigned int* iptrKey,const void* key);
int listDataPair_equal(const void* a,const void* b);
List*  hashtbl_find_list_with_key(const HashTable* hashtbl,const void* key);

typedef struct {
   HashTable* hashtbl;
   ListIter listItemIter;
   ListIter listIter;
}HashIterStruct;

#endif
