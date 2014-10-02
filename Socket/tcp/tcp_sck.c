#include "tcp_sck.h"
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

TCPServerSocket *tcp_server_create ( int buffered )
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

void tcp_server_set_errhandler ( TCPServerSocket* sck,void ( *fnc ) ( const char*,const char*,void* ),void* param )
{
   tcp_socket_set_errhandler ( ( TCPSocket* ) sck,fnc,param );
}
void tcp_socket_set_errhandler ( TCPSocket* sck,void ( *fnc ) ( const char*,const char*,void* ),void*param )
{
   if ( sck )
   {
      ( ( TCPSocket* ) sck )->ErrorHandler.fnc = fnc;
      ( ( TCPSocket* ) sck )->ErrorHandler.param = param;
   }
}

void tcp_socket_flush ( TCPSocket* sck )
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

void tcp_server_bind ( TCPServerSocket* sck,int port )
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

void tcp_server_listen ( TCPServerSocket* sck,int queueLen )
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
void tcp_sck_peername ( TCPSocket* sck, char* name, int len )
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

void tcp_server_accept ( TCPServerSocket* sck,TCPSocket* sckin )
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

int  tcp_get_last_error ( const TCPSocket* sck )
{
   if ( !sck ) return TCPSERERR_NULL_POINTER;
   return tcp_sck_get_lasterr ( sck );
}

const char* tcp_server_err_string ( int err )
{
   if ( err > TCPSERERR_MAX )
      err = TCPSERERR_MAX;
   if ( err == TCPSERERR_ERRNO_IS_SET )
   {
      return strerror ( err );
   }
   return private_tcpsererr_strings[ err ];
}

int tcp_socket_write ( TCPSocket* sck,const void* data,int len )
{
   int ret = 0;
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

int tcp_socket_read ( TCPSocket* sck,void* data,int len )
{
   int ret= 0;
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
void tcp_socket_close ( TCPSocket* sck )
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
void tcp_socket_delete ( TCPSocket* sck )
{
   tcp_socket_close ( sck );
   free ( sck );
}

TCPSocket* tcp_socket_create ( int buffered )
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
void tcp_socket_connect ( TCPSocket* sck,const char* hostaddr,int port )
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
      /*if ( setsockopt ( sck->fd,SOL_TCP,TCP_CORK,&one,sizeof ( int ) ) < 0 )
      {
         if ( errno && ( ( TCPSocket* ) sck )->ErrorHandler.fnc )
         {
            tcp_sck_set_lasterr ( ( TCPSocket* ) sck,TCPSERERR_ERRNO_IS_SET );
            ( ( TCPSocket* ) sck )->ErrorHandler.fnc ( "tcp_socket_connect::setsockopt(TCP_CORK)",tcp_server_err_string ( ( ( TCPSocket* ) sck )->lastError ), ( ( TCPSocket* ) sck )->ErrorHandler.param );
         }
      }*/
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

void tcp_server_delete ( TCPServerSocket* sck )
{
   if ( !sck ) return;
   tcp_socket_close ( ( TCPSocket* ) sck );
   free ( sck );
}
