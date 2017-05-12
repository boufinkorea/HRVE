/*
 * ITTCamera.cpp
 *
 *  Created on: 2017. 2. 10.
 *      Author: jinhkim
 */

#include "ITTCamera.h"

ITT_Camera::ITT_Camera() {
	// TODO Auto-generated constructor stub
	Filename = NULL;
	m_pPipeline = NULL;
	m_pStream = NULL;
	PastFilename = NULL;
	//vwr = NULL;
	pastFiletype = 0;
	m_pDeviceParams = NULL;
	BytesWritten = 0;
	m_pDevice = NULL;
	m_pStreamParams = NULL;
}

ITT_Camera::~ITT_Camera() {
	// TODO Auto-generated destructor stub
}

int ITT_Camera::init() {

	m_sock.create(false);
	if (m_sock.bind(PAYLOAD_PORT) != kaiSUCCESS)
	{
		cout << "Binding error.. check the port..!!" << endl;
		m_camStat.isBind = false;
		return FAIL;
	}
	m_camStat.isBind = true;
	m_sock.setBlockingMode(kaiNON_BLOCKING_SOCKET);
	m_sock.registerOnMsgCallback(this);
	m_camStat.isCamConnected = false;
	m_camStat.imageCount = 0;
	m_camStat.movieCount = 0;
	m_camStat.imageFault = 0;
	m_camStat.movieFault = 0;

	return OKAY;
}

/*duration time to framecount converter*/
uint32_t ITT_Camera::dTime2Frame(uint16_t duration, double_t FrameRate) {
	double_t	dTime = (double_t)duration;
	double_t	outFrame = (duration / 1000.f) * FrameRate;

	return	(uint32_t)outFrame;
}

void ITT_Camera::onMessage(kaiMsg& msg) {
	switch (msg.id())
	{
	case 0x02:	//Take a Picture
	{
		//take a picture...
		int imgType = -1;
		msg.begin();
		msg >> imgType;
		msg.end();
		takePicture(imgType);
		FileTransfer(FILE_IMAGE);
		cout << "Take a picture!!" << endl;
	}
	break;
	case 0x03:	//Record a video for duration msec..
	{
		//record a video... for duration msec..
		uint16_t duration = 0;
		msg.begin();
		msg >> duration;
		msg.end();
		cout << "Duration : " << duration << endl;
		beforeAcquisition();

		BytesWritten = 0;
		filenameStamp(EXT_AVI);		/*Filename & extention decision*/
		int64_t	frmrate;
		m_pDeviceParams->GetIntegerValue("CurrentFrameRate", frmrate);



		uint32_t frameCount = dTime2Frame(duration, (double_t)frmrate);
		cout << "FrameCount : " << frameCount << ", frmrate: " << frmrate << endl;
		for (uint32_t i = 0; i < frameCount; i++)
		{

			takeMovie();

		}
		vwr.release();
		cout << endl;
		afterAcquisition();

		FILE *tmpf = fopen(Filename, "r");
		fseek(tmpf, 0, SEEK_END);
		BytesWritten = ftell(tmpf);
		fclose(tmpf);
		cout << "Before filetransfer" << endl;
		FileTransfer(FILE_MOVIE);
		cout << "Record a video!! for " << duration << "msec" << endl;
	}
	break;
	case 0x04:	//Control camera parameters
	{
		//Set camera parameters...
		msg.begin();
		cam_control	params;
		msg >> params.resolution_mode >> params.exposure_time >> params.analog_gain >> params.digital_gain >> params.frame_time;
		msg.end();
		controlCam(params);
		cout << "Set camera parameters..." << endl;
	}
	break;
	case 0x05:	//Monitor
	{
		msg.begin();
		msg.end();
		/*reportStatus();*/
		cout << "report payload status.." << endl;
	}
	break;
	case 0x06:	//send a file again
	{
		msg.begin();
		msg.end();
		setFilename();
		FileTransfer(pastFiletype);
		cout << "Send a file again.." << endl;
	}
	break;
	default:
	{
		//Default
		cout << "Error has been occured to parse MSG" << endl;
	}
	break;
	}
}

