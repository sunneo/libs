#ifndef __HASHTABLE_H
#define __HASHTABLE_H
struct Hashtable{
   void** entry;
   int size;
   unsigned int (*hashfnc)(const void*);
   int (*keyEQL)(const void*,const void*);
};

struct Hashtable* hashtbl_new( unsigned int (*hashfnc)(const void*),int (*keyEQL)(const void*,const void*) );
void hashtbl_put(struct Hashtable* tbl,const void* key,const void* value);
void* hashtbl_get(const struct Hashtable* tbl,const void* key);
void* hashtbl_remove(struct Hashtable* tbl,const void* key);
int hashtbl_size(const struct Hashtable* tbl);
int hashtbl_empty(const struct Hashtable* tbl);
int hashtbl_contains_key(const struct Hashtable* tbl,const void* key);
void hashtbl_clear(struct Hashtable* tbl);
void hashtbl_delete(struct Hashtable* tbl);

#endif
