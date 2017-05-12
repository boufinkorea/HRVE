/** 
@file kai.h
@brief include files for using kai network library
@author Wonsub Choi
@date 2008/2/26
*/
#ifndef __KAI_H__
#define __KAI_H__

#ifdef WIN32
#include <WinSock2.h>
#else
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#endif

#include "kaiDefinition.h"
#include "kaiMsg.h"
#include "kaiSocket.h"
#include "kaiServer.h"





KAI_EXTERN_C_BEGIN

/** 
 * initialize network
 */
KAIEXPORT int kaiInitialize();

/**
 * disable network.
 */ 
KAIEXPORT void kaiShutdown();


KAI_EXTERN_C_END


#endif