int ITT_Camera::connectCam() {
	m_pDevice = PvDevice::CreateAndConnect(CAM_IP, &m_lResult);
	if (!m_lResult.IsOK())
	{
		cout << "Unable to connect to " << CAM_IP << endl;
		m_camStat.isCamConnected = false;
		return FAIL;
	}
	else
	{
		cout << "Successfully connected to " << CAM_IP << endl;
		m_camStat.isCamConnected = true;
		return OKAY;
	}
}

int ITT_Camera::disconnectCam() {
	if (m_camStat.isCamConnected)
	{
		cout << "Disconnecting the device..." << endl;
		m_lResult = m_pDevice->Disconnect();
		if (!m_lResult.IsOK())
		{
			cout << "Error : Cannot disconnect device..." << endl;
			return FAIL;
		}
		else
		{
			m_camStat.isCamConnected = false;
			PvDevice::Free(m_pDevice);
			cout << "The device has been disconnected!!" << endl;
		}
	}
	else
	{
		cout << "The camera is already disconnected!!" << endl;
	}
	return OKAY;
}

int ITT_Camera::openStream() {

	PvResult lResult;

	// Open stream to the GigE Vision or USB3 Vision device
	cout << "Opening stream to device." << endl;
	m_pStream = PvStream::CreateAndOpen(CAM_IP, &lResult);
	if (m_pStream == NULL)
	{
		cout << "Unable to stream from " << CAM_IP << "." << endl;
		return FAIL;
	}

	return OKAY;
}

int ITT_Camera::configureStream() {

	PvDeviceGEV* lDeviceGEV = dynamic_cast<PvDeviceGEV *>(m_pDevice);
	PvStreamGEV *lStreamGEV = static_cast<PvStreamGEV *>(m_pStream);
	lDeviceGEV->NegotiatePacketSize();
	lDeviceGEV->SetStreamDestination(lStreamGEV->GetLocalIPAddress(), lStreamGEV->GetLocalPort());

	return OKAY;
}

int ITT_Camera::createPipeline() {

	m_pPipeline = new PvPipeline(m_pStream);

	if (m_pPipeline != NULL)
	{
		uint32_t lSize = m_pDevice->GetPayloadSize();

		m_pPipeline->SetBufferCount(BUFFER_COUNT);
		m_pPipeline->SetBufferSize(lSize);
		m_pDeviceParams = m_pDevice->GetParameters();
		m_pStreamParams = m_pStream->GetParameters();
	}
	else
	{
		cout << "Failed to allocate m_pPipeline..." << endl;
		return FAIL;
	}

	return OKAY;
}

int ITT_Camera::beforeAcquisition() {

	cout << "Starting pipeline..." << endl;
	m_pPipeline->Start();

	if (m_pDeviceParams->SetIntegerValue("TLParamsLocked", 1).IsOK())
	{
		cout << "Setting TLParamsLocked to 1..." << endl;
	}
	cout << "Resetting timestamp counter..." << endl;
	m_pDeviceParams->ExecuteCommand("GevTimestampControlReset");

	cout << "Sending StartAquisition command to device.." << endl;
	m_lResult = m_pDeviceParams->ExecuteCommand("AcquisitionStart");

	if (!m_lResult.IsOK())
	{
		cout << "Error : Cannot Start Acquisition..." << endl;
		return FAIL;
	}

	return OKAY;
}

