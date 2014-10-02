#ifndef TCPSOCKET_INCLUDE_H
#  define TCPSOCKET_INCLUDE_H
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
typedef struct tagTCPSocket{
   int fd;
   int buffered;
   FILE* rf;
   FILE* wf;
   int lastError;
   struct sockaddr_in addr;
   struct {
      void(*fnc)(const char* fncName,const char* cause,void*);
      void* param;
   }ErrorHandler;
}TCPSocket;

typedef struct tagTCPServerSocket{
   TCPSocket super;
   int listenLen;
}TCPServerSocket;

TCPServerSocket *tcp_server_create(int buffered);
void tcp_server_set_errhandler(TCPServerSocket* sck,void(*fnc)(const char* fnc,const char* cause,void*),void* param);
void tcp_socket_set_errhandler(TCPSocket* sck,void(*fnc)(const char* fnc,const char* cause,void*),void*param);
void tcp_server_bind(TCPServerSocket* sck,int port);
void tcp_server_listen(TCPServerSocket* sck,int queueLen);
void tcp_server_accept(TCPServerSocket* sck,TCPSocket* sckin);
int  tcp_get_last_error(const TCPSocket* sck);
void tcp_sck_peername ( TCPSocket* sck, char* name, int len );
const char* tcp_server_err_string(int err);
void tcp_server_delete(TCPServerSocket* sck);
int tcp_socket_write(TCPSocket* sck,const void* data,int len);
int tcp_socket_read(TCPSocket* sck,void* data,int len);
void tcp_socket_flush(TCPSocket* sck);
void tcp_socket_close(TCPSocket* sck);
void tcp_socket_delete(TCPSocket* sck);

TCPSocket* tcp_socket_create(int buffered);
void tcp_socket_connect(TCPSocket* sck,const char* hostaddr,int port);
#endif
