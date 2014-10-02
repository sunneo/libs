#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "conf.h"
#if DEBUG == 1
#  define DEBUGSYM fprintf(stderr,"%s %d\n",__FUNCTION__,__LINE__);
#else
#  define DEBUGSYM 
#endif
#define MAXTOKSIZ 256
static void* __malloc_debug(size_t s,const char* fn,int line){
   void* r = malloc(s);
   if(!r){
      fprintf(stderr,"internal error, %s:%d (malloc_debug failed)",fn,line);
      exit(errno);
   }
   memset(r,0,s);
   return r;  
}
#define malloc_debug(S) __malloc_debug((S),__FUNCTION__,__LINE__)
typedef struct KeyValue{
   char* key;
   char* value;
   struct KeyValue* next;
}KeyValue;

typedef struct SectionNode{
   size_t start_cnt;
   char* name;
   struct SectionNode* next;
}SectionNode;

static SectionNode* section_node_new(char* name, size_t start);

static KeyValue* keyvalue_node_new(char* k,char* v);

typedef struct ParsedContent{ 
   int isfromFile;
   int pos;
   int bufsize;
   FILE* fp;
   char* buf;
}ParsedContent;

static int parsed_cont_eof(const ParsedContent* p){
   if(p->isfromFile){
      return feof(p->fp);
   }
   else{
      return p->pos>=p->bufsize-1;
   }
}
static int parsed_cont_error(const ParsedContent* p){
   if(p->isfromFile){
      return ferror(p->fp);
   }
   else{
      return p->pos>=p->bufsize-1 || p->buf==0;
   }
}

static int parsed_cont_getc(ParsedContent* p){
   int retv;
   if(p->isfromFile){
      retv = fgetc(p->fp);
   }
   else{
      int ret = -1;
      if(p->pos >= p->bufsize){
          retv = ret;
      }
      else{
         retv = p->buf[p->pos];
         ++p->pos;
      }
   }
   return retv;
}

static KeyValue* keyvalue_node_new(char* k,char* v){
   KeyValue* r = (KeyValue*)malloc_debug(sizeof(KeyValue));
   r->key = k;
   r->value = v;
   r->next = NULL;
   return r;
}
static SectionNode* section_node_new(char* name, size_t start){
   SectionNode* r = (SectionNode*)malloc_debug(sizeof(SectionNode));
   r->name = name; 
   r->start_cnt = start;
   r->next = NULL;
   return r;
}

static void config_skip_line(ParsedContent* fp){
   while(1){
      if(parsed_cont_eof(fp) || parsed_cont_error(fp) || parsed_cont_getc(fp)=='\n' ){
         return;
      }
   }
}
static char config_skip_space(ParsedContent * fp){
   char c;
   while(1){
      if(parsed_cont_eof(fp) || parsed_cont_error(fp)){
         return 0;
      }
      c = parsed_cont_getc(fp);
      if(c != ' ' && c != '\t'){
         return c;
      }
   }
   return 0;
}
static char* config_parse_string(char c,ParsedContent * fp){
   char* buf = (char*)malloc_debug(MAXTOKSIZ);
   int terminated = 0;
   int i = 1;
   buf[0] = c;
   while(!parsed_cont_eof(fp)){
      c = parsed_cont_getc(fp);
      if(c == '"'){
         if(buf[i-1] != '\\'){ // exactly a string terminate
            buf[i] = c;
            buf[i+1]='\0';
            ++i;
            terminated = 1;
            break;
         }
      }
      buf[i]=c;
      ++i;
   }
   buf[i]=0;
   if(!terminated){
      fprintf(stderr,"warning:string not terminated:`%s`\n",buf);
   }
   return buf;
}
static int config_char_isIdent(char c,int start){
   if(start) return isalpha(c) || c=='_';
   else return  isalpha(c) || c=='_' || isdigit(c);
}

static int config_char_isdigit(char c){
   return isdigit(c);
}

static char config_parse_digit(char** bufout,char first,ParsedContent * fp){
   char c;
   int idx = 1;
   int isTerminated = 0;
   char* buf = (char*)malloc_debug(MAXTOKSIZ);
   buf[0] = first;
   while(!parsed_cont_eof(fp)){
      c = parsed_cont_getc(fp);
      if(config_char_isdigit(c)){
         buf[idx] = c;
         ++idx;
      }
      else{
         break;
      }
   }
   *bufout = buf;
   buf[idx]='\0';
   return c;

}


static char config_parse_ident(char** bufout,char first,ParsedContent * fp){
   char c;
   int idx = 1;
   int isTerminated = 0;
   char* buf = (char*)malloc_debug(MAXTOKSIZ);
   buf[0] = first;
   while(!parsed_cont_eof(fp)){
      c = parsed_cont_getc(fp);
      if(config_char_isIdent(c,0)){
         buf[idx] = c;
         ++idx;
      }
      else{
         break;
      }
   }
   *bufout = buf;
   buf[idx]='\0';
   return c;

}