int ITT_Camera::takePicture(int imgType) {
	beforeAcquisition();

	PvBuffer*	lBuffer = NULL;
	PvResult	lOperationResult;
	m_lResult = m_pPipeline->RetrieveNextBuffer(&lBuffer, 1000, &lOperationResult);


	//char Filename[100]={0,};
	BytesWritten = 0;

	if (m_lResult.IsOK())
	{
		if (lOperationResult.IsOK())
		{
			uint32_t lWidth = 0, lHeight = 0;
			if (lBuffer->GetPayloadType() == PvPayloadTypeImage)
			{
				PvImage* lImage = lBuffer->GetImage();
				lWidth = lImage->GetWidth();
				lHeight = lImage->GetHeight();
				cout << "width : " << lWidth << ", Height : " << lHeight << endl;
				filenameStamp(imgType);		/*Filename & extention decision*/
				cout << "image type : " << imgType << endl;
				PvBuffer* lBufferBGR8 = new PvBuffer();
				lBufferBGR8->GetImage()->Alloc(lWidth, lHeight, PvPixelBGR8);
				PvBufferConverter buffconv;
				m_lResult = buffconv.Convert(lBuffer, lBufferBGR8);

				Mat capturedImg(lHeight, lWidth, CV_8UC3, lBufferBGR8->GetDataPointer());

				imwrite(Filename, capturedImg);
				capturedImg.release();
				FILE *tmpf = fopen(Filename, "r");
				fseek(tmpf, 0, SEEK_END);
				BytesWritten = ftell(tmpf);
				fclose(tmpf);
				lBufferBGR8->Free();
				delete lBufferBGR8;
				//PvBufferWriter BW;

				//BW.Store(lBuffer, Filename, PvBufferFormatBMP, &BytesWritten);
				//cout << "Filesize :" << BytesWritten <<endl;
			}
		}
		m_pPipeline->ReleaseBuffer(lBuffer);
	}
	else {
		cout << "Cannot retrieve taken picture..." << endl;
		afterAcquisition();
		return FAIL;
	}
	lBuffer->Free();
	afterAcquisition();
	return OKAY;
}

int ITT_Camera::afterAcquisition() {
	
	cout << "Sending AcquisitionStop command to the device..." << endl;
	m_pDeviceParams->ExecuteCommand("AcquisitionStop");

	if (m_pDeviceParams->SetIntegerValue("TLParamsLocked", 0).IsOK())
	{
		cout << "Resetting TLParamsLocked to 0..." << endl;
	}
	cout << "Stop Pipeline.." << endl;
	m_pPipeline->Stop();
	//delete m_pPipeline;
	

	return OKAY;
}

int ITT_Camera::FileTransfer(uint8_t filetype) {

	//저장된 파일 보내기
	kaiMsg sendMsg;
	char buffer[10300];
	sendMsg.setBuffer(buffer, 10300);
	sendMsg.id(0x80);
	sendMsg.begin();
	uint8_t 	code = 0;
	uint8_t		type = filetype;
	uint32_t	filesize = BytesWritten;
	sendMsg << code << type << filesize;
	sendMsg.end();
	cout << "Sent bytes : " << m_sock.sendTo(sendMsg, CFS_IP, CFS_PORT) << endl;

	FILE *fp = fopen(Filename, "rb");
	if (fp == NULL)
	{
		cout << "error occured to open file." << endl;
		return FAIL;
	}
	else
	{
		saveFilename(filetype);

		while (!feof(fp))
		{
			char fbuffer[10240];
			memset(buffer, 0, 10300);
			size_t readCnt = fread(fbuffer, 1, 10240, fp);

			sendMsg.id(0x81);
			sendMsg.begin();
			for (uint16_t i = 0; i<readCnt; i++)
			{
				sendMsg << fbuffer[i];
			}
			sendMsg.end();
			m_sock.sendTo(sendMsg, CFS_IP, CFS_PORT);

			usleep(DELAY_SEND_USEC);
		}
		fclose(fp);
		fp = NULL;
		cout << "File has sent" << endl;
		sendMsg.id(0x80);
		sendMsg.begin();
		code = 1;
		sendMsg << code << type << filesize;
		sendMsg.end();
		cout << "Sent bytes : " << m_sock.sendTo(sendMsg, CFS_IP, CFS_PORT) << endl;

	}

	return OKAY;
}

