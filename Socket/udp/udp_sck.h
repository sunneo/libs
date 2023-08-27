#ifndef UDP_SCK_H
#define UDP_SCK_H
#include <stdio.h>
#include <stdlib.h>
#ifdef WIN32
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#elif defined __linux__
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define SOCKET int
#endif

#include <errno.h>
#include <string.h>


typedef struct UDPSocket{
   SOCKET fd;
   int lastError;
   struct sockaddr_in addr;
   struct {
      void(*fnc)(const char* fncName,const char* cause,void*);
      void* param;
   }ErrorHandler;
}UDPSocket;

#ifdef __cplusplus
extern "C" {
#endif
UDPSocket* udp_socket_create();
void udp_server_bind(UDPSocket* sck,int port);
int udp_get_last_error(const UDPSocket* sck);
int udp_socket_read(UDPSocket* sck, void* ptr, int len, struct sockaddr* addr);
int udp_socket_write(UDPSocket* sck, const void* data, int len,struct sockaddr* addr);
void udp_socket_delete(UDPSocket* sck);
void udp_socket_connect(UDPSocket* sck,const char* hostaddr,int port);
#ifdef __cplusplus
};
#endif

#endif
