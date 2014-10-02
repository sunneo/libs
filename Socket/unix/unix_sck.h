#ifndef UNIXSCK_INCLUDE_H
#define UNIXSCK_INCLUDE_H
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/syslog.h>
#include <sys/un.h>

typedef struct tagUnixSocket{
   char filename[128];
   struct sockaddr_un addr;
   int lastError;
   int listened;
   int fd;
   struct {
      void (*fnc)(const char* op,const char* reason,void* param);
      void* param;
   }errhandler;
   int buffered;
   FILE* wf,*rf;
}UnixSocket;

UnixSocket* unixsck_create(int buffered);
int         unixsck_bind(UnixSocket* sck,int port);
void        unixsck_set_errhandler(UnixSocket* sck,void(* fnc)(const char*,const char*,void*),void* param);
int         unixsck_listen(UnixSocket* sck,int backlog); // set socket fd here
UnixSocket* unixsck_accept_new(UnixSocket* sck);
void        unixsck_accept(UnixSocket* scks,UnixSocket* sck);
void        unixsck_close(UnixSocket* sck);
int         unixsck_lasterr(UnixSocket* sck);
const char* unixsck_get_errstr(int errcode);
ssize_t     unixsck_write(UnixSocket* sck,const void* data,size_t size);
ssize_t     unixsck_read(UnixSocket* sck,void* data,size_t size);
void        unixsck_delete(UnixSocket* sck);
int         unixsck_connect(UnixSocket* sck,int port);
void        unixsck_flush(UnixSocket* sck);
#endif

