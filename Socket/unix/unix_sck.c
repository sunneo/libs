#include "unix_sck.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> 
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/un.h>

#define UNIXSCK_ERR_NOERR          0
#define UNIXSCK_ERR_INVALID_PORT   1
#define UNIXSCK_ERR_INVALID_ARG    2
#define UNIXSCK_ERRNO_SET          3
#define UNIXSCK_ERR_MAX 3

const static char* unix_enum_err_str[]={
  "No Error",
  "Invalid Port",
  "Invalid Argument",
  "Errno is set"
};

UnixSocket* unixsck_create(int buf){
   UnixSocket* ret;
   ret = (UnixSocket*)calloc(1,sizeof(UnixSocket));
   ret->buffered = buf;
   return ret;
}
int unixsck_lasterr(UnixSocket* sck){
   if(sck == 0){
      return UNIXSCK_ERR_INVALID_ARG;
   }
   if(sck->lastError == UNIXSCK_ERRNO_SET && errno == 0){
      sck->lastError = 0;
   }
   return sck->lastError;
}
const char* unixsck_get_errstr(int errcode){
   if(errcode < 0 || errcode > UNIXSCK_ERR_MAX){
      return "Invalid Error Code";
   }

   if(errcode == UNIXSCK_ERRNO_SET){
      return strerror(errno);
   }
   return unix_enum_err_str[errcode];
}

void unixsck_set_errhandler(UnixSocket* sck,void(* fnc)(const char*,const char*,void*),void* param){
   if(sck){
      sck->errhandler.fnc = fnc;
      sck->errhandler.param = param;
   }
}

static char* unixsck_get_sckdir(){
   char* ret = getenv("UNIX_SCK_DIR");
   if(!ret){
      ret = (char*)"/tmp";
   }
   return ret;
}

int unixsck_bind(UnixSocket* sck,int port){
   
   int len = 0;
   int ret = 0;
   if(!sck){
      return -1;
   }
   if(port < 0){
       sck->lastError = UNIXSCK_ERR_INVALID_PORT;
       if(sck->errhandler.fnc){
          sck->errhandler.fnc("unixsck_bind(param check)",unixsck_get_errstr(sck->lastError),sck->errhandler.param);
          return -1;
       }
   }
   sck->fd = socket(AF_UNIX,SOCK_STREAM,0);
   if(sck->fd == -1){
      if(errno != 0){
         sck->lastError=UNIXSCK_ERRNO_SET;
         if(sck->errhandler.fnc){
            sck->errhandler.fnc("unixsck_listen::create socket",unixsck_get_errstr(sck->lastError),sck->errhandler.param);
            return -1;
         }
      }
   }
   sck->addr.sun_family = AF_UNIX;
   snprintf(sck->filename,128,"%s/unixsck.port.%d",unixsck_get_sckdir(),port);
   len = strlen(sck->filename)+sizeof(sck->addr.sun_family);
   unlink(sck->filename);
   strcpy(sck->addr.sun_path,sck->filename);
   ret = bind(sck->fd,(struct sockaddr*)&sck->addr,len);
   if(ret == -1){
      if(errno != 0){
         sck->lastError=UNIXSCK_ERRNO_SET;
         if(sck->errhandler.fnc){
            sck->errhandler.fnc("unixsck_listen::bind",unixsck_get_errstr(sck->lastError),sck->errhandler.param);
            return -1;
         }
      }
   }
   return ret;
}

int  unixsck_listen(UnixSocket* sck,int backlog){
   int ret = 0;
   if(!sck){
      return -1;
   }
   if(backlog < 5) backlog = 5;
   if(!sck->listened){
      ret = listen(sck->fd,backlog);
      if(ret == -1){
         if(errno != 0){
            sck->lastError=UNIXSCK_ERRNO_SET;
            if(sck->errhandler.fnc){
               sck->errhandler.fnc("unixsck_listen::listen",unixsck_get_errstr(sck->lastError),sck->errhandler.param);
               return -1;
            }
         }
      }
      sck->listened = 1;
   }
   return ret; 
   
}

