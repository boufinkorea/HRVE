/** 
@file kaiFunction.h
@brief functions used in kai network library
@author Wonsub Choi
@date 2008/2/26
*/
#ifndef __KAIFUNCTION_H__
#define __KAIFUNCTION_H__



void kaiShutdown();
SOCKET kaiCreateSocket(int type);
SOCKET kaiBindSocket(unsigned short port, int type);
int kaiConnectSocket(SOCKET sock, const char* addr, unsigned short port);
int kaiConnectSocket(SOCKET sock, const sockaddr_in* sock_addr);
SOCKET kaiListenSocket(unsigned short port);
void kaiCloseSocket(SOCKET sock);
void kaiMakeSockAddr(const char* addr, unsigned short port, sockaddr_in* sock_addr);
void kaiSetBlockingMode(SOCKET sock, unsigned long blocking);
int kaiSetBroadcasting(SOCKET sock, int mode);
int kaiRecv(SOCKET sock, char* data, int len);
int kaiSend(SOCKET sock, const char* data, int len);
int kaiRecvFrom(SOCKET sock, char* data, int len, sockaddr_in* from);
int kaiSendTo(SOCKET sock, const char* data, int len, const sockaddr_in* to);
int kaiBroadcast(SOCKET sock, int length, char* data, unsigned short port);
int kaiGetCurrentSystemTime();





#endif
