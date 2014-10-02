#ifdef TEST
#include <stdio.h>
#include <stdlib.h>
#include "string_tokenizer.h"
int main(int argc, char *argv[])
{
   StringTokenizer* token = strtok_new("Hello World  , , Good Day!, ...,",", ");
   while(strtok_has_more(token)){
      printf("%s\n",strtok_next(token));
   }  
   strtok_rewind(token);
   strtok_delete(token);
   system("PAUSE");	
   return 0;
}
#endif
