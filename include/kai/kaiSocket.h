/** 
@file kaiSocket.h
@brief network socket class 
@author Wonsub Choi
@date 2008/2/26
*/
#ifndef __KAISOCKET_H__
#define __KAISOCKET_H__

#include <string>
#include "kaiCallback.h"
using namespace std;


class kaiBuffer;
/**
 * kaiSocket class. 
 */
class KAIEXPORT kaiSocket
{
public:
	/** 
	* kaiMsg constructor
	* default constructor, do nothing
	*/
	kaiSocket();

	/** 
	* kaiMsg destructor
	* default destructor. do nothing
	*/
	virtual ~kaiSocket();

	/**
	 * called after constructing kaiSocket object to create socket and attach it. 
	 * @param bTCP true for TCP socket, false for UDP socket
	 * @param lEvent a bitmask which specifies a combination of network event in which the application is interested. 
	 * kaiON_SEND for send callback, kaiON_MESSAGE for message callback,  kaiON_RECV for receive callback, kaiON_ACCEPT for accept callback, kaiON_CLOSE for close. 
	 * @param buffersize total buffer size for message
	 * @param maxdatasize maximum data size that application handle with this socket class
	 */
	void create(bool bTCP = true, unsigned buffersize = kaiBUFFER_SIZE, unsigned maxdatasize = kaiMAX_MSG_SIZE);

	/**
	 * send data on a connected socket
	 * call the onSend callback function if kaiON_SEND flag is set when created. 
	 * @param[in] buf pointer to a buffer containing data to be transmitted. 
	 * @param len size of data to be transmitted
	 * @return the size of data to be sent
	 */
	int send(const char* buf, int len);
	
	/** 
	 * send data on a connected socket
	 * call the onSend callback function if kaiON_SEND flag is set when created. 
	 * @param[in] msg kaiMsg object containing data to be transmitted
	 * @return the size of data to be sent
	 */
	int send(const kaiMsg& msg);

	/** 
	 * send data on a connected socket. the data to be transmitted should already be in _sendBuffer
	 * call the onSend callback function if kaiON_SEND flag is set when created. 
	 * @return the size of data to be sent
	 * @see putMessage()
	 */
	int send();

	/**
	 * send data to a specific destination. created socket should be UDP socket
	 * call the onSend callback function if kaiON_SEND flag is set when created. 
	 * @param[in] buf pointer to a buffer containing data to be transmitted
	 * @param len size of data to be transmitted
	 * @param addr the network address of the destination to which the data is transmitted 
	 * @param port port number of destination
	 * @return the size of data to be sent
	 */
	int sendTo(const char* buf, int len, const char* addr, unsigned short port);

	/**
	* send data to a specific destination. created socket should be UDP socket
	* call the onSend callback function if kaiON_SEND flag is set when created.
	* @param[in] msg kaiMsg object containing data to be transmitted
	* @param addr the network address of the destination to which the data is transmitted 
	* @param port port number of destination
	* @return the size of data to be sent
	*/
	int sendTo(const kaiMsg& msg, const char* addr, unsigned short port);

	/**
	* send data to a specific destination. created socket should be UDP socket. the data to be transmitted should already be in _sendBuffer
	* call the onSend callback function if kaiON_SEND flag is set when created.
	* @param addr the network address of the destination to which the data is transmitted 
	* @param port port number of destination
	* @return the size of data to be sent
	*/
	int sendTo(const char* addr, unsigned short port);
		
	/**
	* send data to a specific destination. created socket should be UDP socket
	* call the onSend callback function kaiON_SEND flag  is set when created.
	* @param[in] buf pointer to a buffer containing data to be transmitted
	* @param len size of data to be transmitted
	* @param lpSockAddr pointer to sockaddr_in structure that contains the network address of destination 
	* @return the size of data to be sent
	*/
	int sendTo(const char* buf, int len, const sockaddr_in* lpSockAddr);

	/**
	* send data to a specific destination. created socket should be UDP socket
	* call the onSend callback function ikaiON_SEND flag  is set when created.
	* @param[in] msg kaiMsg object containing data to be transmitted
	* @param lpSockAddr pointer to sockaddr_in structure that contains the network address of destination 
	* @return the size of data to be sent
	*/
	int sendTo(const kaiMsg& smg, const sockaddr_in* lpSockAddr);


	/**
	* send data to a specific destination. created socket should be UDP socket. the data to be transmitted should already be in _sendBuffer
	* call the onSend callback function if kaiON_SEND flag is set when created.
	* @param lpSockAddr pointer to sockaddr_in structure that contains the network address of destination 
	* @return the size of data to be sent
	*/
	int sendTo(const sockaddr_in* lpSockAddr);
	
	/**
	 * send data to host that already sent data to this socket . created socket should be UDP socket.  
	 * call the onSend callback function if kaiON_SEND flag is set when created.
	 * @param[in] msg kaiMsg object containing data to be transmitted
	 * @return the size of data to be sent
	 */
	int sendTo(const kaiMsg& msg);

	/**
	 * receive data from a socket. socket should be TCP socket. data is written in _recvBuffer
	 * call the onMessage function if kaiON_MESSAGE flag is set and call onReceive function if kaiON_RECV flag is set when crated
	 * if both are set, kaiON_RECV is ignored. recommended to use this function when one of above flags is set. 
	 * * @return the number of bytes received
	 */
	int recv();

