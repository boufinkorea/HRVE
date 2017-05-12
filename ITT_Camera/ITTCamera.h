/*
 * ITTCamera.h
 *
 *  Created on: 2017. 2. 10.
 *      Author: jinhkim
 */

#ifndef ITTCAMERA_H_
#define ITTCAMERA_H_

#include <PvSampleUtils.h>
#include <PvSystem.h>
#include <PvInterface.h>
#include <PvStream.h>
#include <PvPipeline.h>
#include <PvBuffer.h>
#include <PvDevice.h>
#include <PvDeviceGEV.h>
#include <PvStreamGEV.h>
#include <PvBufferWriter.h>
#include <definitions.h>
#include "kai.h"
#include "kaiCallback.h"
#include <time.h>
#include <opencv2/opencv.hpp>

using namespace cv;

class ITT_Camera: public kaiOnMsgCallback {
public:
	ITT_Camera();
	virtual ~ITT_Camera();
	int init();
	uint32_t dTime2Frame(uint16_t duration, double_t FrameRate);
	virtual void onMessage(kaiMsg& msg);
	int connectCam();
	int disconnectCam();
	int openStream();
	int configureStream();
	int createPipeline();
	int beforeAcquisition();
	int takePicture(int imgType);
	int afterAcquisition();
	int FileTransfer(uint8_t filetype=FILE_IMAGE);
	int closePipeline();
	int takeMovie();
	int controlCam(cam_control &controlParams);
	int monitorCam();
	int SetRecv(string Addr, unsigned short Port);

private:
	int filenameStamp(uint8_t type=0);
	int saveFilename(uint8_t filetype_flag);
	int setFilename();

private:
	kaiSocket	m_sock;		/*socket member*/
	PvDevice*	m_pDevice;	/*Camera Device*/
	PvResult	m_lResult;	/*Result variable for error handling _Bobcat camera*/
	cam_status	m_camStat;	/*Camera status variable*/
	PvStream*	m_pStream;	/*Stream pointer*/
	PvPipeline*	m_pPipeline;	/*Pipeline pointer*/
	PvGenParameterArray*	m_pDeviceParams;
	PvGenParameterArray*	m_pStreamParams;
	uint32_t 	BytesWritten;	/*	Filesize in Bytes	*/
	char*	 	Filename;		/*File name	*/
	char*		PastFilename;	/*Past Filename for transfer loss situation*/
	VideoWriter	vwr;		/*video writer instance*/
	uint8_t		pastFiletype;	/*Image or movie*/
};

#endif /* ITTCAMERA_H_ */
