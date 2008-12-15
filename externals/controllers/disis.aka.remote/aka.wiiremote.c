//aka.wiiremote.c
//1.00 : 2008.12.10
//Ivica Ico Bukvic and Ji-Sun Kim with contributions from Sabrina Killian
//http://disis.music.vt.edu
//Based on 1.0B7 by Masayuki Akamatsu

// Copyright by Masayuki Akamatsu
// Code for PD by Hans-Christoph Steiner
// 1.0B1 : 2006.12.12
// 1.0B2 : 2006.12.15
// 1.0B3 : 2006.12.20
// 1.0B4 : 2006.12.24
// 1.0B5 : 2007.02.03
// 1.0B6 : 2007.04.24

#ifdef PD
#include "m_pd.h"
#define SETLONG SETFLOAT
static t_class *wiiremote_class;
#else /* Max */
#include "ext.h"
#endif /* PD */

#include "wiiremote.h"
#include <stdio.h>

#define kInterval	100
#define	kMaxTrial	100

typedef struct _akawiiremote
{
#ifdef PD
	t_object        x_obj;
#else /* Max */
	struct object	obj;
#endif

	WiiRemoteRef	wiiremote;
	char			address[32];

	void			*clock;
	Boolean			connected;

	void			*statusOut;
	void			*dataOut;
} t_akawiiremote;

void *akawiiremote_class;	// the number of instance of this object

void akawiiremote_bang(t_akawiiremote *x);
void akawiiremote_address(t_akawiiremote *x, t_symbol *s);
void akawiiremote_connect(t_akawiiremote *x);
void akawiiremote_disconnect(t_akawiiremote *x);
void akawiiremote_motionsensor(t_akawiiremote *x, long enable);
void akawiiremote_irsensor(t_akawiiremote *x, long enable);
void akawiiremote_vibration(t_akawiiremote *x, long enable);
void akawiiremote_led(t_akawiiremote *x, long enable1, long enable2, long enable3, long enable4);
void akawiiremote_expansion(t_akawiiremote *x, long enable);
void akawiiremote_extraoutput(t_akawiiremote *x, long enable);

//added
void akawiiremote_wiifit_scale(t_akawiiremote *x, long value);

void akawiiremote_getbattery(t_akawiiremote *x);
void akawiiremote_getexpansion(t_akawiiremote *x);
void akawiiremote_getled(t_akawiiremote *x);
void akawiiremote_getaddress(t_akawiiremote *x);
void akawiiremote_getcalibration(t_akawiiremote *x);

void akawiiremote_assist(t_akawiiremote *x, void *b, long m, long a, char *s);
void akawiiremote_clock(t_akawiiremote *x);
void *akawiiremote_new(t_symbol *s, short ac, t_atom *av);
void akawiiremote_free(t_akawiiremote *x);

char	remoteStr[] = "remote";
char	nunchukStr[] = "nunchuk";
char	classicStr[] = "classic";

//KJS
char	wiifitStr[] = "wiifit";

