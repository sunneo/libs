#ifndef CSTRING_LIB_H_
#define CSTRING_LIB_H_

#  ifndef StringCall
#    define StringCall 
#  endif


typedef struct String{
   char* a;
   unsigned length,capacity;
}String;

String* StringCall string_create(const char* str);
unsigned   StringCall string_length(const String* str);
int     StringCall string_empty(const String* str);
char*   StringCall string_buffer(String* str);
char*   StringCall string_begin(const String* str);
char*   StringCall string_end(const String* str);
String* StringCall string_copy(String* dst,const String* src);
String* StringCall string_copy_cstr(String* dst,const char* src);
void    StringCall string_clear(String* str);
String* StringCall string_append(String* dst,const String* src);
String* StringCall string_append_cstr(String* dst,const char* src);
String* StringCall string_append_format(String* dst,const char* fmt,...);
String* StringCall string_append_char(String* dst,char c);
void    StringCall string_delete(String* str);
char*   StringCall string_findfirst(const String* str,int idx_token,const char* token);
int     StringCall string_indexof_char(const String* str, int idx_token, char c);
int     StringCall string_indexof_string(const String* str, int idx_token, const char* token);

#endif
