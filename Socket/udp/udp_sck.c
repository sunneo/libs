#include "udp_sck.h"

UDPSocket* udp_sck_create()
{
   UDPSocket* ret = (UDPSocket*)malloc(sizeof(UDPSocket));
   memset(ret,0,sizeof(UDPSocket));
   
   return ret;
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

int udp_sck_read(UDPSocket* sck, void* ptr, int len, struct sockaddr* addr)
{
   if(sck->fd <= 0) return 0; 
   int slen = sizeof(struct sockaddr_in);
   return recvfrom(sck->fd, ptr, len,0,  (struct sockaddr*)addr,&slen);
}
int udp_sck_write(UDPSocket* sck, const void* data, int len,struct sockaddr* addr)
{
   if(sck->fd <= 0) return 0;
   int slen = sizeof(struct sockaddr_in);
   return sendto(sck->fd, data, len , 0, (struct sockaddr*) addr, slen);
}

void udp_sck_delete(UDPSocket* sck)
{
   if(sck->fd > 0) close(sck->fd);
   sck->fd = 0;
   free(sck);
}