#ifdef PD
void wiiremote_setup()
#else /* Max */
int main()
#endif /* PD */
{
	NumVersion				outSoftwareVersion;
	BluetoothHCIVersionInfo	outHardwareVersion;

	post("disis.aka.wiiremote 1.00 (based on aka.wiimote 1.0B7-UB by Masayuki Akamatsu)");

	if (IOBluetoothGetVersion(&outSoftwareVersion, &outHardwareVersion)==kIOReturnSuccess)	// B7
	{
		if (outSoftwareVersion.majorRev < 1 && outSoftwareVersion.minorAndBugRev < 0x63)
		{
			error("requires Blutooth version 1.6.3 or later.");
			return;
		}
	}
	else
	{
		error("can't get Bluetooth version.");
		return;
	}

#ifdef PD
	post("\tPd port by Hans-Christoph Steiner");

	wiiremote_class = class_new(gensym("wiiremote"),
								 (t_newmethod)akawiiremote_new,
								 (t_method)akawiiremote_free,
								 sizeof(t_akawiiremote),
								 CLASS_DEFAULT,
								 A_GIMME,0);

	class_addbang(wiiremote_class,(t_method)akawiiremote_bang);
	class_addmethod(wiiremote_class,(t_method)akawiiremote_connect,gensym("connect"),0);
	class_addmethod(wiiremote_class,(t_method)akawiiremote_disconnect,gensym("disconnect"),0);
	class_addmethod(wiiremote_class,(t_method)akawiiremote_motionsensor,gensym("motion"), A_DEFFLOAT, 0);
	class_addmethod(wiiremote_class,(t_method)akawiiremote_irsensor,gensym("ir"), A_DEFFLOAT, 0);
	class_addmethod(wiiremote_class,(t_method)akawiiremote_vibration,gensym("vibration"), A_DEFFLOAT, 0);
	class_addmethod(wiiremote_class,(t_method)akawiiremote_led,gensym("led"), A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, 0);

	class_addmethod(wiiremote_class,(t_method)akawiiremote_getbattery,gensym("getbattery"),0);
	class_addmethod(wiiremote_class,(t_method)akawiiremote_getexpansion,gensym("getexpansion"),0);
	class_addmethod(wiiremote_class,(t_method)akawiiremote_getled,gensym("getled"),0);

	class_addmethod(wiiremote_class,(t_method)akawiiremote_assist,gensym("assist"),A_CANT,0);
#else /* Max */
	setup((t_messlist **)&akawiiremote_class, (method)akawiiremote_new, (method)akawiiremote_free, (short)sizeof(t_akawiiremote), 0L, A_GIMME, 0);

	addbang((method)akawiiremote_bang);
	addmess((method)akawiiremote_address,"address",A_DEFSYM, 0);
	addmess((method)akawiiremote_connect,"connect", 0);
	addmess((method)akawiiremote_disconnect,"disconnect",0);
	addmess((method)akawiiremote_motionsensor,"motion", A_DEFLONG, 0);
	addmess((method)akawiiremote_irsensor,"ir", A_DEFLONG, 0);
	addmess((method)akawiiremote_vibration,"vibration", A_DEFLONG, 0);
	addmess((method)akawiiremote_led,"led", A_DEFLONG, A_DEFLONG, A_DEFLONG, A_DEFLONG, 0);
	addmess((method)akawiiremote_expansion,"expansion", A_DEFLONG, 0);
	addmess((method)akawiiremote_wiifit_scale,"wiifit_scale", A_DEFLONG, 0);
	addmess((method)akawiiremote_expansion,"nunchuk", A_DEFLONG, 0);
	addmess((method)akawiiremote_extraoutput,"extraoutput", A_DEFLONG, 0);	// B7

	addmess((method)akawiiremote_getbattery,"getbattery",0);
	addmess((method)akawiiremote_getexpansion,"getexpansion",0);
	addmess((method)akawiiremote_getled,"getled",0);
	addmess((method)akawiiremote_getaddress,"getaddress",0);
	addmess((method)akawiiremote_getcalibration,"getcalibration", 0);

	addmess((method)akawiiremote_assist,"assist",A_CANT,0);
#endif /* PD */
}

//--------------------------------------------------------------------------------------------

