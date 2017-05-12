/** 
@file kaiBuffer.h
@brief ring buffer class 
@author Wonsub Choi
@date 2008/2/26
*/
#ifndef __KAIBUFFER_H
#define __KAIBUFFER_H

class KAIEXPORT kaiBuffer
{
public:
	/** 
	 * kaiBuffer constructor
	 * default constructor, do nothing
	 */
	kaiBuffer();

	/** 
	* kaiBuffer destructor
	* default destructor, do nothing
	*/
	virtual ~kaiBuffer();
	
	/** 
	* initialize buffer. allocate memory for buffer
	* @param buffersize total buffer size to allocate
	* @param maxdatasize maximum data size to be handled per one time 
	*/
	void init(unsigned buffersize = kaiBUFFER_SIZE, unsigned maxdatasize = kaiMAX_MSG_SIZE);

	/** 
	 * refresh member variables
	 */
	void refresh();

	/** 
	 * get data size in buffer
	 * @return data size in buffer
	 */
	unsigned getDataSize();
	
	/** 
	 * get pointer to buffer for putting data
     */
	char* bufferIn();

	/** 
	 * get pointer to buffer for getting data
	 */
	const char* bufferOut() const;
	
	/** 
	 * put data to buffer as much as size
	 * @param[in] data pointer to data buffer
	 * @param size data size to be put
	 */
	void put(const void* data, unsigned short size);

	/**
	 * move data-in pointer 
	 * @param size 
	 */
	void put(unsigned short size);

	void get(void* data, unsigned short size);
	void pop(unsigned short size);

private:
	kaiBuffer(const kaiBuffer& q){}

	char* _buffer;
	char* _bufferInPtr;
	char* _bufferOutPtr;

	char* _bufferEnd;
	
	unsigned _bufferSize;
	unsigned _maxDataSize;
};

#endif
