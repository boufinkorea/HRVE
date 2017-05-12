/*
 * HiREV_Eye.cpp
 * 
 *  Created on: 2017. 2. 10.
 *      Author: jinhkim
 */
#include "ITTCamera.h"
#include <iostream>

using namespace std;
PV_INIT_SIGNAL_HANDLER()
int main()
{
	ITT_Camera cam;
	cout << "HiREV_Eye has been started..." << endl;


	cam.init();
	cam.connectCam();
	cam.openStream();
	cam.configureStream();
	cam.createPipeline();
	//cam_control tmpcont;
	//tmpcont.resolution_mode = 1;
	//tmpcont.exposure_time=0;
	//tmpcont.analog_gain=0xFF;
	//tmpcont.digital_gain=0xFF;
	//tmpcont.frame_time=0;
	//cam.controlCam(tmpcont);

	//cam.takePicture();
	while (PvKbHit() != 1)
	{

		string fromAddr = CFS_IP;
		unsigned short fromPort = CFS_PORT;
		cam.SetRecv(fromAddr, fromPort);
		usleep(100);
	}
	cam.closePipeline();

	cam.disconnectCam();

	return 0;
}