void akawiiremote_bang(t_akawiiremote *x)
{
	t_atom av[7];
	t_atom tmp[25];
	int k;

	if (x->wiiremote->device == nil)
		return;	// do nothing

#ifdef PD
	outlet_float(x->buttonsOut, (t_float) x->wiiremote->buttonData);
#else /* Max */
#endif /* PD */

	if (x->wiiremote->isExpansionPortAttached && x->wiiremote->isExpansionPortEnabled)
	{
		// Classic Controller
		if (x->wiiremote->expType == WiiClassicController)
		{
			// Buttons
			SETSYM(av, gensym("buttons"));
			SETLONG(av + 1, x->wiiremote->cButtonData);
			outlet_anything(x->dataOut, gensym(classicStr), 2, av);

			// Joystick 1
			SETSYM(av, gensym("stick1"));
			SETLONG(av + 1, x->wiiremote->cStickX1);
			SETLONG(av + 2, x->wiiremote->cStickY1);
			outlet_anything(x->dataOut, gensym(classicStr), 3, av);

			// Joystick 2
			SETSYM(av, gensym("stick2"));
			SETLONG(av + 1, x->wiiremote->cStickX2);
			SETLONG(av + 2, x->wiiremote->cStickY2);
			outlet_anything(x->dataOut, gensym(classicStr), 3, av);

			// Analog
			SETSYM(av, gensym("analog"));
			SETLONG(av + 1, x->wiiremote->cAnalogL);
			SETLONG(av + 2, x->wiiremote->cAnalogR);
			outlet_anything(x->dataOut, gensym(classicStr), 3, av);
		}
		
		if (x->wiiremote->expType == WiiBalanceBoard)
		{
			SETSYM(av, gensym("sensors")); //TL TR BL BR
			SETFLOAT(av + 1, x->wiiremote->kg_data[2]);
			SETFLOAT(av + 2, x->wiiremote->kg_data[0]);
			SETFLOAT(av + 3, x->wiiremote->kg_data[3]);
			SETFLOAT(av + 4, x->wiiremote->kg_data[1]);
			outlet_anything(x->dataOut, gensym(wiifitStr), 5, av);

			SETSYM(av, gensym("weight"));
			SETSYM(av + 1, gensym("kg"));
			SETFLOAT(av + 2, x->wiiremote->weightKG);
			outlet_anything(x->dataOut, gensym(wiifitStr), 3, av);
			
			SETSYM(av, gensym("weight"));
			SETSYM(av + 1, gensym("lb"));
			SETFLOAT(av + 2, x->wiiremote->weightLB);
			outlet_anything(x->dataOut, gensym(wiifitStr), 3, av);
			
			SETSYM(av, gensym("center")); //X,Y
			SETFLOAT(av + 1, x->wiiremote->centerGravity_x);
			SETFLOAT(av + 2, x->wiiremote->centerGravity_y);
			outlet_anything(x->dataOut, gensym(wiifitStr), 3, av);
			
			if (x->wiiremote->isExtraOutputEnabled)
			{
				//calibration test for balance board
				SETSYM(av, gensym("calib"));
				SETSYM(av + 1, gensym("TR_0KG"));
				SETLONG(av + 2, x->wiiremote->wiiBalanceBoardCalibData2.TR[0]);
				outlet_anything(x->dataOut, gensym(wiifitStr), 3, av);

				SETSYM(av, gensym("calib"));
				SETSYM(av + 1, gensym("BR_0KG"));
				SETLONG(av + 2, x->wiiremote->wiiBalanceBoardCalibData2.BR[0]);
				outlet_anything(x->dataOut, gensym(wiifitStr), 3, av);

				SETSYM(av, gensym("calib"));
				SETSYM(av + 1, gensym("TL_0KG"));
				SETLONG(av + 2, x->wiiremote->wiiBalanceBoardCalibData2.TL[0]);
				outlet_anything(x->dataOut, gensym(wiifitStr), 3, av);

				SETSYM(av, gensym("calib"));
				SETSYM(av + 1, gensym("BL_0KG"));
				SETLONG(av + 2, x->wiiremote->wiiBalanceBoardCalibData2.BL[0]);
				outlet_anything(x->dataOut, gensym(wiifitStr), 3, av);

				SETSYM(av, gensym("calib"));
				SETSYM(av + 1, gensym("TR_17KG"));
				SETLONG(av + 2, x->wiiremote->wiiBalanceBoardCalibData2.TR[1]);
				outlet_anything(x->dataOut, gensym(wiifitStr), 3, av);

				SETSYM(av, gensym("calib"));
				SETSYM(av + 1, gensym("BR_17KG"));
				SETLONG(av + 2, x->wiiremote->wiiBalanceBoardCalibData2.BR[1]);
				outlet_anything(x->dataOut, gensym(wiifitStr), 3, av);

				SETSYM(av, gensym("calib"));
				SETSYM(av + 1, gensym("TL_17KG"));
				SETLONG(av + 2, x->wiiremote->wiiBalanceBoardCalibData2.TL[1]);
				outlet_anything(x->dataOut, gensym(wiifitStr), 3, av);

				SETSYM(av, gensym("calib"));
				SETSYM(av + 1, gensym("BL_17KG"));
				SETLONG(av + 2, x->wiiremote->wiiBalanceBoardCalibData2.BL[1]);
				outlet_anything(x->dataOut, gensym(wiifitStr), 3, av);

				SETSYM(av, gensym("calib"));
				SETSYM(av + 1, gensym("TR_34KG"));
				SETLONG(av + 2, x->wiiremote->wiiBalanceBoardCalibData2.TR[2]);
				outlet_anything(x->dataOut, gensym(wiifitStr), 3, av);

				SETSYM(av, gensym("calib"));
				SETSYM(av + 1, gensym("BR_34KG"));
				SETLONG(av + 2, x->wiiremote->wiiBalanceBoardCalibData2.BR[2]);
				outlet_anything(x->dataOut, gensym(wiifitStr), 3, av);

				SETSYM(av, gensym("calib"));
				SETSYM(av + 1, gensym("TL_34KG"));
				SETLONG(av + 2, x->wiiremote->wiiBalanceBoardCalibData2.TL[2]);
				outlet_anything(x->dataOut, gensym(wiifitStr), 3, av);

				SETSYM(av, gensym("calib"));
				SETSYM(av + 1, gensym("BL_34KG"));
				SETLONG(av + 2, x->wiiremote->wiiBalanceBoardCalibData2.BL[2]);
				outlet_anything(x->dataOut, gensym(wiifitStr), 3, av);
				
				//raw test for balance board
				SETSYM(av, gensym("raw"));
				SETSYM(av + 1, gensym("TR"));
				SETLONG(av + 2, x->wiiremote->raw_data[0]);
				outlet_anything(x->dataOut, gensym(wiifitStr), 3, av);
				SETSYM(av, gensym("raw"));
				SETSYM(av + 1, gensym("BR"));
				SETLONG(av + 2, x->wiiremote->raw_data[1]);
				outlet_anything(x->dataOut, gensym(wiifitStr), 3, av);
				SETSYM(av, gensym("raw"));
				SETSYM(av + 1, gensym("TL"));
				SETLONG(av + 2, x->wiiremote->raw_data[1]);
				outlet_anything(x->dataOut, gensym(wiifitStr), 3, av);
				SETSYM(av, gensym("raw"));
				SETSYM(av + 1, gensym("BL"));
				SETLONG(av + 2, x->wiiremote->raw_data[1]);
				outlet_anything(x->dataOut, gensym(wiifitStr), 3, av);
			}
			
			//Short Type
			/*
			SETSYM(av, gensym("calib"));
			SETSYM(av + 1, gensym("Cal_TR_Zero2"));
			SETLONG(av + 2, x->wiiremote->wiiBalanceBoardCalibData2.TR[0]);
			outlet_anything(x->dataOut, gensym(wiifitStr), 3, av);

			SETSYM(av + 1, gensym("Cal_BR_Zero2"));
			SETLONG(av + 2, x->wiiremote->wiiBalanceBoardCalibData2.BR[0]);
			outlet_anything(x->dataOut, gensym(wiifitStr), 3, av);

			SETSYM(av + 1, gensym("Cal_TL_Zero2"));
			SETLONG(av + 2, x->wiiremote->wiiBalanceBoardCalibData2.TL[0]);
			outlet_anything(x->dataOut, gensym(wiifitStr), 3, av);

			SETSYM(av + 1, gensym("Cal_BL_Zero2"));
			SETLONG(av + 2, x->wiiremote->wiiBalanceBoardCalibData2.BL[0]);
			outlet_anything(x->dataOut, gensym(wiifitStr), 3, av);

			SETSYM(av + 1, gensym("Cal_TL_17_2"));
			SETLONG(av + 2, x->wiiremote->wiiBalanceBoardCalibData2.TL[1]);
			outlet_anything(x->dataOut, gensym(wiifitStr), 3, av);

			SETSYM(av + 1, gensym("Cal_BL_17_2"));
			SETLONG(av + 2, x->wiiremote->wiiBalanceBoardCalibData2.BL[1]);
			outlet_anything(x->dataOut, gensym(wiifitStr), 3, av);

			SETSYM(av + 1, gensym("Cal_BR_17_2"));
			SETLONG(av + 2, x->wiiremote->wiiBalanceBoardCalibData2.BR[1]);
			outlet_anything(x->dataOut, gensym(wiifitStr), 3, av);
			SETSYM(av + 1, gensym("Cal_TR_17_2"));
			SETLONG(av + 2, x->wiiremote->wiiBalanceBoardCalibData2.TR[1]);
			outlet_anything(x->dataOut, gensym(wiifitStr), 3, av);

			SETSYM(av + 1, gensym("Cal_TR_34_2"));
			SETLONG(av + 2, x->wiiremote->wiiBalanceBoardCalibData2.TR[2]);
			outlet_anything(x->dataOut, gensym(wiifitStr), 3, av);

			SETSYM(av + 1, gensym("Cal_TL_34_2"));
			SETLONG(av + 2, x->wiiremote->wiiBalanceBoardCalibData2.TL[2]);
			outlet_anything(x->dataOut, gensym(wiifitStr), 3, av);

			SETSYM(av + 1, gensym("Cal_BR_34_2"));
			SETLONG(av + 2, x->wiiremote->wiiBalanceBoardCalibData2.BR[2]);
			outlet_anything(x->dataOut, gensym(wiifitStr), 3, av);
			
			SETSYM(av + 1, gensym("Cal_BL_34_2"));
			SETLONG(av + 2, x->wiiremote->wiiBalanceBoardCalibData2.BL[2]);
			outlet_anything(x->dataOut, gensym(wiifitStr), 3, av);
			*/
			
			/*
			SETSYM(tmp , gensym("dump_data"));
			for (k=0; k<16; k++) {
					for (j=0; j<16; j++) {
							SETLONG(tmp + k*16 + j + 1, x->wiiremote->dump_data[j+k*16]);
					}
					SETSYM(tmp + k*16 +j + 1, gensym("\n"));	
			}
			outlet_anything(x->dataOut, gensym(wiifitStr), 273, tmp);
			*/
			
			/*
			SETSYM(tmp , gensym("dump_data"));
			for (k=0; k<24; k++) {
					SETLONG(tmp + k + 1, x->wiiremote->calibration_data[k]);
			}
			outlet_anything(x->dataOut, gensym(wiifitStr), 25, tmp);
			 */
		}

		// Nunchuk
		if (x->wiiremote->expType == WiiNunchuk)
		{
			// Buttons
			SETSYM(av, gensym("buttons"));
			SETLONG(av + 1, x->wiiremote->nButtonData);
			outlet_anything(x->dataOut, gensym(nunchukStr), 2, av);

			// Joystick
			SETSYM(av, gensym("stick"));
			SETLONG(av + 1, x->wiiremote->nStickX);
			SETLONG(av + 2, x->wiiremote->nStickY);
			outlet_anything(x->dataOut, gensym(nunchukStr), 3, av);

			if (x->wiiremote->isExtraOutputEnabled)
			{
				SETSYM(av, gensym("stick_calibration"));
				SETLONG(av + 1, x->wiiremote->nunchukJoyStickCalibData.x_min);
				SETLONG(av + 2, x->wiiremote->nunchukJoyStickCalibData.x_max);
				SETLONG(av + 3, x->wiiremote->nunchukJoyStickCalibData.x_center);
				SETLONG(av + 4, x->wiiremote->nunchukJoyStickCalibData.y_min);
				SETLONG(av + 5, x->wiiremote->nunchukJoyStickCalibData.y_max);
				SETLONG(av + 6, x->wiiremote->nunchukJoyStickCalibData.y_center);
				outlet_anything(x->dataOut, gensym(nunchukStr), 7, av);
			}

			// Motion Sensor
			if (x->wiiremote->isMotionSensorEnabled)
			{
				SETSYM(av, gensym("motion"));
				SETLONG(av + 1, x->wiiremote->nAccX);
				SETLONG(av + 2, x->wiiremote->nAccY);
				SETLONG(av + 3, x->wiiremote->nAccZ);
				SETLONG(av + 4, x->wiiremote->nOrientation);
				outlet_anything(x->dataOut, gensym(nunchukStr), 5, av);

				if (x->wiiremote->isExtraOutputEnabled)
				{
					SETSYM(av, gensym("motion_calibration"));
					SETLONG(av + 1, x->wiiremote->nunchukCalibData.accX_zero);
					SETLONG(av + 2, x->wiiremote->nunchukCalibData.accY_zero);
					SETLONG(av + 3, x->wiiremote->nunchukCalibData.accZ_zero);
					SETLONG(av + 4, x->wiiremote->nunchukCalibData.accX_1g);
					SETLONG(av + 5, x->wiiremote->nunchukCalibData.accY_1g);
					SETLONG(av + 6, x->wiiremote->nunchukCalibData.accZ_1g);
					outlet_anything(x->dataOut, gensym(nunchukStr), 7, av);
				}
			}
		}
	}

	// Wii Remote

	// Buttons
	SETSYM(av, gensym("buttons"));
	SETLONG(av + 1, x->wiiremote->buttonData);
	outlet_anything(x->dataOut, gensym(remoteStr), 2, av);

	// IR Sensor
	if (x->wiiremote->isIRSensorEnabled)
	{
		SETSYM(av, gensym("ir"));
		SETFLOAT(av + 1, x->wiiremote->posX);	// posX and posY are "float"????
		SETFLOAT(av + 2, x->wiiremote->posY);
		SETFLOAT(av + 3, x->wiiremote->angle);
		SETLONG (av + 4, x->wiiremote->tracking);
		outlet_anything(x->dataOut, gensym(remoteStr), 5, av);

		if (x->wiiremote->isExtraOutputEnabled)	// B7
		{
			SETSYM(av, gensym("irraw"));
			SETLONG(av + 1, 0);
			SETLONG(av + 2, x->wiiremote->irData[0].x);
			SETLONG(av + 3, x->wiiremote->irData[0].y);
			SETLONG(av + 4, x->wiiremote->irData[0].s);
			outlet_anything(x->dataOut, gensym(remoteStr), 5, av);
			SETLONG(av + 1, 1);
			SETLONG(av + 2, x->wiiremote->irData[1].x);
			SETLONG(av + 3, x->wiiremote->irData[1].y);
			SETLONG(av + 4, x->wiiremote->irData[1].s);
			outlet_anything(x->dataOut, gensym(remoteStr), 5, av);
			SETLONG(av + 1, 2);
			SETLONG(av + 2, x->wiiremote->irData[2].x);
			SETLONG(av + 3, x->wiiremote->irData[2].y);
			SETLONG(av + 4, x->wiiremote->irData[2].s);
			outlet_anything(x->dataOut, gensym(remoteStr), 5, av);
			SETLONG(av + 1, 3);
			SETLONG(av + 2, x->wiiremote->irData[3].x);
			SETLONG(av + 3, x->wiiremote->irData[3].y);
			SETLONG(av + 4, x->wiiremote->irData[3].s);
			outlet_anything(x->dataOut, gensym(remoteStr), 5, av);
		}
	}

	// Motion Sensor
	if (x->wiiremote->isMotionSensorEnabled)
	{
		SETSYM(av, gensym("motion"));
		SETLONG(av + 1, x->wiiremote->accX);
		SETLONG(av + 2, x->wiiremote->accY);
		SETLONG(av + 3, x->wiiremote->accZ);
		SETLONG(av + 4, x->wiiremote->orientation);
		outlet_anything(x->dataOut, gensym(remoteStr), 5, av);

		if (x->wiiremote->isExtraOutputEnabled)	// B7
		{
			SETSYM(av, gensym("motion_calibration"));
			SETLONG(av + 1, x->wiiremote->wiiCalibData.accX_zero);
			SETLONG(av + 2, x->wiiremote->wiiCalibData.accY_zero);
			SETLONG(av + 3, x->wiiremote->wiiCalibData.accZ_zero);
			SETLONG(av + 4, x->wiiremote->wiiCalibData.accX_1g);
			SETLONG(av + 5, x->wiiremote->wiiCalibData.accY_1g);
			SETLONG(av + 6, x->wiiremote->wiiCalibData.accZ_1g);
			outlet_anything(x->dataOut, gensym(remoteStr), 7, av);
		}
	}
}