typedef struct KeyValueList{
   KeyValue* head;
   KeyValue* tail;
   size_t cnt; 
}KeyValueList;

typedef struct SectionList{
   SectionNode* head;
   SectionNode* tail;
   size_t cnt;
}SectionList;

typedef char* cstr;
static void keyvalue_list_init(KeyValueList* list){
   list->head = NULL;
   list->tail = NULL;
   list->cnt = 0;
}

static void section_list_init(SectionList* slist){
   slist->head = NULL;
   slist->tail = NULL;
   slist->cnt = 0;
}

static void keyvalue_list_add(KeyValueList* list,char* key, char* value){
   if(list->head == NULL){
      list->head = keyvalue_node_new(key,value);
      list->tail = list->head;
   }
   else{
      list->tail->next =  keyvalue_node_new(key,value);
      list->tail = list->tail->next;
   }
   ++list->cnt;
}
static void section_list_add(SectionList* l, SectionNode* s){
   if(l->head == NULL){
      l->head = s;
      l->tail = l->head;
   }
   else{
      l->tail->next = s;
      l->tail = l->tail->next;
   }
   ++l->cnt; 
}
static int keyvalue_list_to_config_pairs(KeyValueList* list,Config* conf){
    if(list->cnt > 0){
      int i;
      KeyValue* p = list->head;
      conf->keys = (char**)malloc_debug(sizeof(char*)*list->cnt);
      conf->values = (char**)malloc_debug(sizeof(char*)*list->cnt);
      conf->cnt = list->cnt;
      
      for(i=0; i<list->cnt; ++i){
         KeyValue* pdel = p;
         if(p == NULL){
            continue;
         }
         conf->keys[i] = p->key;
         conf->values[i] = p->value;
         p = p->next;
         free(pdel);
      }
      return 1;
   }
   return 0;
  
}
static void section_list_conv_to_config(Config* conf,SectionList* list){
   SectionNode* p;
   SectionNode* pdel;
   int i = 0;
   if(list->cnt == 0){
      return;
   }
   conf->section_tags = (char**)malloc_debug(sizeof(char*)*list->cnt);
   conf->section_starts = (int*)malloc_debug(sizeof(int)*list->cnt);
   conf->section_lens = (int*)malloc_debug(sizeof(int)*list->cnt);
   conf->section_size = list->cnt;
   p=list->head;
   while(p!=NULL){
     pdel = p;
     conf->section_tags[i] = p->name;
     conf->section_starts[i]=p->start_cnt;
     if(p->next){
       conf->section_lens[i] =  p->next->start_cnt - conf->section_starts[i];
     }
     else{
       conf->section_lens[i] =  conf->cnt - conf->section_starts[i];
     }
     ++i;
     p=p->next;
     free(pdel);
   }
}

static void config_parse_section(SectionList* list,KeyValueList* kvlist,char shead, ParsedContent* fp){
   char* buf; 
   char c;
   int i=1;
   buf = (char*)malloc_debug(MAXTOKSIZ);
   buf[0] = shead;
   while(!parsed_cont_eof(fp)){
      c = parsed_cont_getc(fp);
      if(c == ']'){
         buf[i] = c;
         ++i;
         break;
      }
      buf[i] = c;
      ++i;
   }
   buf[i] = '\0';
   section_list_add(list,section_node_new(buf,kvlist->cnt));
   
}
static void config_parse_file(Config* conf,ParsedContent* fp){
   char c = 0;
   int use_pp = 0;
   cstr str[2]={0,0};
   int idx = 0;
   int hasAssign = 0;
   KeyValueList list;
   SectionList sections;
   keyvalue_list_init(&list);
   section_list_init(&sections);
DEBUGSYM
   while(!parsed_cont_eof(fp)){
      if(!use_pp){
         c = config_skip_space(fp);
      }
      use_pp = 0;
      if(c == '"'){
         str[idx] = config_parse_string(c,fp);
         idx+=1;
         if(idx == 2){
            if(!hasAssign){
               fprintf(stderr,"not known what to do with 2 string without assign op\n"); 
               fprintf(stderr,"assume an assign op(%s=%s)\n",str[0],str[1]);
            }
            keyvalue_list_add(&list,str[0],str[1]);
            idx = 0;
            hasAssign = 0;
         }
         continue;
      }
      if(c == '['){
         config_parse_section(&sections,&list,c,fp);
         continue;
      }
      if(c == '#'){      config_skip_line(fp);       continue;     }
      if(config_char_isIdent(c,1)){
         c = config_parse_ident(&str[idx],c,fp);
         ++idx;
         if(idx == 2){
            if(!hasAssign){
               fprintf(stderr,"not known what to do with 2 string without assign op\n"); 
               fprintf(stderr,"assume an assign op(%s=%s)\n",str[0],str[1]);
            }
            keyvalue_list_add(&list,str[0],str[1]);
            idx = 0;
            hasAssign = 0;
         }
         use_pp = 1;
         continue;
      }
      if (config_char_isdigit(c)){
         c = config_parse_digit(&str[idx],c,fp);
         ++idx;
         if(idx == 2){
            if(!hasAssign){
               fprintf(stderr,"not known what to do with 2 string without assign op\n"); 
               fprintf(stderr,"assume an assign op(%s=%s)\n",str[0],str[1]);
            }
            keyvalue_list_add(&list,str[0],str[1]);
            idx = 0;
            hasAssign = 0;
         }
         use_pp = 1;
         continue;
      } 
      if( c =='=' && idx == 1){      hasAssign = 1;     }
   }
DEBUGSYM
   if(!keyvalue_list_to_config_pairs(&list,conf)){
      if(str[0] != NULL){
         free(str[0]);
      }
   }
   section_list_conv_to_config(conf,&sections);
DEBUGSYM
}