int ITT_Camera::closePipeline() {

	cout << "Closing Stream.." << endl;
	m_pStream->Close();

	return OKAY;
}

int ITT_Camera::takeMovie() {

	int _res = OKAY;
	PvBuffer*	lBuffer = NULL;
	PvResult	lOperationResult;
	m_lResult = m_pPipeline->RetrieveNextBuffer(&lBuffer, 1000, &lOperationResult);
	double_t	lFrameRateVal = 0;

	if (m_lResult.IsOK())
	{
		if (lOperationResult.IsOK())
		{
			int64_t	frmrate;
			m_pDeviceParams->GetIntegerValue("CurrentFrameRate", frmrate);
			m_pStreamParams->GetFloatValue("AcquisitionRateAverage", lFrameRateVal);
			cout << "FrameRate : " << lFrameRateVal << "Hz\n";
			uint32_t lWidth = 0, lHeight = 0;
			if (lBuffer->GetPayloadType() == PvPayloadTypeImage)
			{
				PvImage* lImage = lBuffer->GetImage();
				lWidth = lImage->GetWidth();
				lHeight = lImage->GetHeight();

				PvBuffer* lBufferBGR8 = new PvBuffer();
				lBufferBGR8->GetImage()->Alloc(lWidth, lHeight, PvPixelBGR8);
				PvBufferConverter buffconv;
				m_lResult = buffconv.Convert(lBuffer, lBufferBGR8);

				Mat capturedImg(lHeight, lWidth, CV_8UC3, lBufferBGR8->GetDataPointer());

				if (!vwr.isOpened())
				{
					cout << "open" << endl;
					if (!vwr.open(Filename, CV_FOURCC('M', 'J', 'P', 'G'), (double_t)frmrate, Size(lWidth, lHeight)))
					{
						cout << endl << "Fail to open vwr..." << endl;
						_res = FAIL;
					}
				}
				else
				{
					vwr << capturedImg;
					cout << "Captured!!" << endl;
				}


				//PvBufferWriter BW;

				//BW.Store(lBuffer, Filename, PvBufferFormatBMP, &BytesWritten);
				//cout << "Filesize :" << BytesWritten <<endl;
			}
		}
		m_pPipeline->ReleaseBuffer(lBuffer);
	}

	return _res;
}

