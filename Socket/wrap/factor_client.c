#include <stdio.h>
#include <errno.h>
#include "sckwrap.h"

static void errHandler(const char* fnc,const char* reason,void* param){
   fprintf(stderr,"%s (%s)\n",fnc,reason);
   errno=0;
}
int main(int argc,char** argv){
   int port;
   char req_str[ 64 ]="";
   int request;
   int response;
   sck_wrap_init(1);
   sscanf(argv[ 1 ],"%d",&port);
   Client* sck = sck_create(1);
   sck_socket_set_errhandler(sck,errHandler,NULL);
   sck_connect(sck,"127.0.0.1",port);
   if(sck_last_error(sck)){
      printf("%s\n",sck_err_string(sck_last_error(sck)));
   }
   while( 1 ){
	  scanf("%s",req_str);
	  if(strncmp(req_str,"exit",64) == 0){
		 request = -1;
		 sck_write(sck,&request,sizeof(int));
		 sck_flush(sck);//tcp_socket_flush(sck);
		 break;
	  }
	  fseek(stdin,0,SEEK_SET);
	  sscanf(req_str,"%d",&request);
      sck_write(sck,&request,sizeof(int));
	  sck_flush(sck);//tcp_socket_flush(sck);
	  if(request == -1){
		 printf("get -1, interpreted as exit\n");
		 break;
	  }
	  sck_read(sck,&response,sizeof(int));
	  printf("server reply `%d` \n",response);
      if(sck_last_error(sck)){
         printf("%s\n",sck_err_string(sck_last_error(sck)));
      }

   }
   sck_delete(sck);

   return 0;
}
