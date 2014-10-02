#ifndef LAZYWRAP_TCPSOCKET_INCLUDE_H
#  define LAZYWRAP_TCPSOCKET_INCLUDE_H
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

static TCPServerSocket *tcp_server_create(int buffered);
static void tcp_server_set_errhandler(TCPServerSocket* sck,void(*fnc)(const char* fnc,const char* cause,void*),void* param);
static void tcp_socket_set_errhandler(TCPSocket* sck,void(*fnc)(const char* fnc,const char* cause,void*),void*param);
static void tcp_server_bind(TCPServerSocket* sck,int port);
static void tcp_server_listen(TCPServerSocket* sck,int queueLen);
static void tcp_server_accept(TCPServerSocket* sck,TCPSocket* sckin);
static int  tcp_get_last_error(const TCPSocket* sck);
static void tcp_sck_peername ( TCPSocket* sck, char* name, int len );
static const char* tcp_server_err_string(int err);
static void tcp_server_delete(TCPServerSocket* sck);
static ssize_t tcp_socket_write(TCPSocket* sck,const void* data,int len);
static ssize_t tcp_socket_read(TCPSocket* sck,void* data,int len);
static void tcp_socket_flush(TCPSocket* sck);
static void tcp_socket_close(TCPSocket* sck);
static void tcp_socket_delete(TCPSocket* sck);

static TCPSocket* tcp_socket_create(int buffered);
static void tcp_socket_connect(TCPSocket* sck,const char* hostaddr,int port);
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>
#define TCPDEFAULT_LISTEN 64
enum TCPSERERR
{
   TCPSERERR_NOERR,
   TCPSERERR_NULL_POINTER,
   TCPSERERR_INVALID_STREAM,
   TCPSERERR_NOPORT,
   TCPSERERR_CONNECT_REFUSE,
   TCPSERERR_ERRNO_IS_SET,
   TCPSERERR_MAX
};
static int isLocalIP ( const char* addr );
static const char* private_tcpsererr_strings[] =
{
   "NO Error",
   "NullPointerError",
   "Invalid Stream Descriptor",
   "No port",
   "Connect Refused",
   "Internal Error Occurred",
   "Unknown Error"
};
static void tcp_sck_set_lasterr ( TCPSocket* sck,int err );
static void tcp_sck_set_buffered ( TCPSocket* sck,int bBuffered )
{
   sck->buffered = bBuffered;
}

static TCPServerSocket *tcp_server_create ( int buffered )
{
   TCPServerSocket* ret =  ( TCPServerSocket* ) calloc ( 1,sizeof ( TCPServerSocket ) );
   tcp_sck_set_buffered ( ( TCPSocket* ) ret,buffered );
   return ret;
}

static void tcp_sck_set_rawport ( TCPSocket* sck,int port )
{
   sck->addr.sin_port = port;
}

__inline static int
isIPAddr ( const char* str )
{
   if ( strlen ( str ) > 15 ) return 0;
   while ( !isdigit ( *str++ ) )
   {
      return 0;
   }
   return 1;
}

static void tcp_server_set_errhandler ( TCPServerSocket* sck,void ( *fnc ) ( const char*,const char*,void* ),void* param )
{
   tcp_socket_set_errhandler ( ( TCPSocket* ) sck,fnc,param );
}
static void tcp_socket_set_errhandler ( TCPSocket* sck,void ( *fnc ) ( const char*,const char*,void* ),void*param )
{
   if ( sck )
   {
      ( ( TCPSocket* ) sck )->ErrorHandler.fnc = fnc;
      ( ( TCPSocket* ) sck )->ErrorHandler.param = param;
   }
}

static void tcp_socket_flush ( TCPSocket* sck )
{
   if ( sck->wf )
   {
      if ( fflush ( sck->wf ) < 0 && errno )
      {
         tcp_sck_set_lasterr ( ( TCPSocket* ) sck,TCPSERERR_ERRNO_IS_SET );
         if ( ( ( TCPSocket* ) sck )->ErrorHandler.fnc )
         {
            ( ( TCPSocket* ) sck )->ErrorHandler.fnc ( "tcp_socket_flush",tcp_server_err_string ( sck->lastError ), ( ( TCPSocket* ) sck )->ErrorHandler.param );
         }
      }
   }
}
static void tcp_setup_socket_fd ( TCPSocket* sck )
{
   sck->fd = socket ( AF_INET,SOCK_STREAM,0 );
   sck->addr.sin_family=AF_INET;
}