//--------------------------------------------------------------------------------------------

void akawiiremote_address(t_akawiiremote *x, t_symbol *s)
{
	if (*(s->s_name) == 0)	// if null string
		*(x->address) = 0;
	else
		strcpy(x->address, s->s_name);
}

//--------------------------------------------------------------------------------------------

void akawiiremote_connect(t_akawiiremote *x)
{
	t_atom	status;
	Boolean	result;

	if (wiiremote_isconnected(x->wiiremote))
	{
		SETLONG(&status, -1);
		outlet_anything(x->statusOut, gensym("connect"), 1, &status);
	}
	else
	{
		result = wiiremote_search(x->wiiremote, x->address);	// start searching the device
		x->connected = false;
		clock_unset(x->clock);			// stop clock
		clock_delay(x->clock, 0.5);		// start clock to check the device found
	}
}

void akawiiremote_foundFunc(t_akawiiremote *x)
{
}

void akawiiremote_disconnect(t_akawiiremote *x)
{
	Boolean	result;
	t_atom	status;

	clock_unset(x->clock);		// stop clock
	wiiremote_stopsearch(x->wiiremote);

	result = wiiremote_disconnect(x->wiiremote);
	SETLONG(&status, result);
	outlet_anything(x->statusOut, gensym("disconnect"), 1, &status);

	x->connected = !result;
}

