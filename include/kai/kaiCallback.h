#ifndef _KAICALLBACK_H_
#define _KAICALLBACK_H_

class kaiMsg;
class kaiSocket;

class kaiOnMsgCallback
{
	public:
	virtual void onMessage(kaiMsg& msg) = 0;
};

class kaiOnRecvCallback
{
	public:
	virtual void onReceive(void* data, int len) = 0;


};

class kaiOnSendCallback
{
	public:
	virtual void onSend(int len) = 0;
};

class kaiOnCloseCallback
{
	public:
	virtual void onClose() = 0;
};

class kaiOnAcceptCallback
{
	public:
	virtual void onAccept(kaiSocket& sock) = 0;
};


#endif 