static FILE* tcp_sck_get_wfile ( const TCPSocket* sck )
{
   return sck->wf;
}

static FILE* tcp_sck_get_rfile ( const TCPSocket* sck )
{
   return sck->rf;
}


static int tcp_sck_fd ( const TCPSocket* sck )
{
   return sck->fd;
}

static struct sockaddr_in* tcp_sck_addr ( TCPSocket* sck )
{
   return &sck->addr;
}

static void tcp_sck_set_rawaddr ( TCPSocket* sck,int addr )
{
   sck->addr.sin_addr.s_addr = addr;
}

static void tcp_sck_set_lasterr ( TCPSocket* sck,int err )
{
   sck->lastError = err;
}

static int tcp_sck_get_lasterr ( const TCPSocket* sck )
{
   return sck->lastError;
}

static unsigned short tcp_sck_get_rawport ( const TCPSocket* sck )
{
   return sck->addr.sin_port;
}

static int tcp_sck_isbuffered ( const TCPSocket* sck )
{
   return sck->buffered;
}

static void tcp_server_bind ( TCPServerSocket* sck,int port )
{
   const static int flag = 1;
   if ( !sck ) return;
   if ( !port )
   {
      tcp_sck_set_lasterr ( ( TCPSocket* ) sck,TCPSERERR_NOPORT );
      return;
   }
   else
   {
      tcp_sck_set_lasterr ( ( TCPSocket* ) sck,TCPSERERR_NOERR );
   }
   tcp_setup_socket_fd ( ( TCPSocket* ) sck );
   tcp_sck_set_rawport ( ( TCPSocket* ) sck,htons ( port ) );
   tcp_sck_set_rawaddr ( ( TCPSocket* ) sck,htonl ( INADDR_ANY ) );
   if ( setsockopt ( tcp_sck_fd ( ( TCPSocket* ) sck ) , SOL_SOCKET, SO_REUSEADDR, &flag,sizeof ( int ) ) < 0 )
   {
      if ( errno && ( ( TCPSocket* ) sck )->ErrorHandler.fnc )
      {
         tcp_sck_set_lasterr ( ( TCPSocket* ) sck,TCPSERERR_ERRNO_IS_SET );
         ( ( TCPSocket* ) sck )->ErrorHandler.fnc ( "tcp_server_bind::setsockopt(SO_REUSEADDR)",tcp_server_err_string ( ( ( TCPSocket* ) sck )->lastError ), ( ( TCPSocket* ) sck )->ErrorHandler.param );
      }

   }
   if ( bind ( tcp_sck_fd ( ( TCPSocket* ) sck ), ( struct sockaddr* ) tcp_sck_addr ( ( TCPSocket* ) sck ),sizeof ( struct sockaddr ) ) < 0 )
   {
      if ( errno && ( ( TCPSocket* ) sck )->ErrorHandler.fnc )
      {
         tcp_sck_set_lasterr ( ( TCPSocket* ) sck,TCPSERERR_ERRNO_IS_SET );
         ( ( TCPSocket* ) sck )->ErrorHandler.fnc ( "tcp_server_bind::bind",tcp_server_err_string ( ( ( TCPSocket* ) sck )->lastError ), ( ( TCPSocket* ) sck )->ErrorHandler.param );
      }
   }
}

