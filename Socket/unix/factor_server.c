#include "unix_sck.h"
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
      if(unixsck_read(p,&request,sizeof(int)) < 0){
         break;
      }
	  if(request == -1) break;
      printf("server get %d\n",request);
      response = factor(request);
      unixsck_write(p,&response,sizeof(int));
	  unixsck_flush(p);//tcp_socket_flush(p);
   }while(1);
   unixsck_delete(p);
}


int run = 1;

void sigterm_handler(int sig){
   run = 0;
}


int main(int argc,char** argv){
   int port;
   UnixSocket* serv;
   if(argc < 2){
	  printf("usage : [prog] port\n");
	  exit(0);
   }
   serv = unixsck_create(1);
   unixsck_set_errhandler(serv,errHandler,NULL);
   signal(SIGTERM,sigterm_handler);
   sscanf(argv[ 1 ],"%d",&port);
   unixsck_bind(serv,port);

   while(run){
	  pid_t pid;
	  UnixSocket* sck_in = unixsck_create(1);
	  pthread_t pth;
	  unixsck_accept(serv,sck_in);
	  if((pid = fork()) == 0){
     	  //pthread_create(&pth,0,thfunc,sck_in);
		  thfunc((void*)sck_in);
          unixsck_delete(serv);
          exit(0);
	  }
	  else if(pid > 0){
		 unixsck_delete(sck_in);
	  }
   }
   unixsck_delete(serv);
   return 0;
}