int ITT_Camera::controlCam(cam_control& controlParams) {

	int _res = OKAY;

	if (controlParams.resolution_mode != 0 &&		/*resolution control*/
		(controlParams.resolution_mode == 1 ||
			controlParams.resolution_mode == 2 ||
			controlParams.resolution_mode == 3 ||
			controlParams.resolution_mode == 4 ||
			controlParams.resolution_mode == 8))
	{
		PvResult	lResult1, lResult2;
		lResult1 = m_pDeviceParams->SetEnumValue("BinningHorizontal", controlParams.resolution_mode);
		lResult2 = m_pDeviceParams->SetEnumValue("BinningVertical", controlParams.resolution_mode);
		if (!lResult1.IsOK())
		{
			cout << "Cannot change binningHorizontal. " << endl;
			_res = FAIL;
		}
		if (!lResult2.IsOK())
		{
			cout << "Cannot change binningVertical. " << endl;
			_res = FAIL;
		}

		int64_t	width, height;
		m_pDeviceParams->GetIntegerValue("WidthMax", width);
		m_pDeviceParams->GetIntegerValue("HeightMax", height);
		m_pDeviceParams->SetIntegerValue("Width", width);
		m_pDeviceParams->SetIntegerValue("Height", height);
		_res = OKAY;
	}
	else
	{
		cout << "Something's wrong with resolution control.." << endl;
		_res = FAIL;
	}


	switch (controlParams.exposure_time)				/*Control Exposure time*/
	{
	case 0:	/*non-change*/
	{
	}
	break;

	case 0xFFFF:	/*Auto Exposure control on*/
	{
		m_pDeviceParams->SetBooleanValue("AecEnable", true);
		_res = OKAY;
	}
	break;

	default:		/*AEC Off & Set shutter speed*/
	{
		m_pDeviceParams->SetBooleanValue("AecEnable", false);
		m_pDeviceParams->SetEnumValue("ExposureMode", 1);
		m_pDeviceParams->SetIntegerValue("ExposureTimeRaw", controlParams.exposure_time);
		_res = OKAY;
	}
	break;
	}

	switch (controlParams.analog_gain)			/*Control Analog gain*/
	{
	case 0xFFFF:		/*non-change*/
	{
	}
	break;

	case 0xF0FF:		/*Auto Gain Control On*/
	{
		m_pDeviceParams->SetBooleanValue("AgcEnable", true);
		_res = OKAY;
	}
	break;

	default:			/*AGC Off & Set analog gain*/
	{
		m_pDeviceParams->SetBooleanValue("AgcEnable", false);
		m_pDeviceParams->SetEnumValue("GainSelector", 16);
		m_pDeviceParams->SetIntegerValue("GainRaw", controlParams.analog_gain);
		_res = OKAY;
	}
	break;
	}

	switch (controlParams.digital_gain)			/*Control Digital gain*/
	{
	case 0xFF:		/*non-change*/
	{
	}
	break;

	default:		/*Control digital gain*/
	{
		m_pDeviceParams->SetIntegerValue("DigitalGainAll", controlParams.digital_gain);
		_res = OKAY;
	}
	break;
	}

	switch (controlParams.frame_time)			/*Control Frame Acquisition Time*/
	{
	case 0:		/*non-change*/
	{
	}
	break;

	default:		/*Control frame acquisition time in usec*/
	{
		_res = OKAY;
		//m_pDeviceParams->SetIntegerValue("
	}
	break;
	}

	return OKAY;
}

int ITT_Camera::monitorCam() {
	return OKAY;
}

int ITT_Camera::SetRecv(string Addr, unsigned short Port) {

	m_sock.recvFrom(Addr, Port);

	return OKAY;
}

int ITT_Camera::filenameStamp(uint8_t type) {

	uint32_t	cnt = 0;
	char* strTime;
	char excode[4] = { 0, };
	switch (type)
	{
	case EXT_JPEG:/*jpg*/
	{
		sprintf(excode, "%s", "jpg");
		cnt = m_camStat.imageCount++;
	}
	break;
	case EXT_BITMAP:/*bmp*/
	{
		sprintf(excode, "%s", "bmp");
		cnt = m_camStat.imageCount++;
	}
	break;
	case EXT_JPEG2000:/*jpeg2000*/
	{
		sprintf(excode, "%s", "jpeg2000");
		cnt = m_camStat.imageCount++;
	}
	break;
	case EXT_AVI:/*avi*/
	{
		sprintf(excode, "%s", "avi");
		cnt = m_camStat.movieCount++;
	}
	break;
	case EXT_PNG:/*png*/
	{
		sprintf(excode, "%s", "png");
		cnt = m_camStat.imageCount++;
	}
	break;
	default:
	{
		cout << "something's wrong with filenamestamp" << endl;
	}
	break;
	}
	time(&m_camStat.timestamp);

	strTime = ctime(&m_camStat.timestamp);

	asprintf(&Filename, "../%s/Image_%s_%04d.%s", FILE_STORAGE_PATH, strTime, cnt, excode);
	cout << Filename << endl;

	return OKAY;
}

int ITT_Camera::saveFilename(uint8_t filetype_flag) {

	asprintf(&PastFilename, "%s", Filename);
	pastFiletype = filetype_flag;
	cout << "PastFilename : " << PastFilename << endl;

	return OKAY;
}

int ITT_Camera::setFilename() {

	asprintf(&Filename, "%s", PastFilename);
	cout << "Filename : " << Filename << endl;

	return OKAY;
}
