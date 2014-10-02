#include <stdio.h>
#include <errno.h>
#include "unix_sck.h"

static void errHandler(const char* fnc,const char* reason,void* param){
   fprintf(stderr,"%s (%s)\n",fnc,reason);
   errno=0;
}
int main(int argc,char** argv){
   int port;
   char req_str[ 64 ]="";
   int request;
   int response;
   sscanf(argv[ 1 ],"%d",&port);
   UnixSocket* sck = unixsck_create(1);
   unixsck_set_errhandler(sck,errHandler,NULL);
   unixsck_connect(sck,port);
   if(unixsck_lasterr(sck)){
      printf("%s\n",unixsck_get_errstr(unixsck_lasterr(sck)));
   }
   while( 1 ){
	  scanf("%s",req_str);
	  if(strncmp(req_str,"exit",64) == 0){
		 request = -1;
		 unixsck_write(sck,&request,sizeof(int));
		 unixsck_flush(sck);//tcp_socket_flush(sck);
		 break;
	  }
	  fseek(stdin,0,SEEK_SET);
	  sscanf(req_str,"%d",&request);
      unixsck_write(sck,&request,sizeof(int));
	  unixsck_flush(sck);//tcp_socket_flush(sck);
	  if(request == -1){
		 printf("get -1, interpreted as exit\n");
		 break;
	  }
	  unixsck_read(sck,&response,sizeof(int));
	  printf("server reply `%d` \n",response);
      if(unixsck_lasterr(sck)){
         printf("%s\n",unixsck_get_errstr(unixsck_lasterr(sck)));
      }

   }
   unixsck_delete(sck);

   return 0;
}
