#ifndef STRING_TOKENIZER_H_
#define STRING_TOKENIZER_H_

/**
 * Author: Sunneo
 * this header provides java-like StringTokenizer class manipulations
 * as below:
 * -----------------------------------------------------------------
 * strtok_new(str,delimString) 
 *   create a StringTokenizer object .
 *   prototype 
 *      StringTokenizer* strtok_new(const char* str,const char* delim);
 *   param str 
 *      string to split with delimiter.
 *   param delimString 
 *      delimiter string.
 *   return  
 *      StringTokenizer object.
 * ------------------------------------------------------------------
 * strtok_has_more(token)
 *   check if there is more string to token.
 *   prototype
 *      int strtok_has_more(const StringTokenizer* token);
 *   param token
 *      StringTokenizer Object
 *   return 
 *      true(1) if there exist more token,otherwise 0.
 * ------------------------------------------------------------------
 * strtok_next(token)
 *   get next token
 *   prototype
 *      const char* strtok_next(StringTokenizer* token);
 *   param token
 *      StringTokenizer Object
 *   return 
 *      the token String.
 * -------------------------------------------------------------------
 * strtok_count(token)
 *   int              strtok_count(StringTokenizer* token);
 *   void             strtok_rewind(StringTokenizer* token);
 *   void             strtok_delete(StringTokenizer* token);
 *
 */
typedef struct {
   char* dupStr;
   const char* delim;
   const char* end;
   char* currentToken;
   char* nextToken;
   int count;
}StringTokenizer;



StringTokenizer* strtok_new(const char* str,const char* delim);
int              strtok_has_more(StringTokenizer* token);
char*            strtok_next(StringTokenizer* token);
int              strtok_count(StringTokenizer* token);
void             strtok_rewind(StringTokenizer* token);
void             strtok_delete(StringTokenizer* token);

#endif
