#include "sckwrap.h"
#include <sys/types.h>
#include <stdio.h>
#include <signal.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
static int factor(int i){
   int ret = 1;
   while(i >= 2){
	  ret*=i;
	  --i;
   }
   return ret;
}
static void errHandler(const char* fnc,const char* reason,void* param){
   fprintf(stderr,"%s (%s)\n",fnc,reason);
   errno=0;
}

void* thfunc(void* p){
   int request;
   int response;
   do{
      if(sck_read((Client*)p,&request,sizeof(int)) < 0){
         break;
      }
	  if(request == -1) break;
      printf("server get %d\n",request);
      response = factor(request);
      sck_write(p,&response,sizeof(int));
	  sck_flush(p);//tcp_socket_flush(p);
   }while(1);
   sck_delete(p);
}


int run = 1;

void sigterm_handler(int sig){
   run = 0;
}


int main(int argc,char** argv){
   int port;
   sck_wrap_init(1);
   Server* serv;
   if(argc < 2){
	  printf("usage : [prog] port\n");
	  exit(0);
   }
   serv = sck_server_create(1);
   sck_server_set_errhandler(serv,errHandler,NULL);
   signal(SIGTERM,sigterm_handler);
   signal(SIGPIPE,SIG_IGN);
   sscanf(argv[ 1 ],"%d",&port);
   sck_server_bind(serv,port);

   while(run){
	  pid_t pid;
	  Client* sck_in = sck_create(1);
	  pthread_t pth;
	  sck_server_accept(serv,sck_in);
	  if((pid = fork()) == 0){
     	  //pthread_create(&pth,0,thfunc,sck_in);
		  thfunc((void*)sck_in);
          sck_server_delete(serv);
          exit(0);
	  }
	  else if(pid > 0){
		 sck_delete(sck_in);
	  }
   }
   sck_server_delete(serv);
   return 0;
}
