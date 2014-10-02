#include "sckwrap.h"
//#include "tcp_sck.h"
//#include "unix_sck.h"
#include "sckwrap.inc.h"
// 0 : tcp
// 1 : unix 
static int sckwrap_use = 0;
typedef struct FncMap{
   void* (*server_create)(int buf);
   void  (*server_set_errhandle)(void*,void(*)(const char*,const char*,void*),void*);
   void  (*sck_set_errhandle)(void*,void(*)(const char*,const char*,void*),void*);
   void  (*server_bind)(void*,int);
   void  (*server_listen)(void*,int);
   void  (*server_accept)(void*,void*);
   int   (*get_last_error)(void*);
   void  (*peer_name)(void*,char*,int len);
   const char* (*err_str)(int);
   void  (*server_delete)(void*);
   ssize_t   (*write)(void*,const void*,int);
   ssize_t   (*read)(void*, void*,int);
   void  (*flush)(void*);
   void  (*close)(void*);
   void  (*socket_delete)(void*);
   void*  (*socket_create)(int buf);
   void  (*socket_connect)(void*,const char*,int);
}FncMap;

static void* server_create_tcp(int buf){
   return tcp_server_create(buf);
}
static void server_set_errhandler_tcp(void* h,void(*fnc)(const char*,const char*,void*),void* p){
   tcp_server_set_errhandler((TCPServerSocket*)h,fnc,p);
}
static void sck_set_errhandler_tcp(void* h,void(*fnc)(const char*,const char*,void*),void* p){
   tcp_socket_set_errhandler((TCPSocket*)h,fnc,p);
}

static void  server_bind_tcp(void* s,int p){
   tcp_server_bind((TCPServerSocket*)s,p);
}
static void  server_listen_tcp(void* s,int l){
   tcp_server_listen((TCPServerSocket*)s,l);
}
static void  server_accept_tcp(void* s,void* in){
   tcp_server_accept((TCPServerSocket*)s,(TCPSocket*)in);
}

static int   get_last_error_tcp(void* s){
   return tcp_get_last_error((TCPSocket*)s);
}
static void  peer_name_tcp(void* s,char* b,int len){
   tcp_sck_peername((TCPSocket*)s,b,len);
}

static const char* err_str_tcp(int e){
   return tcp_server_err_string(e);
}
static void  server_delete_tcp(void* s){
   tcp_server_delete((TCPServerSocket*)s);
}

static ssize_t   write_tcp (void* sck,const void* s,int n){   
   return tcp_socket_write((TCPSocket*)sck,s,n);
}
static ssize_t  read_tcp(void* sck, void* d,int n){
   return tcp_socket_read((TCPSocket*)sck,d,n);
}
static void  flush_tcp(void* s){
   tcp_socket_flush((TCPSocket*)s);
}

static void  close_tcp(void* s){
   tcp_socket_close((TCPSocket*)s);
}
static void  socketr_delete_tcp(void* s){
   tcp_socket_delete((TCPSocket*)s);
}
static void* socket_create_tcp(int buf){
   return tcp_socket_create(buf); 
}
static void  socket_connect_tcp(void* s,const char* ip,int p){
   return tcp_socket_connect((TCPSocket*)s,ip,p);
}

static void* server_create_unix(int buf){
   return unixsck_create(buf);
}
static void server_set_errhandler_unix(void* h,void(*fnc)(const char*,const char*,void*),void* p){
   unixsck_set_errhandler((UnixSocket*)h,fnc,p);
}
static void sck_set_errhandler_unix(void* h,void(*fnc)(const char*,const char*,void*),void* p){
   unixsck_set_errhandler((UnixSocket*)h,fnc,p);
}

static void server_bind_unix(void* s,int p){
   unixsck_bind((UnixSocket*)s,p);
}
static void  server_listen_unix(void* s,int l){
   unixsck_listen((UnixSocket*)s,l);
}
static void  server_accept_unix(void* s,void* in){
   unixsck_accept((UnixSocket*)s,(UnixSocket*)in);
}

