#ifndef LAZY_SOCKET_WRAP_H
#  define LAZY_SOCKET_WRAP_H
#define LAZYWRAP_USE_TCP 0
#define LAZYWRAP_USE_UNIX 1
#include <stdlib.h>
typedef void* Client;
typedef void* Server;
void sck_wrap_init(int use);
void* sck_server_create(int buffered);
void sck_server_set_errhandler(void* sck,void(*fnc)(const char* fnc,const char* cause,void*),void* param);
void sck_socket_set_errhandler(void* sck,void(*fnc)(const char* fnc,const char* cause,void*),void*param);
void sck_server_bind(void* sck,int port);
void sck_server_listen(void* sck,int queueLen);
void sck_server_accept(void* sck,void* sckin);
int sck_last_error(const void* sck);
void sck_peername ( void* sck, char* name, int len );
const char* sck_err_string(int err);
void sck_server_delete(void* sck);
ssize_t sck_write(void* sck,const void* data,int len);
ssize_t sck_read(void* sck,void* data,int len);
void sck_flush(void* sck);
void sck_close(void* sck);
void sck_delete(void* sck);
void* sck_create(int buffered);
void sck_connect(void* sck,const char* hostaddr,int port);
#endif