static void tcp_server_listen ( TCPServerSocket* sck,int queueLen )
{
   if ( !sck ) return;
   if ( tcp_sck_get_lasterr ( ( const TCPSocket* ) sck ) != 0 ) return;
   if ( sck->listenLen > 0 )
   {
      return;
   }
   sck->listenLen = queueLen;
   if ( listen ( tcp_sck_fd ( ( TCPSocket* ) sck ),queueLen ) < 0 )
   {
      if ( errno && ( ( TCPSocket* ) sck )->ErrorHandler.fnc )
      {
         tcp_sck_set_lasterr ( ( TCPSocket* ) sck,TCPSERERR_ERRNO_IS_SET );
         ( ( TCPSocket* ) sck )->ErrorHandler.fnc ( "tcp_server_listen::listen",tcp_server_err_string ( ( ( TCPSocket* ) sck )->lastError ), ( ( TCPSocket* ) sck )->ErrorHandler.param );

      }
   }
}
static void tcp_sck_peername ( TCPSocket* sck, char* name, int len )
{
   struct sockaddr_storage addr;
   char ipstr[INET6_ADDRSTRLEN];
   int port;
   socklen_t  peer_len = sizeof ( addr );
   if ( -1 == getpeername ( tcp_sck_fd ( ( TCPSocket* ) sck ), ( struct sockaddr* ) &addr, &peer_len ) )
   {
      if ( errno && ( ( TCPSocket* ) sck )->ErrorHandler.fnc )
      {
         tcp_sck_set_lasterr ( ( TCPSocket* ) sck,TCPSERERR_ERRNO_IS_SET );
         ( ( TCPSocket* ) sck )->ErrorHandler.fnc ( "tcp_sck_peername::getpeername",tcp_server_err_string ( ( ( TCPSocket* ) sck )->lastError ), ( ( TCPSocket* ) sck )->ErrorHandler.param );
      }
   }
   if ( addr.ss_family == AF_INET )
   {
      struct sockaddr_in *s = ( struct sockaddr_in * ) &addr;
      port = ntohs ( s->sin_port );
      if ( 0==inet_ntop ( AF_INET, &s->sin_addr, ipstr, sizeof ipstr ) )
      {
         if ( errno && ( ( TCPSocket* ) sck )->ErrorHandler.fnc )
         {
            tcp_sck_set_lasterr ( ( TCPSocket* ) sck,TCPSERERR_ERRNO_IS_SET );
            ( ( TCPSocket* ) sck )->ErrorHandler.fnc ( "tcp_sck_peername::inet_ntop(ipv4)",tcp_server_err_string ( ( ( TCPSocket* ) sck )->lastError ), ( ( TCPSocket* ) sck )->ErrorHandler.param );
         }
      }
   }
   else     // AF_INET6
   {
      struct sockaddr_in6 *s = ( struct sockaddr_in6 * ) &addr;
      port = ntohs ( s->sin6_port );
      if ( 0==inet_ntop ( AF_INET6, &s->sin6_addr, ipstr, sizeof ipstr ) )
      {
         if ( errno && ( ( TCPSocket* ) sck )->ErrorHandler.fnc )
         {
            tcp_sck_set_lasterr ( ( TCPSocket* ) sck,TCPSERERR_ERRNO_IS_SET );
            ( ( TCPSocket* ) sck )->ErrorHandler.fnc ( "tcp_sck_peername::inet_ntop(ipv6)",tcp_server_err_string ( ( ( TCPSocket* ) sck )->lastError ), ( ( TCPSocket* ) sck )->ErrorHandler.param );
         }
      }
   }
   strncpy ( name, ipstr, len );
}

