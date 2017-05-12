/** 
@file kaiDefinition.h
@brief definintion for kai network library
@author Wonsub Choi
@date 2008/2/26
*/
#ifndef __KAIDEFINITION_H__
#define __KAIDEFINITION_H__


#ifndef WIN32
#include <sys/types.h>
#endif
#if defined(WIN32) || defined(WINCE)
#	if defined(KAI_EXPORTS)
#		define KAIEXPORT __declspec(dllexport)
#	elif defined(KAI_IMPORTS)
#		define KAIEXPORT __declspec(dllimport)
#	else
#		define KAIEXPORT
#	endif
#else
#	define KAIEXPORT
#endif

#ifdef __cplusplus
#	define KAI_EXTERN_C_BEGIN	extern "C" {
#	define KAI_EXTERN_C_END	}
#else
#	define KAI_EXTERN_C_BEGIN
#	define KAI_EXTERN_C_END
#endif



const int kaiTCP = 0;
const int kaiUDP = 1;
const int kaiINVALID_SOCKET = -1;
const int kaiFAIL = -1;
const int kaiSUCCESS = 1;

#ifndef WIN32
const int SOCKET_ERROR = -1;
const int INVALID_SOCKET = -1;
#endif

#ifndef WIN32
typedef int SOCKET;
#endif

const unsigned long kaiBLOCKING_SOCKET = 0;
const unsigned long kaiNON_BLOCKING_SOCKET = 1;

typedef unsigned char	kaiCommand;
//typedef unsigned short	kaiMsgID;
typedef unsigned char kaiMsgID;
typedef unsigned short	kaiMsgSize;
typedef unsigned char	kaiFlag;

#pragma pack(push, 1)
typedef struct  
{	
	kaiMsgID id;
	kaiMsgSize size;
	kaiFlag flag;
} kaiHeader;
#pragma pack(pop)

const kaiMsgSize Size_kaiHEADER = sizeof(kaiHeader);


const size_t Size_kaiMsgID = sizeof(kaiMsgID);
const size_t Size_kaiMsgSize = sizeof(kaiMsgSize);
const size_t Size_kaiFlag = sizeof(kaiFlag);

const kaiMsgSize kaiMAX_DATA_SIZE = 1024*16;
const kaiMsgSize kaiMAX_MSG_SIZE = kaiMAX_DATA_SIZE + Size_kaiHEADER;
const unsigned kaiBUFFER_SIZE = 1024*128;

const long kaiNO_EVENT		= 0x00;
const long kaiON_MESSAGE	= 0x01;
const long kaiON_ACCEPT		= 0x02;
const long kaiON_CLOSE		= 0x04;
const long kaiON_SEND		= 0x08;
const long kaiON_RECV		= 0x10;


#endif
