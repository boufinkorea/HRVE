/** 
@file kaiMsg.h
@brief message class used in kai network library
@author Wonsub Choi
@date 2008/2/26
*/
#ifndef __KAIMSG_H__
#define __KAIMSG_H__
#include "kaiDefinition.h"
#include <string>
#include <string.h>
/**
 * kaiMsg class. 
 */
class KAIEXPORT kaiMsg
{
public:
	/** 
	 * kaiMsg constructor
	 * default constructor, do nothing
	 */
	kaiMsg();
	/** 
	* kaiMsg destructor
	* if memory are allocated to buffer, free memory
	*/
	virtual ~kaiMsg();
	/** 
	 * kaiMsg copy constructor
	 */
	kaiMsg(const kaiMsg& msg);

	/** 
	 *begin write/read.  should be called before reading and writing data  
	 */
	void			begin();

	/** 
	 * end write/read.  should be called after reading and writing data 
	 */
	void			end();
	
	/** 
	 * allocate new memory to buffer pointer. this memory is released in destructor 
	 * @param[in] size size of memory to be allocated
	 */
	void			allocateMemory(kaiMsgSize size = kaiMAX_MSG_SIZE);
	
	/** 
	 * set buffer pointer to memory that is already available
	 *@param[in] buf pointer to memory
	 */
	void			setBuffer(char* buf, int size = kaiMAX_MSG_SIZE);

	/** 
	 * get message id.
	 * @return message id
	 */
	kaiMsgID		id() const;

	/** 
	 * get message size excluding header size
	 * @return message size
	 */
	kaiMsgSize		size() const;
	
	/** 
	 * get flag field
	 * @return flag
	 */
	kaiFlag			flag() const;

	/** 
	 * set message id
	 * @param id message id
	 */
	void			id(kaiMsgID id);

	/** 
	 * set message size manually
	 * @param size message size excluding header size
	 */
	void			size(kaiMsgSize size);

	/** 
	 * set flag 
	 * @param flag flag set to message
	 */
	void			flag(kaiFlag flag);

	/**
	 * get data size to be read. used only for reading not writing. 
	 * @return data size to be read.
	 */
	kaiMsgSize		getReadSize() const;
	
	/**
	 * get buffer pointer.
	 * @return pointer to buffer
	 */
	const char*		buffer() const;
	
	/** 
	 * get data size including header size. 
	 * @return total data size in this message class
	 */
	unsigned		dataSize() const;

	/**
	 * reset kaiMsg
	 */
	void			reset();

	/** 
	 * overloaded operator for writing data in message object
	 * @code 
	 * kaiMsg msg;
	 * msg.allocate();
	 * msg.begin();
	 * char a='a'; short b=1; int c=2; double d=2.3;
	 * msg << a << b << c << d ;
	 * msg.end();
	 * @endcode
	 * @param[in] val data written in message
	 * @return reference to this
	 */
    template<typename T>
	kaiMsg&		operator<<(T const& val)
	{
		size_t size = sizeof(T);
		if (_pos + size > _bufend)
			return *this;

		memcpy(_pos, &val, size);
		_pos += size;
		return *this;
	}



	/** 
	 * overloaded operator for reading data from message object
	 * @param[out] val data read from message
	 * @see operator<<(const T& val)
	 * @return reference to this
	 */
	template<typename T>
	kaiMsg&		operator>>(T& val)
	{
		size_t size = sizeof(T);
		if (_pos + size > _bufend)
			return *this;

		memcpy(&val, _pos, size);
		_pos += size;

		return *this;
	}


	/** 
	 * write data as much as size
	 * @param[in] data pointer to data buffer
	 * @param size data size to be written
	 */
	void writeData(const void* data, kaiMsgSize size);

	/** 
	 * read data as much as size
	 * @param[out] data pointer to data buffer
	 * @param size data size to be read
	 */
	void readData(void* data, kaiMsgSize size);

	/**
	 * write float
	 * @param[in] float data
	 * @return written data size
	 */
	int writeFloat(float val);

	/**
	 * read float
	 * @param[out] float data
	 * @return read data size
	 */
	int readFloat(float& val);

	/**
	 * write integer
	 * @param[in] integer data
	 * @return written data size
	 */
	int writeInt(int val);

	/**
	 * read integer
	 * @param[out] integer data
	 * @return read data size
	 */
	int readInt(int& val);

	/**
	 * write short integer
	 * @param[in] short integer data
	 * @return written data size
	 */
	int writeShort(short val);

	/**
	 * read short integer
	 * @param[out] short integer data
	 * @return read data size
	 */
	int readShort(short& val);

	/**
	 * write byte
	 * @param[in] byte data
	 * @return written data size
	 */
	int writeByte(unsigned char val);

	/**
	 * read byte
	 * @param[out] byte data
	 * @return read data size
	 */
	int readByte(unsigned char& val);

protected:
	kaiHeader _header;  
private:
	char* _buf; 
	char* _pos; 
	char* _bufend; 
	bool _bAllocated;  
};


#endif
