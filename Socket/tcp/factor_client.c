#include <stdio.h>
#include <errno.h>
#include "tcp_sck.h"
#include <string.h>
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
   TCPSocket* sck = tcp_socket_create(1);
   tcp_socket_set_errhandler(sck,errHandler,NULL);
   tcp_socket_connect(sck,"127.0.0.1",port);
   if(tcp_get_last_error(sck)){
      printf("%s\n",tcp_server_err_string(tcp_get_last_error(sck)));
   }
   while( 1 ){
	  scanf("%s",req_str);
	  if(strncmp(req_str,"exit",64) == 0){
		 request = -1;
		 tcp_socket_write(sck,&request,sizeof(int));
		 tcp_socket_flush(sck);
		 break;
	  }
	  fseek(stdin,0,SEEK_SET);
	  sscanf(req_str,"%d",&request);
      tcp_socket_write(sck,&request,sizeof(int));
	  tcp_socket_flush(sck);
	  if(request == -1){
		 printf("get -1, interpreted as exit\n");
		 break;
	  }
	  tcp_socket_read(sck,&response,sizeof(int));
	  printf("server reply `%d` \n",response);
      if(tcp_get_last_error(sck)){
         printf("%s\n",tcp_server_err_string(tcp_get_last_error(sck)));
      }

   }
   tcp_socket_delete(sck);

   return 0;
}
