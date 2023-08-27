#include "udp_sck.h"
#include <ctype.h>
#ifdef WIN32
#pragma comment(lib, "Ws2_32.lib")
#endif
#ifdef __cplusplus
#define EXTERN extern "C"
#else 
#define EXTERN
#endif

static char* StringCopy(char* dst, const char* src, size_t len)
{
#ifdef WIN32
    errno_t err = strncpy_s(dst, len, src, len);
    if (err != 0)
    {

    }
    return dst;
#elif defined __linux__
    return strncpy(dst, src, len);
#endif
}

static char* errBuf;
static const char* StringError(int errcode)
{
#ifdef WIN32
    if (errBuf == NULL)
    {
        errBuf = (char*)malloc(4096);
        if (errBuf == NULL)
        {
            return "Error Happened in StringError";
        }
    }
    (void)strerror_s(errBuf, 4096, errcode);
    return errBuf;
#elif defined __linux__
    return strerror(errcode);
#endif
}
static void CloseSocket(SOCKET sck)
{
#ifdef WIN32
   closesocket(sck);
#elif defined __linux__
   close(sck);
#endif
}

EXTERN
UDPSocket* udp_socket_create()
{
#ifdef WIN32
    {
        static BOOL wsaStarted = FALSE;
        if (!wsaStarted)
        {
            WSADATA wsaData;
            // Initialize Winsock
            WSAStartup(MAKEWORD(2, 2), &wsaData);
        }
    }
#endif
   UDPSocket* ret = (UDPSocket*)malloc(sizeof(UDPSocket));
   if (ret == NULL)
   {
       fprintf(stderr, "%s %d %s Failed to create UDPSocket object :%s", __FILE__, __LINE__, __func__, StringError(errno));
       return NULL;
   }
   memset(ret,0,sizeof(UDPSocket));
   
   return ret;
}

EXTERN
int udp_get_last_error(const UDPSocket* sck)
{
   return sck->lastError;
}


EXTERN
void udp_server_bind(UDPSocket* sck,int port)
{
   if(!sck) return;
   sck->fd = socket(AF_INET,SOCK_DGRAM, IPPROTO_UDP);
   if(sck->fd < 0)
   {
      sck->lastError = sck->fd;
      fprintf(stderr, "%s %d %s create socket failed:%s",__FILE__,__LINE__,__func__, StringError(sck->lastError));
      return;
   }
#ifdef WIN32
   BOOL tr = TRUE;
   sck->lastError = setsockopt(sck->fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&tr, sizeof(BOOL));
#elif defined __linux__
   int tr = 1;
   sck->lastError = setsockopt(sck->fd, SOL_SOCKET,SO_REUSEADDR,(const char*)&tr,sizeof(int));
#endif
   if(sck->lastError < 0)
   {
       fprintf(stderr, "%s %d %s setsockopt failed:%s",__FILE__,__LINE__,__func__, StringError(sck->lastError));
       return;
   }
   sck->addr.sin_family = AF_INET;
   sck->addr.sin_port = htons(port);
   sck->addr.sin_addr.s_addr = INADDR_ANY;
   memset(sck->addr.sin_zero,0,sizeof(sck->addr.sin_zero));

   sck->lastError = bind(sck->fd,(struct sockaddr*)&sck->addr,sizeof(struct sockaddr));
   if(sck->lastError < 0)
   {
       fprintf(stderr, "%s %d %s bind failed:%s",__FILE__,__LINE__,__func__, StringError(sck->lastError));
       return;
     
   }
}


EXTERN
int udp_socket_read(UDPSocket* sck, void* ptr, int len, struct sockaddr* addr)
{
   if(sck->fd <= 0) return 0; 
   int slen = sizeof(struct sockaddr_in);
   return recvfrom(sck->fd, ptr, len,0,  (struct sockaddr*)addr,&slen);
}

EXTERN
int udp_socket_write(UDPSocket* sck, const void* data, int len,struct sockaddr* addr)
{
   if(sck->fd <= 0) return 0;
   int slen = sizeof(struct sockaddr_in);
   int flag = 0;
#ifdef WIN32
   flag = 0;
#elif defined __linux__
   flag = MSG_CONFIRM;
#endif
   int ret = sendto(sck->fd, data, len , flag, (struct sockaddr*) addr, slen);
   if(ret >= 0)
   {
      sck->lastError=0;
   }
   else
   {
      sck->lastError=ret;
   }
   return ret;
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

EXTERN
void udp_socket_connect(UDPSocket* sck,const char* hostaddr,int port)
{
   if ( !sck ) return;
   sck->fd = socket(AF_INET,SOCK_DGRAM, IPPROTO_UDP);
   struct hostent* host;
   if ( port == 0 )
   {
      fprintf(stderr, "%s %d %s No port\n",__FILE__,__LINE__,__func__);
      return;
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
   sck->addr.sin_family = AF_INET;
   sck->addr.sin_port = htons(port);
   sck->addr.sin_addr.s_addr = inet_addr(hostaddr);
   memset(sck->addr.sin_zero, '\0', sizeof(sck->addr.sin_zero));
   sck->lastError = connect(sck->fd, (struct sockaddr*)&sck->addr, sizeof(struct sockaddr_in));
}


EXTERN
void udp_socket_delete(UDPSocket* sck)
{
   if(sck->fd > 0) CloseSocket(sck->fd);
   sck->fd = 0;
   free(sck);
}

