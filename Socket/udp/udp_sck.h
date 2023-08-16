#ifndef UDP_SCK_H
#define UDP_SCK_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

typedef struct UDPSocket{
   int fd;
   int lastError;
   struct sockaddr_in addr;
   struct {
      void(*fnc)(const char* fncName,const char* cause,void*);
      void* param;
   }ErrorHandler;
}UDPSocket;

UDPSocket* udp_sck_create();
void udp_server_bind(UDPSocket* sck,int port);
int udp_sck_read(UDPSocket* sck, void* ptr, int len, struct sockaddr* addr);
int udp_sck_write(UDPSocket* sck, const void* data, int len,struct sockaddr* addr);
void udp_sck_delete(UDPSocket* sck);
#endif