static void tcp_server_accept ( TCPServerSocket* sck,TCPSocket* sckin )
{
   int isbuffered;
   static int len = sizeof ( struct sockaddr );
   if ( !sck ) return;
   tcp_server_listen ( sck,TCPDEFAULT_LISTEN );
   if ( tcp_sck_get_lasterr ( ( const TCPSocket* ) sck ) != 0 ) return;
   if ( !sckin )
   {
      tcp_sck_set_lasterr ( ( TCPSocket* ) sck, TCPSERERR_NULL_POINTER );
      return;
   }
   else
   {
      tcp_sck_set_lasterr ( ( TCPSocket* ) sck,TCPSERERR_NOERR );
   }
   if ( sckin->lastError )
   {
      return;
   }

   sckin->fd = accept ( tcp_sck_fd ( ( const TCPSocket* ) sck ), ( struct sockaddr* ) tcp_sck_addr ( ( TCPSocket* ) sck ),&len );
   if ( sckin->fd < 0 && errno )
   {
      if ( !sckin->wf && errno )
      {
         tcp_sck_set_lasterr ( ( TCPSocket* ) sck,TCPSERERR_ERRNO_IS_SET );
         if ( ( ( TCPSocket* ) sck )->ErrorHandler.fnc )
         {
            tcp_sck_set_lasterr ( ( TCPSocket* ) sck,TCPSERERR_ERRNO_IS_SET );
            ( ( TCPSocket* ) sck )->ErrorHandler.fnc ( "tcp_server_accept::accept",tcp_server_err_string ( ( ( TCPSocket* ) sck )->lastError ), ( ( TCPSocket* ) sck )->ErrorHandler.param );
         }
      }
   }
   else
   {
      char* peerName;
      peerName = ( char* ) malloc ( sizeof ( char ) *256 );
      tcp_sck_peername ( sckin,peerName,256 );
      if ( isLocalIP ( peerName ) )
      {
         int one = 1;
         if ( setsockopt ( sckin->fd,SOL_TCP,TCP_NODELAY,&one,sizeof ( int ) ) < 0 )
         {
            if ( errno && ( ( TCPSocket* ) sck )->ErrorHandler.fnc )
            {
               tcp_sck_set_lasterr ( ( TCPSocket* ) sck,TCPSERERR_ERRNO_IS_SET );
               ( ( TCPSocket* ) sck )->ErrorHandler.fnc ( "tcp_server_accept::setsockopt(TCP_NODELAY)",tcp_server_err_string ( ( ( TCPSocket* ) sck )->lastError ), ( ( TCPSocket* ) sck )->ErrorHandler.param );
            }
         }
         /*if ( setsockopt ( sckin->fd,SOL_TCP,TCP_CORK,&one,sizeof ( int ) ) < 0 )
         {
            if ( errno && ( ( TCPSocket* ) sck )->ErrorHandler.fnc )
            {
               tcp_sck_set_lasterr ( ( TCPSocket* ) sck,TCPSERERR_ERRNO_IS_SET );
               ( ( TCPSocket* ) sck )->ErrorHandler.fnc ( "tcp_server_accept::setsockopt(TCP_CORK)",tcp_server_err_string ( ( ( TCPSocket* ) sck )->lastError ), ( ( TCPSocket* ) sck )->ErrorHandler.param );
            }
         }*/
      }
      free ( peerName );
      isbuffered =  tcp_sck_isbuffered ( ( TCPSocket* ) sck );
      if ( isbuffered )
      {
         sckin->buffered = isbuffered;
         if ( !sckin->wf )
         {
            sckin->wf = fdopen ( tcp_sck_fd ( sckin ),"wb" );
            if ( !sckin->wf && errno )
            {
               tcp_sck_set_lasterr ( ( TCPSocket* ) sck,TCPSERERR_ERRNO_IS_SET );
               if ( ( ( TCPSocket* ) sck )->ErrorHandler.fnc )
               {
                  tcp_sck_set_lasterr ( ( TCPSocket* ) sck,TCPSERERR_ERRNO_IS_SET );
                  ( ( TCPSocket* ) sck )->ErrorHandler.fnc ( "tcp_server_accept::fdopen(write buffer)",tcp_server_err_string ( ( ( TCPSocket* ) sck )->lastError ), ( ( TCPSocket* ) sck )->ErrorHandler.param );

               }
            }
         }
         if ( !sckin->rf )
         {
            sckin->rf = fdopen ( dup ( tcp_sck_fd ( sckin ) ),"rb" );
            if ( !sckin->rf && errno )
            {
               tcp_sck_set_lasterr ( ( TCPSocket* ) sck,TCPSERERR_ERRNO_IS_SET );
               if ( ( ( TCPSocket* ) sck )->ErrorHandler.fnc )
               {
                  tcp_sck_set_lasterr ( ( TCPSocket* ) sck,TCPSERERR_ERRNO_IS_SET );
                  ( ( TCPSocket* ) sck )->ErrorHandler.fnc ( "tcp_server_accept::fdopen(read buffer)",tcp_server_err_string ( ( ( TCPSocket* ) sck )->lastError ), ( ( TCPSocket* ) sck )->ErrorHandler.param );

               }
            }
         }
      }
   }

}