Config* config_new_from_strbuf(const char* strbuf){
   Config* conf;
   ParsedContent parsed;
   memset(&parsed,0,sizeof(ParsedContent));

   parsed.fp = NULL;
   parsed.isfromFile = 0;
   parsed.pos = 0;
   parsed.buf = (char*) strbuf;
   parsed.bufsize = strlen(strbuf)+1; 
DEBUGSYM
   conf = (Config*)malloc_debug(sizeof(Config));
   
   conf->cnt = 0;
   conf->keys = 0;
   conf->values= 0;
   config_parse_file(conf,&parsed);
DEBUGSYM
   return conf;
  
}

Config* config_new(const char* path){
   FILE* fp;
   Config* conf;
   ParsedContent parsed;
   memset(&parsed,0,sizeof(ParsedContent));
DEBUGSYM
   fp = fopen(path,"r");
   if(!fp){
      
      return NULL;
   }
   parsed.fp = fp;
   parsed.pos = 0;
   parsed.bufsize = 0; 
   parsed.isfromFile = 1;

   conf = (Config*)malloc_debug(sizeof(Config));
   if(!conf){
      perror("internal error, config_parse_file(malloc_debug failed)");
      exit(errno);
   }

   conf->cnt = 0;
   conf->keys = 0;
   conf->values= 0;
   config_parse_file(conf,&parsed);
   fclose(fp);
DEBUGSYM
   return conf;
}
size_t config_size(const Config* conf){
DEBUGSYM
   if(conf == NULL){
      return 0;
   }
DEBUGSYM
   return conf->cnt;
}
int config_empty(const Config* conf){
DEBUGSYM
   if(conf == NULL){
      return 1;
   }
DEBUGSYM
   return conf->cnt == 0;
}
void config_delete(Config* conf){
   if(conf == NULL){
      return;
   }
DEBUGSYM
   if(!config_empty(conf)){
      int i;
      for(i=0; i<conf->cnt; ++i){
         if(conf->keys[i]){
            free(conf->keys[i]);
         }
         if(conf->values[i]){
            free(conf->values[i]);
         }
      }
   }
   if(conf->section_size > 0){
      int i;
      for(i=0; i<conf->section_size; ++i){
         free(conf->section_tags[i]);
      }
      free(conf->section_starts);
      free(conf->section_lens);
   }
   if(conf !=NULL){ 
       free(conf);
   }
DEBUGSYM
}


size_t config_section_size(const Config* conf){
   if(conf == NULL) return 0;
   return conf->section_size;
}
const char* config_section_name(const Config* conf,int section_id){
   if(conf==NULL) return NULL;
   return conf->section_tags[section_id];
}
void config_section_get_region(const Config* conf,int section_id,char*** keys,char*** values,size_t* size){
   int start_pt;
   if(conf == NULL){
       *keys = NULL;
       *values = NULL;
       *size = 0;
       return;
   }
   start_pt = conf->section_starts[section_id];
   *size = conf->section_lens[section_id];
   *keys = &conf->keys[start_pt];
   *values = &conf->values[start_pt];
}

void config_foreach_in_section(Config* conf,int section_id,void (*fn)(int id,const char* name,const char* key, const char* value,void* arg),void* arg){
   int i;
   size_t sz;
   char** keys;
   char** values;
   const char* name;
   if(!fn){
      return;
   }
   name = config_section_name(conf,section_id);
   config_section_get_region(conf,section_id,&keys,&values,&sz);
   for(i=0; i<sz; ++i){
      fn(section_id,name,keys[i],values[i],arg);
   }
}

void config_foreach_section(Config* conf,void (*fn)(int id,const char* name,const char* key, const char* value,void* arg),void* arg){
   int sz = config_section_size(conf);
   int i;
   if(!fn){
      return;
   }
   if(sz <= 0){
      return;
   }
   for(i=0; i<sz; ++i){         
      int secsz;
      char** keys;
      char** values;
      const char* name;
      int j;
      name = config_section_name(conf,i);
      config_section_get_region(conf,i,&keys,&values,&secsz);
      for(j=0; j<secsz; ++j){
         fn(i,name,keys[j],values[j],arg);
      }
   }
}

