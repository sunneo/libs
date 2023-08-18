#include "udp_sck.h"
#include <ctype.h>

UDPSocket* udp_socket_create()
{
   UDPSocket* ret = (UDPSocket*)malloc(sizeof(UDPSocket));
   memset(ret,0,sizeof(UDPSocket));
   
   return ret;
}
int udp_get_last_error(const UDPSocket* sck)
{
   return sck->lastError;
}

void udp_server_bind(UDPSocket* sck,int port)
{
   if(!sck) return;
   sck->fd = socket(AF_INET,SOCK_DGRAM, IPPROTO_UDP);
   if(sck->fd < 0)
   {
      sck->lastError = sck->fd;
      fprintf(stderr, "%s %d %s create socket failed:%s",__FILE__,__LINE__,__func__,strerror(sck->lastError));
      return;
   }
   char tr = '1';
   sck->lastError = setsockopt(sck->fd, SOL_SOCKET,SO_REUSEADDR, &tr,sizeof(int));
   if(sck->lastError < 0)
   {
       fprintf(stderr, "%s %d %s create socket failed:%s",__FILE__,__LINE__,__func__,strerror(sck->lastError));
       return;
   }
   sck->addr.sin_family = AF_INET;
   sck->addr.sin_port = htons(port);
   sck->addr.sin_addr.s_addr = INADDR_ANY;
   memset(sck->addr.sin_zero,0,sizeof(sck->addr.sin_zero));

   sck->lastError = bind(sck->fd,(struct sockaddr*)&sck->addr,sizeof(struct sockaddr));
   if(sck->lastError < 0)
   {
       fprintf(stderr, "%s %d %s bind failed:%s",__FILE__,__LINE__,__func__,strerror(sck->lastError));
       return;
     
   }
}

int udp_socket_read(UDPSocket* sck, void* ptr, int len, struct sockaddr* addr)
{
   if(sck->fd <= 0) return 0; 
   int slen = sizeof(struct sockaddr_in);
   return recvfrom(sck->fd, ptr, len,0,  (struct sockaddr*)addr,&slen);
}
int udp_socket_write(UDPSocket* sck, const void* data, int len,struct sockaddr* addr)
{
   if(sck->fd <= 0) return 0;
   int slen = sizeof(struct sockaddr_in);
   int ret = sendto(sck->fd, data, len , MSG_CONFIRM, (struct sockaddr*) addr, slen);
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

void udp_socket_delete(UDPSocket* sck)
{
   if(sck->fd > 0) close(sck->fd);
   sck->fd = 0;
   free(sck);
}

