// wiiremote.h
//Ivica Ico Bukvic and Ji-Sun Kim
//http://disis.music.vt.edu
//Based on 1.0B7 by Masayuki Akamatsu

// Copyright by Masayuki Akamatsu
// Based on "DarwiinRemote" by Hiroaki Kimura

#include <IOBluetooth/IOBluetoothUserLib.h>

typedef unsigned char WiiIRModeType;
enum {
	kWiiIRModeBasic			= 0x01,
	kWiiIRModeExtended		= 0x03,
	kWiiIRModeFull			= 0x05
};

typedef struct {
	int x, y, s;
} IRData;

typedef struct {
	unsigned char accX_zero, accY_zero, accZ_zero, accX_1g, accY_1g, accZ_1g;
} WiiAccCalibData;

typedef struct {
	unsigned char x_min, x_max, x_center, y_min, y_max, y_center;
} WiiJoyStickCalibData;

//KJS
typedef struct {
	unsigned char tr_zero_1, tr_zero_2, br_zero_1, br_zero_2, tl_zero_1, tl_zero_2, bl_zero_1, bl_zero_2; // 0kg value
	unsigned char tr_17_1, tr_17_2, br_17_1, br_17_2, tl_17_1, tl_17_2, bl_17_1, bl_17_2; // 17kg value
	unsigned char tr_34_1, tr_34_2, br_34_1, br_34_2, tl_34_1, tl_34_2, bl_34_1, bl_34_2; // 34kg value
} WiiBalanceBoardCalibData;

typedef struct {
	unsigned short TR[3], BR[3], TL[3], BL[3];
} WiiBalanceBoardCalibData2;

typedef UInt16 WiiButtonType;
enum {
	WiiRemoteAButton,
	WiiRemoteBButton,
	WiiRemoteOneButton,
	WiiRemoteTwoButton,
	WiiRemoteMinusButton,
	WiiRemoteHomeButton,
	WiiRemotePlusButton,
	WiiRemoteUpButton,
	WiiRemoteDownButton,
	WiiRemoteLeftButton,
	WiiRemoteRightButton,

	WiiNunchukZButton,
	WiiNunchukCButton,

	WiiClassicControllerXButton,
	WiiClassicControllerYButton,
	WiiClassicControllerAButton,
	WiiClassicControllerBButton,
	WiiClassicControllerLButton,
	WiiClassicControllerRButton,
	WiiClassicControllerZLButton,
	WiiClassicControllerZRButton,
	WiiClassicControllerUpButton,
	WiiClassicControllerDownButton,
	WiiClassicControllerLeftButton,
	WiiClassicControllerRightButton,
	WiiClassicControllerMinusButton,
	WiiClassicControllerHomeButton,
	WiiClassicControllerPlusButton
};

typedef UInt16 WiiExpansionPortType;
enum{
	WiiExpNotAttached,
	WiiNunchuk,
	WiiClassicController,
	WiiBalanceBoard //KJS
};

typedef UInt16 WiiAccelerationSensorType;
enum{
	WiiRemoteAccelerationSensor,
	WiiNunchukAccelerationSensor
};


typedef UInt16 WiiJoyStickType;
enum{
	WiiNunchukJoyStick,
	WiiClassicControllerLeftJoyStick,
	WiiClassicControllerRightJoyStick
};


typedef struct _WiiRemoteRec
{
	IOBluetoothDeviceInquiryRef	inquiry;
	IOBluetoothDeviceRef		device;
	IOBluetoothL2CAPChannelRef	ichan;
	IOBluetoothL2CAPChannelRef	cchan;

	CFStringRef		address;

	unsigned char	accX;
	unsigned char	accY;
	unsigned char	accZ;
	unsigned short	buttonData;

	float			lowZ;
	float			lowX;
	int				orientation;
	int				leftPoint; // is point 0 or 1 on the left. -1 when not tracking.

	float			posX;
	float			posY;
	float			angle;
	Boolean			tracking;

	WiiExpansionPortType expType;
	WiiAccCalibData	wiiCalibData, nunchukCalibData;
	WiiJoyStickCalibData nunchukJoyStickCalibData;

	WiiBalanceBoardCalibData wiiBalanceBoardCalibData;
	WiiBalanceBoardCalibData2 wiiBalanceBoardCalibData2;

	WiiIRModeType	wiiIRMode;
	IRData			irData[4];
	double			batteryLevel;

	Boolean			readingRegister;
	Boolean			isMotionSensorEnabled;
	Boolean			isIRSensorEnabled;
	Boolean			isVibrationEnabled;
	Boolean			isExpansionPortEnabled;
	Boolean			initExpPort;
	Boolean			isLED1Illuminated;
	Boolean			isLED2Illuminated;
	Boolean			isLED3Illuminated;
	Boolean			isLED4Illuminated;
	Boolean			isExtraOutputEnabled;

	Boolean			isExpansionPortAttached;

	IOBluetoothUserNotificationRef	disconnectNotification;

	//nunchuk
	unsigned char	nStickX;
	unsigned char	nStickY;
	unsigned char	nAccX;
	unsigned char	nAccY;
	unsigned char	nAccZ;
	unsigned char	nButtonData;

	float			nLowZ;
	float			nLowX;
	int				nOrientation;

	//classic controller
	unsigned short	cButtonData;
	unsigned char	cStickX1;
	unsigned char	cStickY1;
	unsigned char	cStickX2;
	unsigned char	cStickY2;
	unsigned char	cAnalogL;
	unsigned char	cAnalogR;

	//long			raw[8];
	unsigned char	raw[8];
	short  raw_data[4];
	float  kg_data[4];
	float  lb_data[4];
	
	float  weightKG;
	float  weightLB;
	
	float centerGravity_x, centerGravity_y;
	long  interpolationFactor;
	
	unsigned char calibration_data[24];
	long calibrate;

}	WiiRemoteRec, *WiiRemoteRef;

void			wiiremote_init(WiiRemoteRef wiiremote);
Boolean			wiiremote_isconnected(WiiRemoteRef wiiremote);
Boolean			wiiremote_search(WiiRemoteRef wiiremote, char *address);
Boolean			wiiremote_stopsearch(WiiRemoteRef wiiremote);
Boolean			wiiremote_connect(WiiRemoteRef wiiremote);
Boolean			wiiremote_disconnect(WiiRemoteRef wiiremote);
void			wiiremote_getaddress(WiiRemoteRef wiiremote, char *address);
Boolean			wiiremote_motionsensor(WiiRemoteRef wiiremote, Boolean enabled);
Boolean			wiiremote_irsensor(WiiRemoteRef wiiremote, Boolean enabled);
Boolean			wiiremote_vibration(WiiRemoteRef wiiremote, Boolean enabled);
Boolean			wiiremote_led(WiiRemoteRef wiiremote, Boolean enabled1, Boolean enabled2, Boolean enabled3, Boolean enabled4);
Boolean			wiiremote_expansion(WiiRemoteRef wiiremote, Boolean enabled);
Boolean			wiiremote_getstatus(WiiRemoteRef wiiremote);


