#ifndef __HASHTABLE_H
#define __HASHTABLE_H
struct HashMap {
   void** entry;
   int size;
   int ( *hashfnc ) ( const void* );
   int ( *keyEQL ) ( const void*,const void* );
};

struct HashMap* hashmap_new ( int ( *hashfnc ) ( const void* ),int ( *keyEQL ) ( const void*,const void* ) );
void hashmap_put ( struct HashMap* tbl,const void* key,const void* value );
void* hashmap_get ( const struct HashMap* tbl,const void* key );
void* hashmap_remove ( struct HashMap* tbl,const void* key );
int hashmap_size ( const struct HashMap* tbl );
int hashmap_empty ( const struct HashMap* tbl );
int hashmap_contains_key ( const struct HashMap* tbl,const void* key );
void hashmap_clear ( struct HashMap* tbl );
void hashmap_delete ( struct HashMap* tbl );

#endif