//--------------------------------------------------------------------------------------------

void akawiiremote_motionsensor(t_akawiiremote *x, long enable)
{
	Boolean	result;
	t_atom	status;

	result = wiiremote_motionsensor(x->wiiremote, enable);
	//SETLONG(&status, result);
	//outlet_anything(x->statusOut, gensym("motion"), 1, &status);
}

void akawiiremote_irsensor(t_akawiiremote *x, long enable)
{
	Boolean	result;
	t_atom	status;

	result = wiiremote_irsensor(x->wiiremote, enable);
	//SETLONG(&status, result);
	//outlet_anything(x->statusOut, gensym("ir"), 1, &status);
}

void akawiiremote_extraoutput(t_akawiiremote *x, long enable)	// B7
{
	x->wiiremote->isExtraOutputEnabled = enable;
}

void akawiiremote_expansion(t_akawiiremote *x, long enable)
{
	Boolean	result;
	t_atom	status;

	result = wiiremote_expansion(x->wiiremote, enable);
	//SETLONG(&status, result);
	//outlet_anything(x->statusOut, gensym("nunchuk"), 1, &status);
}

void akawiiremote_wiifit_scale(t_akawiiremote *x, long value)
{
	setInterpolationFactor(x->wiiremote, value);
}

void akawiiremote_vibration(t_akawiiremote *x, long enable)
{
	Boolean	result;
	t_atom	status;

	result = wiiremote_vibration(x->wiiremote, enable);
	//SETLONG(&status, result);
	//outlet_anything(x->statusOut, gensym("vibration"), 1, &status);
}

