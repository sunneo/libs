#include "string_tokenizer.h"
#include <string.h>
#include <stdlib.h>
#ifndef __fastcall
#  ifdef __GNUC
#    define __fastcall __attribute__(( regparm(3) ))
#  else
#    define __fastcall __stdcall
#  endif
#endif

__inline static char* __fastcall
strdup_length(const char* str,int* len){
   char* ret;
   *len = strlen(str) + 1;
   ret = (char*)malloc(*len);
   strncpy(ret,str,*len);
   return ret;
}

StringTokenizer* 
strtok_new(const char* str,const char* delim){
   int len;
   StringTokenizer* token = (StringTokenizer*)malloc(sizeof(StringTokenizer));
   token->nextToken = token->dupStr = strdup_length(str,&len);
   token->end = token->dupStr+ len;
   token->delim = delim;
   token->currentToken = NULL;
   token->count = -1;
   return token;
}

__inline static int 
strtok_test(StringTokenizer* token,char** testCurrent,char** testNext){
   if( token == NULL ) return 0;  
   *testNext = token->nextToken;       /* copy */
   *testCurrent = strtok(*testNext,token->delim); /* get token */  
   *testNext = strchr(*testNext,0) + 1; /* move to the cut position */
   return  (*testCurrent) && (*testNext <= token->end) ;
}

int 
strtok_has_more(StringTokenizer* token){
   char* current;
   char* next;
   return strtok_test(token,&current,&next);
}

char*      
strtok_next(StringTokenizer* token){
   char* current;
   char* next;
   if( !strtok_test(token,&current,&next) ) return NULL;
   token->currentToken = current;
   token->nextToken = next;
   return token->currentToken;
}

void 
strtok_delete(StringTokenizer* token){
   if(!token) return;
   if(token->dupStr) free(token->dupStr);
   token->dupStr = NULL;
   free(token);
}

void 
strtok_rewind(StringTokenizer* token){
   if(token == NULL){
      return;
   }
   token->nextToken = token->dupStr;
   token->currentToken = 0;
}

int strtok_count(StringTokenizer* token){
   if(!token) return 0;
   if( token->count != -1 ) return token->count;
   token->count = 0;
   char* current;
   char* next;
   while (strtok_test(token, &current, &next)) {
       if (next > 1){
           *(next - 1) = (*token->delim);
       }
      strtok_next(token);
      ++token->count;
   }
   if (next > 1){
       *(next - 1) = (*token->delim);
   }
   strtok_rewind(token);
   return token->count;
}