static int  tcp_get_last_error ( const TCPSocket* sck )
{
   if ( !sck ) return TCPSERERR_NULL_POINTER;
   return tcp_sck_get_lasterr ( sck );
}

static const char* tcp_server_err_string ( int err )
{
   if ( err > TCPSERERR_MAX )
      err = TCPSERERR_MAX;
   if ( err == TCPSERERR_ERRNO_IS_SET )
   {
      return strerror ( err );
   }
   return private_tcpsererr_strings[ err ];
}

static ssize_t tcp_socket_write ( TCPSocket* sck,const void* data,int len )
{
   ssize_t ret = 0;
   if ( !sck || sck->lastError != 0 )
   {
      return -1;
   }
   if ( sck->fd == 0 || ( sck->buffered && !sck->wf ) )
   {
      sck->lastError = TCPSERERR_INVALID_STREAM;
      return -1;
   }
   if ( !sck->buffered )
   {
      ret = send ( tcp_sck_fd ( ( const TCPSocket* ) sck ),data,len,0 );
      if ( ret < 0 && errno != 0 )
      {
         tcp_sck_set_lasterr ( ( TCPSocket* ) sck,TCPSERERR_ERRNO_IS_SET );
         if ( ( ( TCPSocket* ) sck )->ErrorHandler.fnc )
         {
            ( ( TCPSocket* ) sck )->ErrorHandler.fnc ( "tcp_socket_write::send",tcp_server_err_string ( ( ( TCPSocket* ) sck )->lastError ), ( ( TCPSocket* ) sck )->ErrorHandler.param );

         }
      }
   }
   else
   {
      ret =  fwrite ( data,1,len,tcp_sck_get_wfile ( ( const TCPSocket* ) sck ) );
      if ( ret < 0 && ferror ( tcp_sck_get_wfile ( ( const TCPSocket* ) sck ) ) )
      {
         tcp_sck_set_lasterr ( ( TCPSocket* ) sck,TCPSERERR_ERRNO_IS_SET );
         if ( ( ( TCPSocket* ) sck )->ErrorHandler.fnc )
         {
            ( ( TCPSocket* ) sck )->ErrorHandler.fnc ( "tcp_socket_write::fwrite",tcp_server_err_string ( ( ( TCPSocket* ) sck )->lastError ), ( ( TCPSocket* ) sck )->ErrorHandler.param );
         }
      }
   }
   return ret;
}

static ssize_t tcp_socket_read ( TCPSocket* sck,void* data,int len )
{
   ssize_t ret= 0;
   if ( !sck || sck->lastError != 0 )
   {
      return -1;
   }
   if ( sck->fd == 0 || ( sck->buffered && !sck->wf ) )
   {
      sck->lastError = TCPSERERR_INVALID_STREAM;
      return -1;
   }
   sck->lastError = TCPSERERR_NOERR;
   if ( !sck->buffered )
   {
      ret =  recv ( tcp_sck_fd ( ( const TCPSocket* ) sck ),data,len,MSG_WAITALL );
      if ( ret < 0 && errno != 0 )
      {
         tcp_sck_set_lasterr ( ( TCPSocket* ) sck,TCPSERERR_ERRNO_IS_SET );
         if ( ( ( TCPSocket* ) sck )->ErrorHandler.fnc )
         {
            ( ( TCPSocket* ) sck )->ErrorHandler.fnc ( "tcp_socket_read::recv",tcp_server_err_string ( ( ( TCPSocket* ) sck )->lastError ), ( ( TCPSocket* ) sck )->ErrorHandler.param );
         }

      }
   }
   else
   {
      ret =  fread ( data,1,len,tcp_sck_get_rfile ( ( const TCPSocket* ) sck ) );
      if ( ret < 0 && ferror ( tcp_sck_get_rfile ( ( const TCPSocket* ) sck ) ) )
      {
         tcp_sck_set_lasterr ( ( TCPSocket* ) sck,TCPSERERR_ERRNO_IS_SET );
         if ( ( ( TCPSocket* ) sck )->ErrorHandler.fnc )
         {
            ( ( TCPSocket* ) sck )->ErrorHandler.fnc ( "tcp_socket_read::fread",tcp_server_err_string ( ( ( TCPSocket* ) sck )->lastError ), ( ( TCPSocket* ) sck )->ErrorHandler.param );
         }

      }

   }
   return ret;

}
static void tcp_socket_close ( TCPSocket* sck )
{
   if ( !sck ) return;
   if ( sck->fd )
   {
      if ( sck->buffered )
      {
         if ( sck->wf )
         {
            fclose ( sck->wf );
         }
         if ( sck->rf )
         {
            fclose ( sck->rf );
         }
      }
      close ( sck->fd );
   }
   sck->wf = 0;
   sck->rf = 0;
   sck->fd = 0;
}
static void tcp_socket_delete ( TCPSocket* sck )
{
   tcp_socket_close ( sck );
   free ( sck );
}

