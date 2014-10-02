#ifndef __CONFIG_H
#define __CONFIG_H
#include <sys/types.h>
typedef struct Config{
   char** section_tags;
   int* section_starts;
   int* section_lens;
   size_t section_size;
   char** keys;
   char** values;
   size_t cnt;
}Config;

Config* config_new(const char* path);
Config* config_new_from_strbuf(const char* strbuf);
size_t config_size(const Config* conf);
int config_empty(const Config* conf);
size_t config_section_size(const Config* conf);
const char* config_section_name(const Config* conf,int section_id);
void config_section_get_region(const Config* conf,int section_id,char*** keys,char*** values,size_t* size);
void config_foreach_in_section(Config* conf,int section_id,void (*fn)(int id,const char* name,const char* key, const char* value,void* arg),void* arg);
void config_foreach_section(Config* conf,void (*fn)(int id,const char* name,const char* key, const char* value,void* arg),void* arg);
void config_delete(Config* conf);

#endif