void akawiiremote_led(t_akawiiremote *x, long enable1, long enable2, long enable3, long enable4)
{
	Boolean	result;
	t_atom	status;

	result = wiiremote_led(x->wiiremote, enable1, enable2, enable3, enable4);
	//SETLONG(&status, result);
	//outlet_anything(x->statusOut, gensym("led"), 1, &status);
}

//--------------------------------------------------------------------------------------------

void akawiiremote_getbattery(t_akawiiremote *x)
{
	if (x->wiiremote->device == nil)
	{
		outlet_anything(x->statusOut, gensym("battery"), 0, nil);
	}
	else
	{
		t_atom	status;

		SETFLOAT(&status, x->wiiremote->batteryLevel);
		outlet_anything(x->statusOut, gensym("battery"), 1, &status);
	}
}

void akawiiremote_getexpansion(t_akawiiremote *x)
{
	if (x->wiiremote->device == nil)
	{
		outlet_anything(x->statusOut, gensym("expansion"), 0, nil);
	}
	else
	{
		t_atom	status;
		if (x->wiiremote->isExpansionPortAttached)
			SETLONG(&status, x->wiiremote->expType);
		else
			SETLONG(&status, 0);
	outlet_anything(x->statusOut, gensym("expansion"), 1, &status);
	}
}

