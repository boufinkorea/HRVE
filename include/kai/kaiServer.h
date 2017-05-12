#ifndef __KAISERVER_H__
#define __KAISERVER_H__
#include <map>
#include <vector>
#include <queue>
/**
@file kaiServer.h
@brief network server class 
@author Wonsub Choi
@date 2008/2/27
*/
using namespace std;
class kaiIDGenerator;


/**
 * kaiServer class. 
 */
class KAIEXPORT kaiServer
{
public:
	typedef map<int, kaiSocket*> ClientList;

	/** 
	 * kaiServer constructor
	 * default constructor, do nothing
	 */
	kaiServer();

	/** 
	 * kaiServer destructor
	 * default destructor, do nothing
	 */
	virtual ~kaiServer();

	/** 
	 * called after constructing kaiServer object. 
	 * @param port port number. if TCP, listen socket is binded to this port, otherwise newly opened UDP sockets are binded to port number increased one by one from this number.
	 * @param event a bitmask which specifies a combination of network event in which application is interested.
	 * kaiON_ACCEPT for accept callback, kaiON_MESSAGE for message callback, kaiON_CLOSE for close callback. 
	 * @param bTCP if to create TCP server set this value true, otherwise false. 
	 */
	int create(unsigned short port, long event = kaiON_ACCEPT | kaiON_MESSAGE | kaiON_CLOSE, bool bTCP = true);
	
	/**
	 * run the server. 
	 * @param bThreaded if true, server spawn the new thread. otherwise server runs on current thread.
	 * @param loopPerSec determine how many loop is processed per one second.  
	 */
	void run(bool bThreaded = false, int loopPerSec = 50);

	/* 
	 * stop the server process
	 */
	void stop();

	/*
	 * open new UDP socket
	 * @return if success, return binded port number. otherwise return 0
	 */
	unsigned short openUDPChannel();

	/*
	 *
	 */
	void closeClient(int id);

#ifdef WIN32
	friend void threadFunc(void* param);
#else 
	friend void* threadFunc(void* param);
#endif

protected:
	/** 
	 * called when there is client that has message. 
	 * override this function to process the message of each client 
	 * @param id unique id for client. 
	 * @param msg kaiMsg object to be read from recvBuffer
	 */
	virtual void onMessage(int id, kaiMsg& msg){}

	/** 
	 * called when there is client that received new incoming data.
	 * override this function to do something when client receive new data 
	 * @param id unique id for client. 
	 * @param data pointer to data received
	 * @param len size of data received
	 */
	virtual void onReceive(int id, const void* data, int len){}
	
	/** 
	 * called when accepting new connection
	 * override this function to do something when new socket connection is accepted
	 * @param id unique id for client. 
	 * @param sock kaiSocket object for new socket. 
	 */
	virtual void onAccept(int id){}

	/** 
	 * called when closing socket
	 * override this function to do something when client is closed
	 * @param id unique id for client.
	 */
	virtual void onClose(int id){}
	
    /** 
	 * call this function to get kaiSocket object with id
	 * @param id unique id for client
	 */
	kaiSocket* findClient(int id);

private:
	void _run();
	void _closeClient();

	fd_set _rset;
	fd_set _temp;
	kaiSocket _serverSocket;
	
	int _curClientNum;
	int _maxClientNum;
	timeval _timeout;
	
	ClientList _clientList;
	ClientList _clientListByID;
	kaiIDGenerator* _idGen;
	
	long _event;
	unsigned short _port;
	bool _bTCP;
	int _hThread;
	int _loopPerSec;
	bool _bStop;
	int _sleepTime;
	queue<int> _closeQueue;
	
#ifndef WIN32
	vector<int> _fdSets;
	int _maxfd;
#endif

#ifdef WIN32
	CRITICAL_SECTION _cs;
#else
    pthread_mutex_t _lock;
#endif
	
};


#endif