static TCPSocket* tcp_socket_create ( int buffered )
{
   TCPSocket* ret =  ( TCPSocket* ) calloc ( 1,sizeof ( TCPSocket ) );
   if ( ret )
   {
      ret->buffered  = buffered;
   }
   return ret;
}
static int isLocalIP ( const char* addr )
{
   return ( strcmp ( addr,"127.0.0.1" ) == 0 || strcmp ( addr,"localhost" ) ==0 );
}
static void tcp_socket_connect ( TCPSocket* sck,const char* hostaddr,int port )
{
   if ( !sck ) return;
   struct hostent* host;
   if ( port == 0 )
   {
      tcp_sck_set_lasterr ( sck,TCPSERERR_NOPORT );
      return;
   }
   else
   {
      tcp_sck_set_lasterr ( sck,TCPSERERR_NOERR );
   }
   if ( !isIPAddr ( hostaddr ) )
   {
      host = gethostbyname ( hostaddr );
      sck->addr.sin_addr = * ( struct in_addr * ) *host->h_addr_list;
   }
   else
   {
      sck->addr.sin_addr.s_addr = inet_addr ( hostaddr );
   }
   tcp_setup_socket_fd ( sck );
   if ( isLocalIP ( hostaddr ) )
   {
      int one = 1;
      if ( setsockopt ( sck->fd,SOL_TCP,TCP_NODELAY,&one,sizeof ( int ) ) < 0 )
      {
         if ( errno && ( ( TCPSocket* ) sck )->ErrorHandler.fnc )
         {
            tcp_sck_set_lasterr ( ( TCPSocket* ) sck,TCPSERERR_ERRNO_IS_SET );
            ( ( TCPSocket* ) sck )->ErrorHandler.fnc ( "tcp_socket_connect::setsockopt(TCP_NODELAY)",tcp_server_err_string ( ( ( TCPSocket* ) sck )->lastError ), ( ( TCPSocket* ) sck )->ErrorHandler.param );
         }
      }
   }
   sck->addr.sin_port = ntohs ( port );
   if ( -1== connect ( tcp_sck_fd ( sck ), ( struct sockaddr* ) tcp_sck_addr ( sck ),sizeof ( struct sockaddr ) ) )
   {
      tcp_sck_set_lasterr ( sck,TCPSERERR_CONNECT_REFUSE );
      if ( ( ( TCPSocket* ) sck )->ErrorHandler.fnc )
      {
         ( ( TCPSocket* ) sck )->ErrorHandler.fnc ( "tcp_socket_connect::connect",tcp_server_err_string ( ( ( TCPSocket* ) sck )->lastError ), ( ( TCPSocket* ) sck )->ErrorHandler.param );
      }

   }
   if ( sck->buffered )
   {
      if ( !sck->wf )
      {
         sck->wf = fdopen ( tcp_sck_fd ( sck ),"wb" );
         if ( !sck->wf && errno )
         {
            tcp_sck_set_lasterr ( ( TCPSocket* ) sck,TCPSERERR_ERRNO_IS_SET );
            if ( ( ( TCPSocket* ) sck )->ErrorHandler.fnc )
            {
               ( ( TCPSocket* ) sck )->ErrorHandler.fnc ( "tcp_socket_connect::fdopen(write buffer)",tcp_server_err_string ( ( ( TCPSocket* ) sck )->lastError ), ( ( TCPSocket* ) sck )->ErrorHandler.param );
            }

         }
      }
      if ( !sck->rf )
      {
         sck->rf = fdopen ( dup ( tcp_sck_fd ( sck ) ),"rb" );
         if ( !sck->rf && errno )
         {
            tcp_sck_set_lasterr ( ( TCPSocket* ) sck,TCPSERERR_ERRNO_IS_SET );
            if ( ( ( TCPSocket* ) sck )->ErrorHandler.fnc )
            {
               ( ( TCPSocket* ) sck )->ErrorHandler.fnc ( "tcp_socket_connect::fdopen(read buffer)",tcp_server_err_string ( ( ( TCPSocket* ) sck )->lastError ), ( ( TCPSocket* ) sck )->ErrorHandler.param );
            }

         }

      }
   }

}