void akawiiremote_getled(t_akawiiremote *x)
{
	if (x->wiiremote->device == nil)
	{
		outlet_anything(x->statusOut, gensym("led"), 0, nil);
	}
	else
	{
		t_atom list[4];

		SETLONG(list,     x->wiiremote->isLED1Illuminated);
		SETLONG(list + 1, x->wiiremote->isLED2Illuminated);
		SETLONG(list + 2, x->wiiremote->isLED3Illuminated);
		SETLONG(list + 3, x->wiiremote->isLED4Illuminated);
		outlet_anything(x->statusOut, gensym("led"), 4, list);
	}
}

void akawiiremote_getcalibration(t_akawiiremote *x)
{
	if (x->wiiremote->device == nil)
	{
		outlet_anything(x->statusOut, gensym("calibration"), 0, nil);
	}
	else
	{
		t_atom list[8];

		if (x->wiiremote->isExpansionPortAttached)
		{
			SETSYM(list,      gensym(nunchukStr));
			SETSYM(list +  1, gensym("stick"));
			SETLONG(list + 2, x->wiiremote->nunchukJoyStickCalibData.x_max);
			SETLONG(list + 3, x->wiiremote->nunchukJoyStickCalibData.x_min);
			SETLONG(list + 4, x->wiiremote->nunchukJoyStickCalibData.x_center);
			SETLONG(list + 5, x->wiiremote->nunchukJoyStickCalibData.y_max);
			SETLONG(list + 6, x->wiiremote->nunchukJoyStickCalibData.y_min);
			SETLONG(list + 7, x->wiiremote->nunchukJoyStickCalibData.y_center);
			outlet_anything(x->statusOut, gensym("calibration"), 8, list);

			SETSYM(list +  1, gensym("motion"));
			SETLONG(list + 2, x->wiiremote->nunchukCalibData.accX_zero);
			SETLONG(list + 3, x->wiiremote->nunchukCalibData.accY_zero);
			SETLONG(list + 4, x->wiiremote->nunchukCalibData.accZ_zero);
			SETLONG(list + 5, x->wiiremote->nunchukCalibData.accX_1g);
			SETLONG(list + 6, x->wiiremote->nunchukCalibData.accY_1g);
			SETLONG(list + 7, x->wiiremote->nunchukCalibData.accZ_1g);
			outlet_anything(x->statusOut, gensym("calibration"), 8, list);
		}

		SETSYM(list,      gensym(remoteStr));
		SETSYM(list +  1, gensym("motion"));
		SETLONG(list + 2, x->wiiremote->wiiCalibData.accX_zero);
		SETLONG(list + 3, x->wiiremote->wiiCalibData.accY_zero);
		SETLONG(list + 4, x->wiiremote->wiiCalibData.accZ_zero);
		SETLONG(list + 5, x->wiiremote->wiiCalibData.accX_1g);
		SETLONG(list + 6, x->wiiremote->wiiCalibData.accY_1g);
		SETLONG(list + 7, x->wiiremote->wiiCalibData.accZ_1g);
		outlet_anything(x->statusOut, gensym("calibration"), 8, list);
	}
}

