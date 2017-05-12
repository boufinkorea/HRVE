#define CAM_IP	"192.168.0.100"
#define PAYLOAD_PORT	6150
#define BUFFER_COUNT ( 16 )



#define	COMM_INIT	0
#define	COMM_BUSY	1
#define	COMM_WAIT	2


#define CFS_IP	"192.168.0.32"
#define CFS_PORT	5150

#define DELAY_SEND_USEC	1000

#define OKAY	0
#define FAIL	1


#define EXT_JPEG		0
#define	EXT_BITMAP		1
#define	EXT_JPEG2000	2
#define	EXT_AVI			3
#define	EXT_PNG			4

#define	FILE_IMAGE		0
#define	FILE_MOVIE		1



struct cam_status
{
	bool isCamConnected;
	uint32_t	imageCount;		/*Image numbers has been taken*/
	uint32_t	movieCount;		/*Movie numbers has been taken*/
	time_t		timestamp;		/*stamp of current time*/
	time_t		startTimestamp;	/*stamp of misstion start time*/
	time_t		endTimestamp;	/*stamp of mission end time*/
	bool		isBind;			
	bool		imageFault;
	bool		movieFault;
};


struct cam_control
{
	uint8_t	resolution_mode;	//resolution using binning mode
								//1, 2, 3, 4, 8
								//0 : non-change
								
	uint32_t	exposure_time;	//shutter speed for exposure control
								//in usec
								//0 : non-change, 0xFFFF : AEC
								
	uint16_t	analog_gain;	//Analog gain
								//0~1023
								//0xFFFF: non-change
								
	uint8_t		digital_gain;	//Digital gain
								//0~20
								//0xFF:	non-change
								
	uint32_t	frame_time;		//FrameTime
								//minFrametime ~ 16777215
								//in usec
								//0 : non-change
};