static void tcp_server_delete ( TCPServerSocket* sck )
{
   if ( !sck ) return;
   tcp_socket_close ( ( TCPSocket* ) sck );
   free ( sck );
}
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

static UnixSocket* unixsck_create(int buffered);
static int         unixsck_bind(UnixSocket* sck,int port);
static void        unixsck_set_errhandler(UnixSocket* sck,void(* fnc)(const char*,const char*,void*),void* param);
static int         unixsck_listen(UnixSocket* sck,int backlog); // set socket fd here
static UnixSocket* unixsck_accept_new(UnixSocket* sck);
static void        unixsck_accept(UnixSocket* scks,UnixSocket* sck);
static void        unixsck_close(UnixSocket* sck);
static int         unixsck_lasterr(UnixSocket* sck);
static const char* unixsck_get_errstr(int errcode);
static ssize_t     unixsck_write(UnixSocket* sck,const void* data,size_t size);
static ssize_t     unixsck_read(UnixSocket* sck,void* data,size_t size);
static void        unixsck_delete(UnixSocket* sck);
static int         unixsck_connect(UnixSocket* sck,int port);
static void        unixsck_flush(UnixSocket* sck);
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

static UnixSocket* unixsck_create(int buf){
   UnixSocket* ret;
   ret = (UnixSocket*)calloc(1,sizeof(UnixSocket));
   ret->buffered = buf;
   return ret;
}
static int unixsck_lasterr(UnixSocket* sck){
   if(sck == 0){
      return UNIXSCK_ERR_INVALID_ARG;
   }
   if(sck->lastError == UNIXSCK_ERRNO_SET && errno == 0){
      sck->lastError = 0;
   }
   return sck->lastError;
}
static const char* unixsck_get_errstr(int errcode){
   if(errcode < 0 || errcode > UNIXSCK_ERR_MAX){
      return "Invalid Error Code";
   }

   if(errcode == UNIXSCK_ERRNO_SET){
      return strerror(errno);
   }
   return unix_enum_err_str[errcode];
}

static void unixsck_set_errhandler(UnixSocket* sck,void(* fnc)(const char*,const char*,void*),void* param){
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

static int unixsck_bind(UnixSocket* sck,int port){
   
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

static int  unixsck_listen(UnixSocket* sck,int backlog){
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

static UnixSocket* unixsck_accept_new(UnixSocket* sck){
   UnixSocket* insck;
   sck = unixsck_create(sck->buffered);
   if(sck){
      unixsck_accept(sck,insck);
   }
   return sck;
}
static void unixsck_accept(UnixSocket* scks,UnixSocket* sck){
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
static void unixsck_close(UnixSocket* sck){
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
static void unixsck_delete(UnixSocket* sck){
   unixsck_close(sck);
   free(sck);
}
static ssize_t unixsck_write(UnixSocket* sck,const void* data,size_t size){
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
static ssize_t unixsck_read(UnixSocket* sck,void* data,size_t size){
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

static int unixsck_connect(UnixSocket* sck,int port){
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
static void unixsck_flush(UnixSocket* sck){
   if(sck && sck->buffered && sck->wf){ 
      fflush(sck->wf);
   }
}
#endif