//--------------------------------------------------------------------------------------------

void akawiiremote_getaddress(t_akawiiremote *x)
{
	if (x->wiiremote->device == nil)
	{
		outlet_anything(x->statusOut, gensym("address"), 0, nil);
	}
	else
	{
		char	str[32];
		t_atom	address;

		wiiremote_getaddress(x->wiiremote, str);
		SETSYM(&address, gensym(str));
		outlet_anything(x->statusOut, gensym("address"), 1, &address);
	}
}

//--------------------------------------------------------------------------------------------

void akawiiremote_clock(t_akawiiremote *x)
{
	Boolean	connection;
	t_atom	status;

	connection = wiiremote_isconnected(x->wiiremote);

	if (x->connected == false && connection == true)	// if the device is connected...
	{
		wiiremote_getstatus(x->wiiremote);
		x->connected = true;
		SETLONG(&status, 1);
		outlet_anything(x->statusOut, gensym("connect"), 1, &status);
	}

	if (x->connected == true && connection == false)
	{
		x->connected = false;
		SETLONG(&status, 0);
		outlet_anything(x->statusOut, gensym("connect"), 1, &status);
	}

	clock_delay(x->clock, kInterval);	// restart clock
}

//--------------------------------------------------------------------------------------------

void akawiiremote_assist(t_akawiiremote *x, void *b, long m, long a, char *s)
{
#ifndef PD /* Max */
	if (m==ASSIST_INLET)
	{
		sprintf(s,"connect, bang, disconnect....");
	}
	else
#endif /* NOT PD */
	{
		switch(a)
		{
			case 0: sprintf(s,"data messages"); break;
			case 1: sprintf(s,"status messages"); break;
		}
	}
}

//--------------------------------------------------------------------------------------------

void *akawiiremote_new(t_symbol *s, short ac, t_atom *av)
{
#ifdef PD
	t_akawiiremote *x = (t_akawiiremote *)pd_new(wiiremote_class);

	x->wiiremote = (WiiRemoteRef)getbytes(sizeof(WiiRemoteRec));
	if (x->wiiremote != nil)
		wiiremote_init(x->wiiremote);

	x->clock = clock_new(x, (t_method)akawiiremote_clock);

	/* create anything outlet used for HID data */
	x->statusOut = outlet_new(&x->x_obj, 0);
	x->buttonsOut = outlet_new(&x->x_obj, &s_float);
	x->irOut = outlet_new(&x->x_obj, &s_list);
	x->accOut = outlet_new(&x->x_obj, &s_list);
#else /* Max */
	t_akawiiremote *x;

	x = (t_akawiiremote *)newobject(akawiiremote_class);

	x->wiiremote = (WiiRemoteRef)getbytes(sizeof(WiiRemoteRec));
	if (x->wiiremote != nil)
	{
		wiiremote_init(x->wiiremote);
		x->wiiremote->isMotionSensorEnabled = true;
		x->wiiremote->isIRSensorEnabled = false;
		x->wiiremote->isVibrationEnabled = false;
		x->wiiremote->isExpansionPortEnabled = false;
		x->wiiremote->isLED1Illuminated = false;
		x->wiiremote->isLED2Illuminated = false;
		x->wiiremote->isLED3Illuminated = false;
		x->wiiremote->isLED4Illuminated = false;
		x->wiiremote->isExtraOutputEnabled = false;
	}

	if (ac>0 && av[0].a_type == A_SYM)
		strcpy(x->address, av[0].a_w.w_sym->s_name);

	x->clock = clock_new(x, (method)akawiiremote_clock);

	x->statusOut = outlet_new(x, 0);
	x->dataOut = outlet_new(x, 0);
#endif /* PD */
	x->connected = false;

	return x;
}

void akawiiremote_free(t_akawiiremote *x)
{
	if (x->wiiremote != nil)
	{
		if (wiiremote_isconnected(x->wiiremote))
			wiiremote_disconnect(x->wiiremote);
		freebytes(x->wiiremote, sizeof(WiiRemoteRec));
		x->wiiremote = nil;
	}

	clock_unset(x->clock);
#ifdef PD
	clock_free(x->clock);
#else /* Max */
	freeobject((t_object *)x->clock);
#endif /* PD */
}

