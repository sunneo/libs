#include <stdio.h>
#include "conf.h"

static void each_section_fn(int i,const char* name,const char* key,const char* value, void* arg){
  printf("%s %s:%s\n",name,key,value);
}

int main(){
   Config* conf = config_new("config.conf");
   if(conf){
      int i;
      for(i=0; i<conf->cnt; ++i){
          printf("%s=%s\n",conf->keys[i],conf->values[i]);
      }
   }
   printf("section size:%d\n",config_section_size(conf));
   config_foreach_section(conf,each_section_fn,NULL);
   config_delete(conf);
   conf = config_new_from_strbuf(" aa=bb   vendor=AMD ");
   if(conf){
      int i;
      for(i=0; i<conf->cnt; ++i){
          printf("%s=%s\n",conf->keys[i],conf->values[i]);
      }
   }
   config_delete(conf);

   return 0;
}