	/**
	 * receive data from a socket. socket should be TCP socket. 
	 * no callback function is called. 
	 * @param[out] buf pointer to buffer to receive data 
	 * @param len maximum data size to receive
	 * @return the number of bytes received
	 */
	int recv(void* buf, int len);
	

	/**
	 * receive data from a socket. socket should be UDP socket. 
	 * call the onMessage function if kaiON_MESSAGE flag is set and call onReceive function if kaiON_RECV flag is set when crated
	 * if both are set, kaiON_RECV is ignored. recommended to use this function when one of above flags is set.
	 * @param[out] addr string object that receives a dotted number ip address 
	 * @param[out] port port number of source address
	 * @return the number of bytes received
	 */
	int recvFrom(string& addr, unsigned short& port);

	/**
	* receive data from a socket. socket should be UDP socket. 
	* no callback function is called. 
	* @param[out] buf pointer to buffer to receive data 
	* @param len maximum data size to receive
	* @param[out] addr string object that receives a dotted number ip address 
	* @param[out] port port number of source address
	* @return the number of bytes received
	*/
	int recvFrom(void* buf, int len, string& addr, unsigned short& port);
	
	/**
	* receive data from a socket. socket should be UDP socket. 
	* call the onMessage function if kaiON_MESSAGE flag is set and call onReceive function if kaiON_RECV flag is set when crated
	* if both are set, kaiON_RECV is ignored. recommended to use this function when one of above flags is set.
	* @param[out] lpSockAddr pointer to sockaddr_in structure 
	* @return the number of bytes received
	*/
	int recvFrom(sockaddr_in* lpSockAddr);

	/**
	* receive data from a socket. socket should be UDP socket. 
	* no callback function is called. 
	* @param[out] buf pointer to buffer to receive data 
	* @param len maximum data size to receive
	* @param[out] lpSockAddr pointer to sockaddr_in structure 
	* @return the number of bytes received
	*/
	int recvFrom(void* buf, int len, sockaddr_in* lpSockAddr);

	/** 
	 * set socket handle manually. socket handle assigned before is released. 
	 * @param sock socket handle to be assigned
	 */
	void setSocket(SOCKET sock);

	/** 
	 * get socket handle contained in this object
	 * @return socket handle
	 */
	SOCKET getSocket();

	/** 
	 * close the socket
	 */ 
	void close();

	/**
	 * establish a connection. can be used for both TCP and UDP socket
	 * @return if this function is successful return kaiSUCCESS otherwise return kaiFAIL.
	 */
	int connect(const char* addr, unsigned short port);

	/** 
	 * listen for incoming connection requests
	 * @return if this function is successful return kaiSUCCESS otherwise return kaiFAIL.
	 */
	int listen();

	/** 
	 * associate a local address with the socket
	 * @param port port number to bind
	 * @return if this function is successful return kaiSUCCESS otherwise return kaiFAIL.
	 */
	int bind(unsigned short port);

	/** 
	 * accept a connector on a socket
	 * @param[out] a reference identifying a new socket that is available for connection
	 */
	void accept(kaiSocket& sock);
	
	/** 
	 * set blocking mode for socket. 
	 * @param blocking specifies blocking mode. for blocking socket. kaiBLOCKING_SOCKET, for non-blocking mode, set kaiNON_BLOCKING_SOCKET
	 */
	void setBlockingMode(int blocking);
	
	/** 
	 * set id for this object
	 * @param id id to be set
	 */
	void setID(int id);

	/** 
	 * get id
	 * @return id 
	 */
	int getID() const;
	
	/** 
	 * get message from _recvBuffer manually. this function get one message from recvBuffer
	 * @param[out] reference to kaiMsg object 
	 * @return if there is message return true otherwize return false. 
	 */
	bool getMessage(kaiMsg& msg);
	
	/** 
	 * get message from _recvBuffer manually. this function get whole data from recvBuffer. 
	 * @param[out] data double pointer for data. get address to data buffer.
	 * @len[out] size of data got with this call. 
	 */
	void getMessage(void** data, int& len);

	/** 
	 * put message to sendBuffer.
	 * @param[in] kaiMsg object to put data in sendBuffer
	 */
	void putMessage(const kaiMsg& msg);

	/** 
	 * put data to send buffer
	 * @param[in] pointer to data buffer to put data in sendBuffer
	 * @len total size of data to be put
	 */
	void putMessage(const void* data, int len);
	
	/** 
	 *set user data
	 *@param integer parameter
	 */
	void setUserParam(int param);

	/** 
	 * get user data
	 * @return integer user data
	 */
	int getUserParam() const;

	
	void reset();

	void setNoDelay();

	void setManualClose(bool b);

	void registerOnMsgCallback(kaiOnMsgCallback* callback);
	void registerOnRecvCallback(kaiOnRecvCallback* callback);
	void registerOnSendCallback(kaiOnSendCallback* callback);
	void registerOnCloseCallback(kaiOnCloseCallback* callback); 
	void registerOnAcceptCallback(kaiOnAcceptCallback* callback);


private:
	
	kaiOnMsgCallback* _onMsgCallback;
	kaiOnRecvCallback* _onRecvCallback;
	kaiOnSendCallback* _onSendCallback;
	kaiOnCloseCallback* _onCloseCallback;
	kaiOnAcceptCallback* _onAcceptCallback;

	

private:
	
	SOCKET		_socket;
	kaiBuffer*	_recvBuffer;
	kaiBuffer*	_sendBuffer;
	sockaddr_in _addr;
	int			_id;
	
	int			_userParam;
	bool		_bManualClose;

};

#endif
