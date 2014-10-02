#ifndef HASHTABLE_H_
#define HASHTABLE_H_


#ifdef __cplusplus
#  define EXTERN extern "C"
#  define EXTERN_BEGIN EXTERN {
#  define EXTERN_END }
#else
#  define EXTERN extern
#  define EXTERN_BEGIN
#  define EXTERN_END 
#endif
#define HASHTABLE_DEFAULT_SIZE 4096
#define HASHTABLE_MINIMAL_SIZE 128
typedef void* HashIter;
typedef void* HashKeyIter;
typedef struct {
   void* data;
   int size;
   int count;
   int (*hash_function)(const void* data);
   int (*equal_function)(const void* key1,const void* key2);
   void* keyset;
}HashTable;

EXTERN HashTable* hashtbl_create(int size, int(*hashFunction)(const void*),int (*keyMatchFunction)(const void* k1,const void* k2) );

EXTERN int        hashtbl_put(HashTable* hashtbl,const void* key,const void* data);
EXTERN void*      hashtbl_get(HashTable* hashtbl,const void* key);
EXTERN int        hashtbl_empty(const HashTable* hashtbl);
EXTERN int        hashtbl_exist_key(const HashTable* hashtbl,const void* key);
EXTERN int        hashtbl_remove(HashTable* hashtbl,const void* key);
EXTERN void       hashtbl_clear(HashTable* hashtbl);
EXTERN int        hashtbl_count(const HashTable* hashtbl);
EXTERN int        hashtbl_itemcount(const HashTable* hashtbl,const void* key);
EXTERN void       hashtbl_delete(HashTable* hashtbl);
EXTERN int        hashtbl_get_keyset_size(const HashTable* hashtbl);
EXTERN void       hashtbl_get_keyset(const HashTable* hashtbl,void** dstKeySet,int bufsize);
/**
 * iterator for a key. 
 * user could get
 */
EXTERN HashKeyIter   hashtbl_getkeyiter(const HashTable* hashtbl,const void* key);
EXTERN int           hashtbl_keyiter_hasNext(const HashKeyIter keyiter);
EXTERN void*         hashtbl_keyiter_next(HashKeyIter keyiter);
EXTERN void*         hashtbl_keyiter_data(const HashKeyIter keyiter);
EXTERN void          hashtbl_keyiter_delete(HashKeyIter keyiter);

EXTERN HashIter   hashtbl_getiter(const HashTable* hashtbl);
EXTERN void*      hashtbl_iter_data(const HashIter  iter);
EXTERN int        hashtbl_iter_hasNext(const HashIter  iter);
EXTERN void*      hashtbl_iter_next(HashIter  iter);
EXTERN void       hashtbl_iter_delete(HashIter  iter);



#endif