static int   get_last_error_unix(void* s){
   return unixsck_lasterr((UnixSocket*)s);
}
static void  peer_name_unix(void* s,char* b,int len){
   strncpy(b,"UnixSocket.Peer",len);
}

static const char* err_str_unix(int e){
   return unixsck_get_errstr(e);
}
static void  server_delete_unix(void* s){
   unixsck_delete((UnixSocket*)s);
}
static ssize_t   write_unix (void* sck,const void* s,int n){
   return unixsck_write((UnixSocket*)sck,s,n);
}
static ssize_t  read_unix(void* sck, void* d,int n){
   return unixsck_read((UnixSocket*)sck,d,n);
}
static void  flush_unix(void* s){
   unixsck_flush((UnixSocket*)s);
}

static void  close_unix(void* s){
   unixsck_close((UnixSocket*)s);
}
static void  socket_delete_unix(void* s){
   unixsck_delete((UnixSocket*)s);
}
static void* socket_create_unix(int buf){
   return unixsck_create(buf); 
}
static void  socket_connect_unix(void* s,const char* ip,int p){
    unixsck_connect((UnixSocket*)s,p);
}

static FncMap fncMap[2]={
   {
     server_create_tcp,server_set_errhandler_tcp,sck_set_errhandler_tcp,server_bind_tcp,
     server_listen_tcp,server_accept_tcp,get_last_error_tcp,peer_name_tcp,
     err_str_tcp,server_delete_tcp,
     write_tcp,read_tcp,flush_tcp,close_tcp,
     server_delete_tcp,socket_create_tcp,socket_connect_tcp
   },
   {
     server_create_unix,server_set_errhandler_unix,sck_set_errhandler_unix,server_bind_unix,
     server_listen_unix,server_accept_unix,get_last_error_unix,peer_name_unix,
     err_str_unix,server_delete_unix,
     write_unix,read_unix,flush_unix,close_unix,
     server_delete_unix,socket_create_unix,socket_connect_unix
   }
};
#include <signal.h>
void sck_wrap_init(int use){
   sckwrap_use = use % 2;
   signal(SIGPIPE,SIG_IGN);
}

void* sck_server_create(int buffered){
   return fncMap[sckwrap_use].server_create(buffered);
}
void sck_server_set_errhandler(void* sck,void(*fnc)(const char* fnc,const char* cause,void*),void* param){
   fncMap[sckwrap_use].server_set_errhandle(sck,fnc,param);
}
void sck_socket_set_errhandler(void* sck,void(*fnc)(const char* fnc,const char* cause,void*),void*param){
   fncMap[sckwrap_use].sck_set_errhandle(sck,fnc,param);
}
void sck_server_bind(void* sck,int port){
   fncMap[sckwrap_use].server_bind(sck,port);
}

void sck_server_listen(void* sck,int queueLen){
   fncMap[sckwrap_use].server_listen(sck,queueLen);
}
void sck_server_accept(void* sck,void* sckin){
   fncMap[sckwrap_use].server_accept(sck,sckin);
}
int sck_last_error(const void* sck){
   return fncMap[sckwrap_use].get_last_error((void*)sck);
}
void sck_peername ( void* sck, char* name, int len ){
   fncMap[sckwrap_use].peer_name(sck,name,len);
}
const char* sck_err_string(int err){
   return fncMap[sckwrap_use].err_str(err);
}
void sck_server_delete(void* sck){
   fncMap[sckwrap_use].server_delete(sck);
}

ssize_t sck_write(void* sck,const void* data,int len){
   return fncMap[sckwrap_use].write(sck,data,len);
}
ssize_t sck_read(void* sck,void* data,int len){
   return fncMap[sckwrap_use].read(sck,data,len);
}
void sck_flush(void* sck){
   fncMap[sckwrap_use].flush(sck);
}
void sck_close(void* sck){
   fncMap[sckwrap_use].close(sck);
}
void sck_delete(void* sck){
   fncMap[sckwrap_use].server_delete(sck);
}
void* sck_create(int buffered){
   return fncMap[sckwrap_use].socket_create(buffered);
}
void sck_connect(void* sck,const char* hostaddr,int port){
   fncMap[sckwrap_use].socket_connect(sck,hostaddr,port);
}