UnixSocket* unixsck_accept_new(UnixSocket* sck){
   UnixSocket* insck;
   sck = unixsck_create(sck->buffered);
   if(sck){
      unixsck_accept(sck,insck);
   }
   return sck;
}
void unixsck_accept(UnixSocket* scks,UnixSocket* sck){
   int t = sizeof(struct sockaddr_un);
   if(!scks || !sck){
      return;
   }
   if(!sck->listened){
      unixsck_listen(scks,128);
   }
   sck->fd = accept(scks->fd,(struct sockaddr*)&sck->addr,&t);
   if(sck->fd == -1){
      if(errno != 0){
         scks->lastError=UNIXSCK_ERRNO_SET;
         if(scks->errhandler.fnc){
            scks->errhandler.fnc("unixsck_accept::accept",unixsck_get_errstr(scks->lastError),scks->errhandler.param);
            return;
         }
      }
   }
   if ( sck->buffered )
   {
         if ( !sck->wf )
         {
            sck->wf = fdopen ( sck->fd,"wb" );
            if ( !sck->wf && errno )
            {
               sck->lastError=UNIXSCK_ERRNO_SET;
               if(sck->errhandler.fnc){
                  sck->errhandler.fnc("unixsck_accept::fdopen(wb)",unixsck_get_errstr(sck->lastError),sck->errhandler.param);
               }

            }
         }
         if ( !sck->rf )
         {
            sck->rf = fdopen ( dup ( sck->fd ),"rb" );
            if ( !sck->rf && errno )
            {
               sck->lastError=UNIXSCK_ERRNO_SET;
               if(sck->errhandler.fnc){
                  sck->errhandler.fnc("unixsck_accept::fdopen(rb)",unixsck_get_errstr(sck->lastError),sck->errhandler.param);
               }
            }
         }
    }
}
void unixsck_close(UnixSocket* sck){
   if(!sck){ 
      return;
   }
   if(sck->fd){
      close(sck->fd);
      sck->fd = 0;
      sck->listened = 0;
      sck->lastError = 0;
   }
   if(sck->wf){
      fclose(sck->wf);
      sck->wf = 0;
   }
   if(sck->rf){
      fclose(sck->rf);
      sck->rf = 0;
   }
}
void unixsck_delete(UnixSocket* sck){
   unixsck_close(sck);
   free(sck);
}
ssize_t unixsck_write(UnixSocket* sck,const void* data,size_t size){
   ssize_t ret = 0;
   if(!sck || !data){
      return 0;
   }
   if(sck->buffered && sck->wf){
      ret = fwrite(data,1,size,sck->wf);
   }
   else{
      ret = send(sck->fd,data,size,0);
   }
   if(ret < 0){
       if(errno != 0){
         sck->lastError=UNIXSCK_ERRNO_SET;
         if(sck->errhandler.fnc){
            sck->errhandler.fnc("unixsck_write::send",unixsck_get_errstr(sck->lastError),sck->errhandler.param);
            return;
         }
      }
   }
   return ret;
}
ssize_t unixsck_read(UnixSocket* sck,void* data,size_t size){
   ssize_t ret = 0;
   if(!sck || !data){
      return 0;
   }
   if(sck->buffered && sck->rf){
      ret = fread(data,1,size,sck->rf);
   }
   else{
      ret = recv(sck->fd,data,size,MSG_WAITALL);
   }
   if(ret < 0){
       if(errno != 0){
         sck->lastError=UNIXSCK_ERRNO_SET;
         if(sck->errhandler.fnc){
            sck->errhandler.fnc("unixsck_read::recv",unixsck_get_errstr(sck->lastError),sck->errhandler.param);
            return;
         }
      }
   }
   return ret;
  
}

int unixsck_connect(UnixSocket* sck,int port){
   int len = 0;
   int ret = 0;
   if(!sck || port < 0) return -1;
   sck->fd = socket(AF_UNIX,SOCK_STREAM,0);
   if(sck->fd == -1){
      if(errno != 0){
         sck->lastError=UNIXSCK_ERRNO_SET;
         if(sck->errhandler.fnc){
            sck->errhandler.fnc("unixsck_listen::create socket",unixsck_get_errstr(sck->lastError),sck->errhandler.param);
            return -1;
         }
      }
   }
   sck->addr.sun_family = AF_UNIX;
   snprintf(sck->filename,128,"%s/unixsck.port.%d",unixsck_get_sckdir(),port);
   strcpy(sck->addr.sun_path,sck->filename);

   len = strlen(sck->filename)+sizeof(sck->addr.sun_family);
   ret = connect(sck->fd,(struct sockaddr *)&sck->addr, len);
   if(ret == -1){
       if(errno != 0){
         sck->lastError=UNIXSCK_ERRNO_SET;
         if(sck->errhandler.fnc){
            sck->errhandler.fnc("unixsck_read::recv",unixsck_get_errstr(sck->lastError),sck->errhandler.param);
            return;
         }
      }
   }
   if ( sck->buffered )
   {
         if ( !sck->wf )
         {
            sck->wf = fdopen ( sck->fd,"wb" );
            if ( !sck->wf && errno )
            {
               sck->lastError=UNIXSCK_ERRNO_SET;
               if(sck->errhandler.fnc){
                  sck->errhandler.fnc("unixsck_accept::fdopen(wb)",unixsck_get_errstr(sck->lastError),sck->errhandler.param);
               }

            }
         }
         if ( !sck->rf )
         {
            sck->rf = fdopen ( dup ( sck->fd ),"rb" );
            if ( !sck->rf && errno )
            {
               sck->lastError=UNIXSCK_ERRNO_SET;
               if(sck->errhandler.fnc){
                  sck->errhandler.fnc("unixsck_accept::fdopen(rb)",unixsck_get_errstr(sck->lastError),sck->errhandler.param);
               }
            }
         }
    }

   return ret;
}
void unixsck_flush(UnixSocket* sck){
   if(sck && sck->buffered && sck->wf){ 
      fflush(sck->wf);
   }
}

