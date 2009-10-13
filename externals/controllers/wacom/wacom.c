/*
 Copyright (c) 1999, 2000, 01, 08.  The Regents of the University of California (Regents).
 All Rights Reserved.
 
 Permission to use, copy, modify, and distribute this software and its
 documentation for educational, research, and not-for-profit purposes, without
 fee and without a signed licensing agreement, is hereby granted, provided that
 the above copyright notice, this paragraph and the following two paragraphs
 appear in all copies, modifications, and distributions.  Contact The Office of
 Technology Licensing, UC Berkeley, 2150 Shattuck Avenue, Suite 510, Berkeley,
 CA 94720-1620, (510) 643-7201, for commercial licensing opportunities.
 
 Written by Richard Dudas and Jean-Michel Coutier.
 
 IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
 SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS,
 ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF
 REGENTS HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 
 REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT
 LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 FOR A PARTICULAR PURPOSE. THE SOFTWARE AND ACCOMPANYING
 DOCUMENTATION, IF ANY, PROVIDED HEREUNDER IS PROVIDED "AS IS".
 REGENTS HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
 ENHANCEMENTS, OR MODIFICATIONS.
 
 
 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 NAME: wacom
 DESCRIPTION: Report the location, pressure and other parameters of transducers on a Wacom™ Graphics Tablet. You can use several tablets and several tools with different wacom objects.
 AUTHORS: Jean-Michel Courutier, Richard Dudas, and Michael Zbyszynski
 COPYRIGHT_YEARS: 1996, 1999, 2001,02,03,04,05,06,08
 DRUPAL_NODE: /patch/666
 SVN_REVISION: $LastChangedRevision: 1634 $
 VERSION 1.0: based on simple example in wacom SDK; created for Atau Tanaka at Bionic Media in Paris -Rd
 VERSION 2.0: major update finally includes internal clock, wacom tablet driver access instead of adb, different outlets than previous version = no retro-compatibility  -Rd
 VERSION 3.0b1: Ported to OS X -JMC
 VERSION 3.0b2: bug finder->max->finder, output only if the data changes, pressure -> 0 if tool far from tablet, prox record integrated in polling
 VERSION 3.0b3: bug pressure -> 0 if tool far from tablet
 VERSION 3.0b4: wacom mouse: output the mouse coordinates
 VERSION 3.1b1: better performance, 3 handlersw by object, movecurs tablet specific
 VERSION 3.1b3: bug when no dirvers; bug with prox info in poll bang mode
 VERSION 3.1b5: universal binaries; new message xyrange for position : raw, 0-1, 0-1 proportional
 VERSION 4.0: First CNMAT release, assist strings, bug thinking new driver is old -mz
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 */

/* 
Wacom Tablet Max Object 2003, Jean-Michel Couturier  
	
	From the Wacom Tablet Max Object 1996, Richard Dudas  
     
History:

	 2006 3.1ß5: universal binaries; new message xyrange for position : raw, 0-1, 0-1 proportionnal

	 2006 3.1ß3: bug when no drivers; bug with prox info in poll bang mode

	 2005 3.1ß: better performance; 3 handlers by object, movecurs tablet specific.

	 2004 ß4: wacom mouse: output the mouse coordinates

	 2003 ß3: bug pressure -> 0 if tool far from tablet
	 
	 2003 ß2: bug finder->max->finder, output only if the data changes, pressure -> 0 if tool far from tablet,
	 			 prox record integrated in polling
	 			 
	 2003 Ported to OS X !

     2001 July cleaned up code and removed old 68k-specific code -Rd
        
     1999 major update finally includes internal clock -Rd
        wacom tablet driver access instead of adb
  		different outlets than previous version = no retro-compatibility
  		needed to hack together a hybrid wacom.h file to get it to work properly (genuine kluge)
  		the 1999 update was sponsored in part by CNMAT
  		
     1996 version based on simple example in wacom SDK -Rd
        created for Atau Tanaka at Bionic Media in Paris
*/

//#include <Carbon.h>
//#include <CarbonEvents.h>
#include <math.h>
#include <assert.h> 
#include "ext.h"
#include "ext_common.h"
#include "ext_obex.h"
#include "ext_obex_util.h"


#include "Wacom.h"
#include "WacomHelperFuncs.h"
#include "TAEHelpers.h"
#include "ext_strings.h"

#define VERSION "wacom object v4.1ß for os X"
#define RES_ID 23000
#define PI 3.1415
#define kIntuos2TabletID 0x0c
#define NB_TABLETS 16
#define NB_TOOLS 8
#define NB_DEVICE_ID 16
#define NB_WACOM_OBJ 16
//--- Secret Wacom event name. Do not share this code :
#define pSpecialCtxHndlg	'SCtx'
//--------

typedef struct _transducer
{
	Atom 	t_name;
//	long	t_UniqueID;
	long	t_DeviceID;
} t_transducer;


typedef struct _tablet
{
	Atom 	t_name;
	UInt32	t_contextID; // Wacom context for the application for all tablets
	long	t_XDimension;
	long	t_YDimension;
	long	t_resolution;
	long    t_orientation;
	short	t_tiltLevels;
	UInt32 	t_nbTools;
	t_transducer t_tools[NB_TOOLS];
} t_tablet;
	
typedef struct _devID
{
	UInt16	d_ID;
	UInt16	d_systemTabletID;
	Atom 	d_name;
	TabletProximityRec	d_proxRecord;
} t_devID;

typedef struct _wacom
{
	t_object w_ob;
	int w_poll;
	void *w_clock;
	double w_timegrain; // timegrain no longer used - qelem is reset
	short w_tickProxChange;
	int w_wacObjID;
	long w_deviceID;
	long w_systemTabletID;
	short w_mouse; // output tablet or mouse data
	short w_flushQueue;
	short w_scaleMode;
	
	TabletPointRec		w_theTabletRecord;	// The Tablet Pointer Record
	TabletPointRec		w_theTabletOldRecord;	// The Tablet Pointer Old Record
	TabletProximityRec	w_theTabletProxRecord;	// The Tablet Proximity Record
		
	// outlets
	void *w_out_xyposition;		// x, y position list
	void *w_out_pressure;		// pressure
	void *w_out_xytilt;			// x, y tilt angle
	void *w_out_buttons;		// button flags
	
	void *w_out_rotation;		// rotation  (puck)
	void *w_out_zposition;		// "z direction" wheel on puck
	void *w_out_tanpressure;	// side pressure on airbrush

	void *w_out_deviceID;		//  device info
	void *w_out_proxEvents;		//  device info

	void *w_out_menu;		// output menu items

	Boolean w_contextEn;
	Boolean w_specialContextValue;
	Boolean w_appActive;
	UInt32 	w_nbTablets;
	t_tablet w_tablet[NB_TABLETS];
	int 	w_deviceIDNb;
	t_devID w_devID[NB_DEVICE_ID];

	EventHandlerUPP w_appUPP;
	EventHandlerUPP w_mouseUPP;
	EventHandlerUPP w_tabletPointerUPP;
	EventHandlerUPP w_tabletProxUPP;
	EventHandlerRef w_outRefApp;
	EventHandlerRef w_outRefM; 
	EventHandlerRef w_outRefP; 
	EventHandlerRef w_outRefProx; 
} t_wacom;


void *wacom_class;

void wacom_bang(t_wacom *x); 
void wacom_int(t_wacom *x, long n);
void wacom_getinfo(t_wacom *x);
void wacom_poll(t_wacom *x, Symbol *s, short ac, Atom *av); 
void wacom_nopoll(t_wacom *x);
void wacom_positionScale(t_wacom *x, Symbol *s, short ac, Atom *av);
void wacom_movemousePointer(t_wacom *x, Symbol *s, short ac, Atom *av); // to remove
void wacom_nomousePointer(t_wacom *x, Symbol *s, short ac, Atom *av);
void wacom_bgrd(t_wacom *x, Symbol *s, short ac, Atom *av);
void wacom_flushQueue(t_wacom *x, Symbol *s, short ac, Atom *av);
void wacom_menu(t_wacom *x);
void wacom_tick(t_wacom *x);
void wacom_assist (t_wacom *x, void *b, long m, long a, char *s);
void wacom_free (t_wacom *x);
void *wacom_new(Symbol *s, short ac, Atom *av); 
void wacom_mousePointer(t_wacom *x, Boolean contextEn);
void wacom_background(t_wacom *x, Boolean specialContextValue);
void wacom_outputPoint(t_wacom *x, TabletPointRec theTabletRecord);
void wacom_outputProx(t_wacom *x,TabletProximityRec theTabletProxRecord);

TabletPointRec wacom_copyTabletRec(TabletPointRec Record);
TabletProximityRec wacom_copyTabletProxRec(TabletProximityRec Record);
void InstallHandlers(t_wacom *x);
void RemoveHandlers(t_wacom *x);
pascal OSStatus HandleAppEvents(EventHandlerCallRef inCallRef, EventRef inEvent, void *userData );
pascal OSStatus HandleTabletPointer(EventHandlerCallRef inCallRef, EventRef inEvent, void *userData );
pascal OSStatus HandleMouseEvents(EventHandlerCallRef inCallRef, EventRef inEvent, void *userData );
pascal OSStatus HandleTabletProximity(EventHandlerCallRef inCallRef, EventRef inEvent, void *userData );
void CheckDriver(t_wacom *x);
Atom UpdateSpecificPointerType(UInt16 vendorPointerType, UInt16 tabletType);

int main(void)
{
	setup((t_messlist **)&wacom_class, (method)wacom_new, (method)wacom_free, (short)sizeof(t_wacom),  0L, A_GIMME, 0);
	addbang((method)wacom_bang);
	addint((method)wacom_int);
	addmess((method)wacom_poll,"poll",A_GIMME,0);
	addmess((method)wacom_nopoll,"nopoll",0L);
	addmess((method)wacom_positionScale,"xyrange",A_GIMME,0);
	addmess((method)wacom_movemousePointer,"movecurs",A_GIMME,0);
	addmess((method)wacom_nomousePointer,"nopointer",A_GIMME,0);
	addmess((method)wacom_bgrd,"bgrd",A_GIMME,0);
	addmess((method)wacom_flushQueue,"flusheventqueue",A_GIMME,0);
	addmess((method)wacom_menu,"menu",0L);
	addmess((method)wacom_getinfo,"getinfo",0L);
	addmess((method)wacom_assist, "assist", A_CANT, 0);
	
	rescopy('STR#',RES_ID);
	
	post(VERSION,0);
	post("Developed by Jean-Michel Couturier, 2007");
//	post(" parts of the wacom object are",0);
//	post(" Copyright (c) 1999.  The Regents of the University of California (Regents).",0);
//    post(" All Rights Reserved.",0);

	return 0;
}


void wacom_bang(t_wacom *x) // output wacom data when bang
{
	if (x->w_poll) 
		if(x->w_tickProxChange)
			wacom_outputProx(x,x->w_theTabletProxRecord);
		x->w_tickProxChange = 0;
		wacom_outputPoint(x,x->w_theTabletRecord);
}

void wacom_int(t_wacom *x, long n) // read menu output to select a tablet or a tool
{
	int id, i;

	if(n == 0) {
		x->w_deviceID = -1; // all tools
		x->w_systemTabletID = 0; // all tablets
	}
	else if( n > 0 && n <= x->w_nbTablets) {
		x->w_deviceID = -1; // all tools
		x->w_systemTabletID = n; // one tablet
		for(i=1;i<=x->w_nbTablets;i++) {
			if (x->w_tablet[i-1].t_contextID && i != x->w_systemTabletID) {
				DestoryWacomContext(x->w_tablet[i-1].t_contextID);
				x->w_tablet[i-1].t_contextID = 0;
			}
		}
	}
	else if( n > x->w_nbTablets && n <= x->w_nbTablets+x->w_deviceIDNb) {
		id = n-x->w_nbTablets-1;
		x->w_deviceID = x->w_devID[id].d_ID; // one tools (one device ID)
		x->w_systemTabletID = x->w_devID[id].d_systemTabletID;
		for(i=1;i<=x->w_nbTablets;i++) {
			if (x->w_tablet[i-1].t_contextID && i != x->w_systemTabletID) {
				DestoryWacomContext(x->w_tablet[i-1].t_contextID);
				x->w_tablet[i-1].t_contextID = 0;
			}
		}

		x->w_theTabletProxRecord = wacom_copyTabletProxRec(x->w_devID[id].d_proxRecord);
//		post("devID: %i",x->w_deviceID);
	}
	else
		error("bad index");
	wacom_mousePointer(x, x->w_contextEn);
	wacom_background(x, x->w_specialContextValue);
	
}

void wacom_getinfo(t_wacom *x)
{
	int i;
	if (x->w_systemTabletID <= 0)
		for(i=0;i<x->w_nbTablets;i++) {

			object_post((t_object*)x,"Tablet %i name: %s",i+1,x->w_tablet[i].t_name.a_w.w_sym->s_name);
			post("X dimension: %i",x->w_tablet[i].t_XDimension);
			post("Y dimension: %i",x->w_tablet[i].t_YDimension);
			post("Resolution: %i",x->w_tablet[i].t_resolution);
//			post("Orientation: %i",x->w_tablet[i].t_orientation);
			post("Tilt levels: %i",x->w_tablet[i].t_tiltLevels);
	}
	else {
			i = x->w_systemTabletID - 1;
			post("Tablet %i name: %s",i+1,x->w_tablet[i].t_name.a_w.w_sym->s_name);
			post("X dimension: %i",x->w_tablet[i].t_XDimension);
			post("Y dimension: %i",x->w_tablet[i].t_YDimension);
			post("Resolution: %i",x->w_tablet[i].t_resolution);
//			post("Orientation: %i",x->w_tablet[i].t_orientation);
			post("Tilt levels: %i",x->w_tablet[i].t_tiltLevels);
	}
}

void wacom_nopoll(t_wacom *x)
{
	x->w_poll = 0;
	clock_unset(x->w_clock);
	RemoveHandlers(x);
}

void wacom_poll(t_wacom *x, Symbol *s, short ac, Atom *av)
{
	if (!ac) {
		x->w_poll = 1;
		clock_unset(x->w_clock);
		InstallHandlers(x);

	}
	else {
		
		if (av->a_type==A_SYM && !strcmp("bang",av->a_w.w_sym->s_name)) { // message poll bang: no internal clock
			x->w_poll = 2;
			clock_unset(x->w_clock);
			InstallHandlers(x);
		}
		else if (av->a_type==A_LONG && av->a_w.w_long != 0) {
			x->w_timegrain = (double)av->a_w.w_long;
			x->w_poll = 2;
			clock_fdelay(x->w_clock,0L);
			InstallHandlers(x);
		}
		else if (av->a_type==A_FLOAT && av->a_w.w_float != 0) {
			x->w_timegrain = fabs(av->a_w.w_float);
			x->w_poll = 2;
			clock_fdelay(x->w_clock,0L);
			InstallHandlers(x);		
		}
	}
}

void wacom_positionScale(t_wacom *x, Symbol *s, short ac, Atom *av)
{
	if (ac) {
		if (av->a_type==A_LONG && av->a_w.w_long == 0) {
			x->w_scaleMode = 0;
		}
		else if (av->a_type==A_LONG && av->a_w.w_long == 1) {
			x->w_scaleMode = 1;
		}
		else if (av->a_type==A_LONG && av->a_w.w_long == 2) {
			x->w_scaleMode = 2;
		}
	}

}


void wacom_movemousePointer(t_wacom *x, Symbol *s, short ac, Atom *av) // depreciated, but still working for compatibility reasons
{
	if (av->a_type==A_LONG) {
		if (av->a_w.w_long == 0)
			wacom_mousePointer(x, true);
		else
			wacom_mousePointer(x, false);
	}
}

void wacom_nomousePointer(t_wacom *x, Symbol *s, short ac, Atom *av)
{	
	if (av->a_type==A_LONG) {
		if (av->a_w.w_long == 0)
			wacom_mousePointer(x, false);
		else
			wacom_mousePointer(x, true);
	}
}

void wacom_bgrd(t_wacom *x, Symbol *s, short ac, Atom *av)
{	
	if (av->a_type==A_LONG) {
		if (av->a_w.w_long == 0) {
			if(x->w_specialContextValue != false)
				wacom_background(x, false);
		}
		else if(x->w_specialContextValue != true)
			wacom_background(x, true);
	}
}
// Secret function that enable to output data even if max/MSP is in background. Do not share this code !
// ---------------------------------------------------------------
void wacom_background(t_wacom *x, Boolean specialContextValue)
{
	EventTargetRef target;
	EventTypeSpec		applicationEvents[2] = {{ kEventClassApplication, kEventAppActivated	},
													{ kEventClassApplication, kEventAppDeactivated }  };
	UInt32 i;
	TAEObject	theTabletDriverObject;
    theTabletDriverObject.objectType = cWTDTablet;
	if (specialContextValue == false) {
		RemoveEventHandler (x->w_outRefApp); 
		DisposeEventHandlerUPP (x->w_appUPP);
		if(x->w_appActive == false) {
			x->w_appActive = true;
			RemoveHandlers(x);
			InstallHandlers(x);
		}
		if (!x->w_mouse) {	// if tablet data
			for(i=1;i<=x->w_nbTablets;i++) {
//				if (x->w_systemTabletID == 0 || i == x->w_systemTabletID) {
		// Turn off special context Handling
					theTabletDriverObject.tabletIndex = i;
					SetData_ofSize_ofType_ofTabletObject_ForAttribute(&specialContextValue,
							sizeof(specialContextValue),
							typeBoolean,
							&theTabletDriverObject,
							pSpecialCtxHndlg);
//	       		}
	        }
		}
	}
	else if (specialContextValue == true) {

		target = GetApplicationEventTarget();
		x->w_appUPP = NewEventHandlerUPP( HandleAppEvents);
		InstallEventHandler(target, x->w_appUPP,
									sizeof(applicationEvents)/sizeof(EventTypeSpec),
									applicationEvents, x, &x->w_outRefApp );
		if(x->w_appActive == false) {
			RemoveHandlers(x);
			InstallHandlers(x);
		}
		if (!x->w_mouse) {			// if tablet data
			for(i=1;i<=x->w_nbTablets;i++) {
//				if (x->w_systemTabletID == 0 || i == x->w_systemTabletID) {
		// Turn on special context Handling
						theTabletDriverObject.tabletIndex = i;
						SetData_ofSize_ofType_ofTabletObject_ForAttribute(&specialContextValue,
								sizeof(specialContextValue),
								typeBoolean,
								&theTabletDriverObject,
								pSpecialCtxHndlg);
						
//	        	}
			}
		}
	}
	x->w_specialContextValue = specialContextValue;
	
}
// ---------------------------------------------------------------



void wacom_mousePointer(t_wacom *x, Boolean ctxtEn)
{
	UInt32 i;
	Boolean contextEn = ctxtEn;
	Boolean mCurs = false;

//		post("systtablet ID: %i",x->w_systemTabletID);

	if (!x->w_mouse) { // if tablet data
		if (contextEn == false) {
			for(i=1;i<=x->w_nbTablets;i++) {
				if ((x->w_systemTabletID == 0 || i == x->w_systemTabletID) && x->w_tablet[i-1].t_contextID) {
					SetData_ofSize_ofType_ofContext_ForAttribute(&contextEn, sizeof(contextEn), typeBoolean,
	                                              x->w_tablet[i-1].t_contextID, pContextEnabled);
//	  				post("context1ID: %i",x->w_tablet[i-1].t_contextID);       	
	       		}
	        }
		x->w_contextEn = contextEn;
		}
		else if (contextEn == true) {
			for(i=1;i<=x->w_nbTablets;i++) {
				if (x->w_systemTabletID == 0 || i == x->w_systemTabletID) {
					if (x->w_tablet[i-1].t_contextID == 0) {
//			 		post("context2ID: %i",x->w_tablet[i-1].t_contextID);       	
				 		CreateWacomContextForTablet(i, &x->w_tablet[i-1].t_contextID);
						SetData_ofSize_ofType_ofContext_ForAttribute(&mCurs, sizeof(mCurs), typeBoolean,
	                                                   x->w_tablet[i-1].t_contextID, pContextMovesSystemCursor);
						
					}
					else				
						SetData_ofSize_ofType_ofContext_ForAttribute(&contextEn, sizeof(contextEn), typeBoolean,
	                                               x->w_tablet[i-1].t_contextID, pContextEnabled);
	            
	            }
	        }
		x->w_contextEn = contextEn;
		}
	}
	
}

void wacom_flushQueue(t_wacom *x, Symbol *s, short ac, Atom *av) // get only last events in queue
{
	if (av->a_type==A_LONG) {
		if (av->a_w.w_long == 0)
			x->w_flushQueue = 0;
		else
			x->w_flushQueue = 1;
	}

}

void wacom_menu(t_wacom *x)
{
	int i;
	Atom at[32];
	char str[256];

	SETSYM(at, gensym("clear"));
		outlet_anything(x->w_out_menu,at->a_w.w_sym,0,0L);
	SETSYM(at, gensym("append"));
	SETSYM(at+1, gensym("All tablets and tools"));
	for(i=0;i<x->w_nbTablets;i++) {
		SETSYM(at+2*i+2, gensym(","));
		sprintf(str,"%s / all tools",x->w_tablet[i].t_name.a_w.w_sym->s_name);
		SETSYM(at+2*i+3,gensym(str));
	}
	for(i=0;i<x->w_deviceIDNb;i++) {
		SETSYM(at+2*i+2+2*x->w_nbTablets, gensym(","));
		SETSYM(at+2*i+3+2*x->w_nbTablets,x->w_devID[i].d_name.a_w.w_sym);
	}
	outlet_anything(x->w_out_menu,at->a_w.w_sym,2*x->w_nbTablets+2*x->w_deviceIDNb+1,at+1);
}

void wacom_tick(t_wacom *x)
{
	if(x->w_tickProxChange)
		wacom_outputProx(x,x->w_theTabletProxRecord);
	x->w_tickProxChange = 0;
	wacom_outputPoint(x,x->w_theTabletRecord);
	if(x->w_mouse) {	
		x->w_theTabletRecord.tiltX = 0;
		x->w_theTabletRecord.tiltY = 0;
		x->w_theTabletRecord.absZ = 0;
	}
	clock_delay(x->w_clock, x->w_timegrain);
}

//------------------------------------------------
//	Output fonctions
//------------------------------------------------

void wacom_outputPoint(t_wacom *x, TabletPointRec theTabletRecord) 
{
	Atom at[3];
	long xposition;
	long yposition;
	long pressure;
	long xtilt;
	long ytilt;
	long buttons;
	long rotation;
	long zposition;
	long tanpressure;
	long deviceID;
	
	 	
	xposition = (long)theTabletRecord.absX;
	yposition = (long)theTabletRecord.absY;
	pressure = (long)theTabletRecord.pressure;
	xtilt = (long)theTabletRecord.tiltX;
	ytilt = (long)theTabletRecord.tiltY;
	buttons = (long)theTabletRecord.buttons;
	rotation = (long)theTabletRecord.rotation;
	zposition = (long)theTabletRecord.absZ;
	tanpressure = (long)theTabletRecord.tangentialPressure;
	deviceID = (long)theTabletRecord.deviceID;
			
	if(deviceID != x->w_theTabletOldRecord.deviceID)
		outlet_int(x->w_out_deviceID,deviceID);
//	if (theTabletProxRecord.capabilityMask & kTransducerTangentialPressureBitMask)
	if(tanpressure != x->w_theTabletOldRecord.tangentialPressure)
			outlet_int(x->w_out_tanpressure, tanpressure);
//	if (theTabletProxRecord.capabilityMask & kTransducerAbsZBitMask)
	if(zposition != x->w_theTabletOldRecord.absZ)
		outlet_int(x->w_out_zposition, zposition);
//	if (theTabletProxRecord.capabilityMask & kTransducerRotationBitMask)
	if(rotation != x->w_theTabletOldRecord.rotation)
			outlet_int(x->w_out_rotation, rotation);

	if(buttons != x->w_theTabletOldRecord.buttons)
		outlet_int(x->w_out_buttons, buttons);
		
//	if (theTabletProxRecord.capabilityMask & 
//					(kTransducerTiltXBitMask | kTransducerTiltYBitMask)) {
	if(xtilt != x->w_theTabletOldRecord.tiltX || ytilt != x->w_theTabletOldRecord.tiltY) {
		SETLONG(at, xtilt);
		SETLONG(at+1, ytilt);
		outlet_list(x->w_out_xytilt, NIL, 2, at);
	}
	if(pressure != x->w_theTabletOldRecord.pressure)
		outlet_int(x->w_out_pressure, pressure);		

	if(xposition != x->w_theTabletOldRecord.absX || yposition != x->w_theTabletOldRecord.absY) {
		switch(x->w_scaleMode) {
			case 0: // normal mode
				SETLONG(at, xposition);
				SETLONG(at+1, yposition);
				break;
			case 1: // between 0 and 1
				SETFLOAT(at, (float)xposition/x->w_tablet[x->w_theTabletProxRecord.systemTabletID-1].t_XDimension);
				SETFLOAT(at+1, (float)yposition/x->w_tablet[x->w_theTabletProxRecord.systemTabletID-1].t_YDimension);
				break;
			case 2: // between 0 and 1 but keep proportionnality
				SETFLOAT(at, (float)xposition/x->w_tablet[x->w_theTabletProxRecord.systemTabletID-1].t_XDimension);
				SETFLOAT(at+1, (float)yposition/x->w_tablet[x->w_theTabletProxRecord.systemTabletID-1].t_XDimension);
				break;
		}
		outlet_list(x->w_out_xyposition, NIL, 2, at);
	}
	
	x->w_theTabletOldRecord = wacom_copyTabletRec(theTabletRecord);
}

void wacom_outputProx(t_wacom *x,TabletProximityRec theTabletProxRecord)
{
	Atom at[11];
	long pressure = 0;

	// Display the Vendor ID of the tablet associated with this event
	SETLONG(at, theTabletProxRecord.vendorID);
	// Pointer events that have this same Device ID come from the transducer
	SETLONG(at+1, theTabletProxRecord.tabletID);
	// The index of the transducer on the tablet.
	// The user can be using more than one transducer on the tablet
	// at the same time! However, only one transducer can move
	// the cursor. The other transducer will generate pure tablet
	// events.
	SETLONG(at+2, theTabletProxRecord.pointerID);
	// Display the Device ID of this use of this transducer.
	SETLONG(at+3, theTabletProxRecord.deviceID);
	// Display the index of the tablet the events are coming from.
	// The user may have more than one tablet attached!
	SETLONG(at+4, theTabletProxRecord.systemTabletID);
	// Vendor supplied id of the type of transducer
	SETLONG(at+5, theTabletProxRecord.vendorPointerType);
	// The Serial Number of this transducer. Transducer's of different
	// classes can have the same SN! That's why there is a unique ID! :)
	SETLONG(at+6, theTabletProxRecord.pointerSerialNumber);
	// Display the unique ID of this transducer. Remember, use this
	// to distinquish between different transducers. The deviceID is
	// only used to match up pointer events with proximity events.
	SETLONG(at+7, theTabletProxRecord.uniqueID);
	// Display the RAW capabilites field.
	SETLONG(at+8, theTabletProxRecord.capabilityMask);
	// Display what type of pointer is being used.
	// This is how you determine if the user is using the pen tip or eraser.
	SETLONG(at+9, theTabletProxRecord.pointerType);
	// Enter or Exit Proximity?
	SETLONG(at+10, theTabletProxRecord.enterProximity);

	outlet_list(x->w_out_proxEvents, NIL, 11, at);
// put pressure to 0 if the  tool is far from the tablet
	if(!theTabletProxRecord.enterProximity && x->w_theTabletOldRecord.pressure != 0) {
		outlet_int(x->w_out_pressure, pressure);		
		x->w_theTabletRecord.pressure = x->w_theTabletOldRecord.pressure = 0;
	}
	
}

TabletPointRec wacom_copyTabletRec(TabletPointRec theTabletRecord)
{
	TabletPointRec res;	

	res.absX = theTabletRecord.absX;
	res.absY = theTabletRecord.absY;
	res.pressure = theTabletRecord.pressure;
	res.tiltX = theTabletRecord.tiltX;
	res.tiltY = theTabletRecord.tiltY;
	res.buttons = theTabletRecord.buttons;
	res.rotation = theTabletRecord.rotation;
	res.absZ = theTabletRecord.absZ;
	res.tangentialPressure = theTabletRecord.tangentialPressure;
	res.deviceID = theTabletRecord.deviceID;

	return res;
}

TabletProximityRec wacom_copyTabletProxRec(TabletProximityRec theTabletProxRecord)
{
TabletProximityRec res;

		res.vendorID = theTabletProxRecord.vendorID;
		res.tabletID = theTabletProxRecord.tabletID;
		res.pointerID = theTabletProxRecord.pointerID;
		res.deviceID = theTabletProxRecord.deviceID;
		res.systemTabletID = theTabletProxRecord.systemTabletID;
		res.vendorPointerType = theTabletProxRecord.vendorPointerType;
		res.pointerSerialNumber = theTabletProxRecord.pointerSerialNumber;
		res.uniqueID = theTabletProxRecord.uniqueID;
		res.capabilityMask = theTabletProxRecord.capabilityMask;
		res.pointerType = theTabletProxRecord.pointerType;
		res.enterProximity = theTabletProxRecord.enterProximity;

	return res;
}

//------------------------------------------------
//	Assist
//------------------------------------------------

/* old style
 void wacom_assist(t_wacom *x, void *b, long m, long a, char *s)
{
	assist_string(RES_ID,m,a,1,2,s);
}
*/
void wacom_assist(t_wacom *x, void *b, long m, long a, char *s) 
{
	if (m == ASSIST_INLET)
		sprintf(s,"messages to wacom object");
	else {
		switch(a) {	
			case 0:
				sprintf(s,"(list) x, y position");
				break;
			case 1:
				sprintf(s,"(int) pressure");
				break;
			case 2:
				sprintf(s,"(list) x, y tilt");
				break;
			case 3:
				sprintf(s,"(int) button flags");
				break;
			case 4:
				sprintf(s,"(int) rotation");
				break;
			case 5:
				sprintf(s,"(int) z axis");
				break;
			case 6:
				sprintf(s,"(int) tangental pressure");
				break;
			case 7:
				sprintf(s,"(int) device ID");
				break;
			case 8:
				sprintf(s,"(list) tablet proximity record");
				break;
			case 9:
				sprintf(s,"(list) menu");
				break;
		}
	}
}
//------------------------------------------------
//	Free
//------------------------------------------------

void wacom_free (t_wacom *x)
{
	int i;
	wacom_mousePointer(x, false);
	wacom_background(x, false);
	for(i=1;i<=x->w_nbTablets;i++) {
		if (x->w_tablet[i-1].t_contextID)
			DestoryWacomContext(x->w_tablet[i-1].t_contextID);
		}
	if(x->w_poll)
		RemoveHandlers(x);

	freeobject(x->w_clock);
}

//------------------------------------------------
//	New
//------------------------------------------------

void *wacom_new(Symbol *s, short ac, Atom *av)
{
//	short taberr;
//	Atom at[3];
	t_wacom *x;
	int i;

	x = (t_wacom *)newobject(wacom_class);
	x->w_clock = clock_new(x, (method)wacom_tick);
	x->w_timegrain = 20; // 20 millisecond polling for the moment
	x->w_tickProxChange = 0;
	x->w_poll = 0;
	x->w_mouse = 0; // mouse pointer instead of tilt
	x->w_flushQueue = 0;
	x->w_deviceIDNb = 0;
	x->w_nbTablets = 0;
	x->w_contextEn = true;
	x->w_specialContextValue = false;
	x->w_appActive = true;
	x->w_scaleMode = 0;
	
	for(i=1;i<=NB_TABLETS;i++)
 		x->w_tablet[i-1].t_contextID = 0;

	if (ac > 0 && av->a_type==A_LONG) {
		x->w_deviceID = av->a_w.w_long; //  one tool
		x->w_systemTabletID = -1; // no tablet until deviceID Prox event appears

	}
	else {
		x->w_deviceID = -1; //  all tools
		x->w_systemTabletID = 0; // all tablets

	}
		
	if (ac != 0 && av->a_type==A_SYM && av->a_w.w_sym->s_name[0]) { // if name after wacom
		if(!strcmp(av->a_w.w_sym->s_name,"mouse")) { // compare with mouse
			x->w_mouse = 1; // mouse instead of tablet
			x->w_deviceID = -2; // 0: mouse only
		}
	}
	else
		x->w_mouse = 0;
		
	x->w_out_menu = listout(x);
	x->w_out_proxEvents = listout(x);
	x->w_out_deviceID = intout(x);
	x->w_out_tanpressure = intout(x);
	x->w_out_zposition = intout(x);
	x->w_out_rotation = intout(x);
	x->w_out_buttons = intout(x);
	x->w_out_xytilt = listout(x); // list
	x->w_out_pressure = intout(x);
	x->w_out_xyposition = listout(x); // list
	
	x->w_theTabletRecord.absX = 0;
	x->w_theTabletRecord.absY = 0;
	x->w_theTabletRecord.pressure = 0;
	x->w_theTabletRecord.tiltX = 0;
	x->w_theTabletRecord.tiltY = 0;
	x->w_theTabletRecord.buttons = 0;
	x->w_theTabletRecord.rotation = 0;
	x->w_theTabletRecord.absZ = 0;
	x->w_theTabletRecord.tangentialPressure = 0;
	x->w_theTabletRecord.deviceID = 0;

	x->w_theTabletOldRecord.absX = -1;
	x->w_theTabletOldRecord.absY = -1;
	x->w_theTabletOldRecord.pressure = -1;
	x->w_theTabletOldRecord.tiltX = -1;
	x->w_theTabletOldRecord.tiltY = -1;
	x->w_theTabletOldRecord.buttons = -1;
	x->w_theTabletOldRecord.rotation = -1;
	x->w_theTabletOldRecord.absZ = -1;
	x->w_theTabletOldRecord.tangentialPressure = -1;
	x->w_theTabletOldRecord.deviceID = -1;

	CheckDriver(x); // Check the version of the driver and names of the tablets.
	InstallHandlers(x);

	return (x);
}

//////////////////////////////////////////////////////////////////////////////
// InstallHandlers - from wacom SDK
//
//  This function installs the callback routines to be used when mouse or
//  tablet events occur.
//
//////////////////////////////////////////////////////////////////////////////
void InstallHandlers(t_wacom *x)
{
	OSStatus				status;
	// These are the mouse events we want to listen to because they may contain
	// embedded tablet events. If you do not care about tablet data unless a
	// button (or tip) is pressed, and you are using Wacom's Tablet Driver
	// version 4.7.5 or higher, then you can remove the kEventMouseMoved entry.
	// If you are using an older tablet driver then you must listen to all mouse
	// events to get proximity events. In this case, I want to demonstrate getting
	// tablet data from all mouse events, so we am going to listen for
	// kEventMouseMoved events as well.

	EventTargetRef target;
	
	EventTypeSpec		mouseEvents[5] = {{ kEventClassMouse, kEventMouseDown		},
													{ kEventClassMouse, kEventMouseUp      },
													{ kEventClassMouse, kEventMouseDragged },
													{ kEventClassMouse, kEventMouseMoved   },
													{ kEventClassMouse, kEventMouseWheelMoved }  };


	// These are pure tablet events. That is, they are not embedded in a mouse event.
	EventTypeSpec	tabletPointerEvent = { kEventClassTablet, kEventTabletPointer };
	EventTypeSpec	tabletProximityEvent = { kEventClassTablet, kEventTabletProximity };

	// Install the mouse event handlers
	if(x->w_appActive == true)
		target = GetApplicationEventTarget();
	else
//		target = GetApplicationEventTarget();
		target = GetEventMonitorTarget(); // à compiler avec CW 9

	x->w_mouseUPP = NewEventHandlerUPP( HandleMouseEvents);
	status = InstallEventHandler(target, x->w_mouseUPP,
													sizeof(mouseEvents)/sizeof(EventTypeSpec),
													mouseEvents, x, &x->w_outRefM );
//	status = InstallApplicationEventHandler(	NewEventHandlerUPP( HandleMouseEvents ),
//													sizeof(mouseEvents)/sizeof(EventTypeSpec),
//													mouseEvents, x, &x->w_outRefM );
//	assert(status == noErr);
	
	// Install the pure Tablet Pointer Event Handler. Generally, you do not need
	// to worry about pure tablet pointer events unless you are doing dual handed
	// input. That is, two transducers on the tablet at the same time.
	x->w_tabletPointerUPP = NewEventHandlerUPP(HandleTabletPointer);
	status = InstallEventHandler(target, x->w_tabletPointerUPP,
													1, &tabletPointerEvent, x,  &x->w_outRefP );
//	status = InstallApplicationEventHandler( NewEventHandlerUPP(HandleTabletPointer),
//													1, &tabletPointerEvent, x,  &x->w_outRefP );
//	assert(status == noErr);

	// Install the pure Proximity Event Handler. If you are running 4.7.5 or higher
	// tablet driver, then all proximity events will be generated as pure tablet
	// proximity events caught with this handler. Otherwise, proximity events are
	// embedded in the mouse mouse events.
	x->w_tabletProxUPP = NewEventHandlerUPP( HandleTabletProximity );
	status = InstallEventHandler(target, x->w_tabletProxUPP,
													1, &tabletProximityEvent, x,  &x->w_outRefProx );
//	status = InstallApplicationEventHandler( NewEventHandlerUPP( HandleTabletProximity ),
//													1, &tabletProximityEvent, x,  &x->w_outRefProx );
//	assert(status == noErr);

}

void RemoveHandlers(t_wacom *x)
{
	RemoveEventHandler (x->w_outRefM); 
	RemoveEventHandler (x->w_outRefP); 
	RemoveEventHandler (x->w_outRefProx); 
	DisposeEventHandlerUPP (x->w_mouseUPP);
	DisposeEventHandlerUPP (x->w_tabletPointerUPP);
	DisposeEventHandlerUPP (x->w_tabletProxUPP);
}


//////////////////////////////////////////////////////////////////////////////
// HandleAppEvents - from wacom SDK
//
//  This is a callback function that the Carbon Event Loop calls whenever the
//  window receives an application Event. Look at the function
//  InstallHandlers() below to see how this call back was installed.
//
//
//////////////////////////////////////////////////////////////////////////////
pascal OSStatus HandleAppEvents(EventHandlerCallRef inCallRef,
											 EventRef inEvent, void *userData )
{
	t_wacom *x = userData;
	UInt32 eventKind;

	eventKind =  GetEventKind (inEvent);

	if(eventKind == kEventAppActivated)
	{
//		post("active");
		x->w_appActive = true;
		RemoveHandlers(x);
		InstallHandlers(x);
	}
	else if(eventKind == kEventAppDeactivated)
	{
//		post("inactive");
		x->w_appActive = false;
		RemoveHandlers(x);
		InstallHandlers(x);
	}
	
	CallNextEventHandler (inCallRef, inEvent);
	
	return eventNotHandledErr;
}

//////////////////////////////////////////////////////////////////////////////
// HandleMouseEvents - from wacom SDK
//
//  This is a callback function that the Carbon Event Loop calls whenever the
//  window receives a Mouse Event. Look at the function
//  InstallHandlers() below to see how this call back was installed.
//
//  This function extracts the mouse data from the event.
//  If this mouse event does contain an embedded mouse event, this function
//  will call the tablet pointer or proximity event handler.
//
//////////////////////////////////////////////////////////////////////////////
pascal OSStatus HandleMouseEvents(EventHandlerCallRef inCallRef,
											 EventRef inEvent, void *userData )
{

	// When I installed this callback fuction, I set the userData to point to
	// the only window created by this application. That way I do not have
	// have a global pointer to the window to access it's controls here.
	// Other, perhaps better, uses of userData would be a pointer to brush
	// data or perhaps the brush object itself.
//	int i;
	t_wacom *x = userData;
	HIPoint				aPoint;
	UInt32				anInt32;
//	EventMouseButton	aButton;
	SInt32 MouseWheelDelta;
	EventMouseWheelAxis aMouseWheelAxis;


	// Determine if there is an embedded tablet event in this mouse event.
	if(noErr == GetEventParameter(inEvent, kEventParamTabletEventType,
											typeUInt32, NULL, sizeof(UInt32),
											NULL, (void *)&anInt32))
	{
		// Embedded tablet events can either be a proximity or pointer event.
		if(anInt32 == kEventTabletPoint)
		{
			// Yes there is one!
			// Send the event off to the tablet pointer event handler.
			HandleTabletPointer(inCallRef, inEvent, userData);
		}
		else
		{
			// You can ignore embedded proximity events if running tablet driver version
			// 4.7.5 or higher, because the proximity event will also be sent as a pure
			// tablet proximity event, and we hanve a Handler installed for pure
			// proximity events.
		}
	}
	else
	{
			if(x->w_mouse) {
			
	// Get the mouse location. Notice that we are using a HIPoint here.
	// HiPoints are cool because they are floating points. That means, that you
	// will get the exact location of the cursor down to about 4 decimal places.
	// So if you want sub pixel information, you can use this instead of looking
	// at the absX and absY tablet data and converting it to screen cooridinates.
	if(noErr == GetEventParameter(inEvent, kEventParamMouseLocation,
											typeHIPoint, NULL, sizeof(HIPoint),
											NULL, (void *)&aPoint))
	{
//	x->w_theTabletOldRecord.absX = -1;
//	x->w_theTabletOldRecord.absY = -1;
	x->w_theTabletRecord.absX = aPoint.x;
	x->w_theTabletRecord.absY = aPoint.y;
	}
	else 
	{
		// All mouse events should have a mouse loc. But if something goes wrong,
		// make sure the output is pretty.
	}

	// Get the delta between the last mouse loc and this one.
	// Delta locations aren't all that useful for tablets. But here it is if want it.
	if(noErr == GetEventParameter(inEvent, kEventParamMouseDelta, typeHIPoint,
								  NULL, sizeof(HIPoint), NULL,
								  (void *)&aPoint))
	{
		x->w_theTabletOldRecord.tiltX = 0;
		x->w_theTabletOldRecord.tiltY = 0;
		x->w_theTabletRecord.tiltX += aPoint.x;
		x->w_theTabletRecord.tiltY += aPoint.y;
	}
	else
	{
		// All mouse events should have a mouse delta. But if something goes wrong,
		// make sure the output is pretty.
		
	}

	// Get the state of the keyboard modifiers for this mouse event.
	if(noErr == GetEventParameter(inEvent, kEventParamKeyModifiers,
											typeUInt32, NULL, sizeof(UInt32),
											NULL, (void *)&anInt32))
	{
//		x->w_theTabletOldRecord.pressure =-1;
		x->w_theTabletRecord.pressure =anInt32;
	}
	else
	{
		// All mouse events should have modifiers. But if something goes wrong,
		// make sure the output is pretty.
	}
	// Get the chord of the mouse buttons. The mouse chord is a 32 bit field that
	// represents the state of all the buttons. (Up to 32 of them!) This is how you
	// check to see if multiple buttons are being pressed during a drag.
	if(noErr == GetEventParameter(inEvent, kEventParamMouseChord,
											typeUInt32, NULL, sizeof(UInt32),
											NULL, (void *)&anInt32))
	{
//		x->w_theTabletOldRecord.buttons = -1;
		x->w_theTabletRecord.buttons = anInt32;
	}
	else
	{
		// There is no chord data for Mouse Moves because no buttons are down
		// during a move. For mouse moves, the get will fail, so make the output pretty.
	}
	if(noErr == GetEventParameter(inEvent, kEventParamMouseWheelDelta,
											typeSInt32, NULL, sizeof(SInt32),
											NULL, (void *)&MouseWheelDelta))
	{
//		post("wheel: %i",MouseWheelDelta);
		x->w_theTabletOldRecord.absZ = 0;
		x->w_theTabletRecord.absZ += MouseWheelDelta;
	}
	else
	{
	}


	if(noErr == GetEventParameter(inEvent, kEventParamMouseWheelAxis,
											typeMouseWheelAxis, NULL, sizeof(EventMouseWheelAxis),
											NULL, (void *)&aMouseWheelAxis))
	{
//		post("wheel axis: %i",aMouseWheelAxis);
//		x->w_theTabletOldRecord.tangentialPressure = -1;
		x->w_theTabletRecord.tangentialPressure = (UInt16)aMouseWheelAxis;
	}
	else
	{
	}
			
//	x->w_theTabletRecord.deviceID = -2;
			
	if(x->w_poll == 1) {
		wacom_outputPoint(x,x->w_theTabletRecord);
		x->w_theTabletRecord.tiltX = 0;
		x->w_theTabletRecord.tiltY = 0;
		x->w_theTabletRecord.absZ = 0;
	}
			}
		}

	CallNextEventHandler (inCallRef, inEvent);

   return eventNotHandledErr;
}


//////////////////////////////////////////////////////////////////////////////
// HandleTabletPointer - from wacom SDK
//
//  This is a callback function that the Carbon Event Loop calls whenever the
//  window receives a pure Tablet Pointer Event. Look at the function
//  InstallHandlers() below to see how this call back was installed.
//
//  Also this call back is directly called from the MouseEvent callback when
//  the mouse event contains an embedded tablet event.
//
//  This function extracts the tablet pointer record from the event.
//
// NOTE: This function was created from the "Tablet Data Structures .
// 		Pointer Event" section of the "Next Generation Tablet Software
//			Implementer’s Guide, MacOS X" which can be found on	Wacom's dev web
//			site <http://www.wacomeng.com/devsupport/mac.html>.
//////////////////////////////////////////////////////////////////////////////

pascal OSStatus HandleTabletPointer(EventHandlerCallRef inCallRef,
                                    EventRef inEvent, void *userData )
{
	int j;
	t_wacom				*x = userData;
	TabletPointRec		theTabletRecord;	// The Tablet Pointer Record


	const EventTypeSpec	events[ 1 ] = { { kEventClassTablet, kEventTabletPointer } };
	
	
	EventRef latestEvent = inEvent;
	EventRef futureEvent = NULL;


//  flush event queue if the option is checked
	if(x->w_flushQueue)
		while(noErr == ReceiveNextEvent(1, events, kEventDurationNoWait, true, &futureEvent)) {
	    	if(latestEvent != inEvent)
	        	ReleaseEvent(latestEvent);
	    	latestEvent = futureEvent;
		}
	

	// Extract the tablet Pointer Event. If there is no Tablet Pointer data
	// in this event, then this call will return an error. Just ignore the
	// error and go on. This can occur when a proximity event is embedded in
	// a mouse event and you did not check the mouse event to see which type
	// of tablet event was embedded.
	if(noErr == GetEventParameter(latestEvent, kEventParamTabletPointRec,
								  typeTabletPointRec, NULL,
								  sizeof(TabletPointRec),
								  NULL, &theTabletRecord))
	{

			if(x->w_poll == 1) {
				if (x->w_deviceID == -1) {
					if (x->w_systemTabletID == 0)		
						wacom_outputPoint(x,theTabletRecord);
					else {
						for(j=0;j<x->w_deviceIDNb;j++)
							if(x->w_devID[j].d_ID == theTabletRecord.deviceID 
									&& x->w_systemTabletID == x->w_devID[j].d_systemTabletID)
								wacom_outputPoint(x,theTabletRecord);
					}
				}
				else if(x->w_deviceID == theTabletRecord.deviceID)
					wacom_outputPoint(x,theTabletRecord);
			}
			else if(x->w_poll == 2) { // si poll n
				if (x->w_deviceID == -1) {
					if (x->w_systemTabletID == 0)		
						x->w_theTabletRecord = wacom_copyTabletRec(theTabletRecord);
					else {
						for(j=0;j<x->w_deviceIDNb;j++)
							if(x->w_devID[j].d_ID == theTabletRecord.deviceID 
									&& x->w_systemTabletID == x->w_devID[j].d_systemTabletID)
								x->w_theTabletRecord = wacom_copyTabletRec(theTabletRecord);
					}
				}
				else if(x->w_deviceID == theTabletRecord.deviceID)
					x->w_theTabletRecord = wacom_copyTabletRec(theTabletRecord);
			}
	}
	
   if(latestEvent != inEvent)
		ReleaseEvent(futureEvent);

	CallNextEventHandler (inCallRef, inEvent);

	return noErr;
}

//////////////////////////////////////////////////////////////////////////////
// HandleTabletProximity
//
//  This is a callback function that the Carbon Event Loop calls whenever the
//  application receives a pure Tablet Proximity Event. Look at the function
//  InstallHandlers() below to see how this call back was installed.
//
//  This function extracts the tablet proximity record from the event and
//  updates the static text boxes in the Tablet Proximity Event group of the
//  window.
//
// NOTE: This function was created from the "Tablet Data Structures .
// 		Proximity Event" section of the "Next Generation Tablet Software
//			Implementer’s Guide, MacOS X" which can be found on	Wacom's dev web
//			site <http://www.wacomeng.com/devsupport/mac.html>.
//////////////////////////////////////////////////////////////////////////////

pascal OSStatus HandleTabletProximity(EventHandlerCallRef inCallRef,
                                    EventRef inEvent, void *userData )
{
	t_wacom				*x = userData; // ugh Type casting...
	Atom buff;
	int i=0;
	char str[25];
//	Boolean contextEn;
	TabletProximityRec	theTabletProxRecord;	// The Tablet Proximity Record


	// Extract the Tablet Proximity reccord from the event.
	if(noErr == GetEventParameter(inEvent, kEventParamTabletProximityRec,
								  typeTabletProximityRec, NULL,
								  sizeof(TabletProximityRec),
								  NULL, &theTabletProxRecord))
	{

		while (x->w_devID[i].d_ID != theTabletProxRecord.deviceID && i<x->w_deviceIDNb)
			i++;
//		post("i: %i, nbdevID: %i",i,x->w_deviceIDNb);
//		post("ProxsystID: %i",theTabletProxRecord.systemTabletID);			
//		post("1:devID: %i, proxdevID: %i",x->w_deviceID,theTabletProxRecord.deviceID);
//		check if there is a tablet on the computer
		if(i == x->w_deviceIDNb && theTabletProxRecord.systemTabletID) {
			x->w_deviceIDNb++;
//			post("devID: %i, proxdevID: %i",x->w_deviceID,theTabletProxRecord.deviceID);
			x->w_devID[i].d_ID = theTabletProxRecord.deviceID;
			if (theTabletProxRecord.systemTabletID > x->w_nbTablets)
				CheckDriver(x);
			x->w_devID[i].d_systemTabletID = theTabletProxRecord.systemTabletID;
			x->w_devID[i].d_proxRecord = wacom_copyTabletProxRec(theTabletProxRecord);
			if (x->w_deviceID == theTabletProxRecord.deviceID)
				x->w_systemTabletID = theTabletProxRecord.systemTabletID;
//			post("systID: %i",x->w_systemTabletID);
			// Lets pick the vendorPointerType apart and fiqure out exactly
			// which transducer is being used. We should probably check make
			// sure that the vendor ID for this proximity event is Wacom's.
			// But hey, why would you run Wacom's sample code on another
			// vendor's tablet? ;)
// BUG on MacIntel with first versions of wacom drivers  !!! change of vendorpointertype. it's ok now.
			buff = UpdateSpecificPointerType(theTabletProxRecord.vendorPointerType,
												theTabletProxRecord.tabletID);
			sprintf(str,"%s / %s", 
						x->w_tablet[theTabletProxRecord.systemTabletID-1].t_name.a_w.w_sym->s_name,
				 		buff.a_w.w_sym->s_name);
			SETSYM(&x->w_devID[i].d_name,gensym(str));

		}
		
			if(x->w_poll > 0) {
				if (x->w_deviceID == -1) {
					if (x->w_systemTabletID == 0 ||
								x->w_systemTabletID == 
								theTabletProxRecord.systemTabletID) {
						x->w_theTabletProxRecord = wacom_copyTabletProxRec(theTabletProxRecord);
						if(x->w_poll < 2)
							wacom_outputProx(x,theTabletProxRecord);
						else
							x->w_tickProxChange = 1;	
					}
				}
				else if(x->w_deviceID == theTabletProxRecord.deviceID) {	
//					contextEn = x->w_contextEn;
//					SetData_ofSize_ofType_ofContext_ForAttribute(&contextEn, sizeof(contextEn), typeBoolean,
//                                                   x->w_tablet[x->w_systemTabletID-1].t_contextID, pContextEnabled);
					x->w_theTabletProxRecord = wacom_copyTabletProxRec(theTabletProxRecord);
					if(x->w_poll < 2)
						wacom_outputProx(x,theTabletProxRecord);
					else
						x->w_tickProxChange = 1;	
				}
			}			
		}

	CallNextEventHandler (inCallRef, inEvent);

	return noErr;

}


//////////////////////////////////////////////////////////////////////////////
// UpdateSpecificPointerType
//
// This function will display the general class of the current transducer
// and will also try to display the specific transducer.
//
// The current general type of transducers are Stylus, Airbrush, and Puck
//
// vendorPointerType - The vendorPointerType field of a tablet proximity
//							  record.
//
// tabletType - The tabletID field of a tablet proximity record. This param
//				is used to help distinquish between transducers that have the
//				same vendor pointer type id but, are different for different
//				tablets. For example, the intuos tablet's grip stylus' ID was
//				changed for the Intuos 2 line of tablets when it became the
//				standard transducer shipping with the tablet.
//
// 
// NOTE: This function was created from the "Tablet Data Structures .
// 		Proximity Event . vendorPointerType" section of the "Next Generation
// 		Tablet Software Implementer’s Guide, MacOS X" which can be found on
//			Wacom's dev web site <http://www.wacomeng.com/devsupport/mac.html>.
//////////////////////////////////////////////////////////////////////////////

Atom UpdateSpecificPointerType(UInt16 vendorPointerType, UInt16 tabletType)
{
	Atom Res;
	SETSYM(&Res,gensym("Unknown")); // set default to "unknown"

	// Try to determine the general type of transducer To verify (pb between 2 version of the doc)
	if((vendorPointerType & 0x0F06) == 0x0002)
	{
		SETSYM(&Res,gensym("General Stylus"));
	}
	if((vendorPointerType & 0x0F06) == 0x0902)
	{
		SETSYM(&Res,gensym("Airbrush"));
	}
	if((vendorPointerType & 0x0006) == 0x0006)
	{
		SETSYM(&Res,gensym("General Mouse"));
	}
	if((vendorPointerType & 0x0006) == 0x0004)
	{
		SETSYM(&Res,gensym("Pro Mouse"));
	}
	if((vendorPointerType & 0x0F06) == 0x0804)
	{
		SETSYM(&Res,gensym("Rotation Stylus"));
	}
	
	// Try to determine the exact type of transducer	
	if( vendorPointerType == 0x822 )
	{
		if( tabletType == kIntuos2TabletID ) // Is this an Intuos 2 Tablet?
		{
			SETSYM(&Res,gensym("Classic Stylus"));
		}
		else
		{
			SETSYM(&Res,gensym("Standard Stylus"));
		}
	}
	if( vendorPointerType == 0x852 )
	{
		if( tabletType == kIntuos2TabletID ) // Is this an Intuos 2 Tablet?
		{
			SETSYM(&Res,gensym("Standard (Grip) Stylus"));
		}
		else
		{
			SETSYM(&Res,gensym("Grip Stylus"));
		}
	}
	if( vendorPointerType == 0x842 )
	{
		if( tabletType == kIntuos2TabletID ) // Is this an Intuos 2 Tablet
		{
			SETSYM(&Res,gensym("Designer Stylus"));
		}
		else
		{
			SETSYM(&Res,gensym("Intuos Grip Stylus"));
		}
	}
	if( vendorPointerType == 0x812 )
	{
		SETSYM(&Res,gensym("Inking Stylus"));
	}
	if( vendorPointerType == 0x832 )
	{
		SETSYM(&Res,gensym("Stroke Stylus"));
	}
	if( vendorPointerType == 0x912 )
	{
		SETSYM(&Res,gensym("Airbrush"));
	}
	if( vendorPointerType == 0x007 )
	{
		SETSYM(&Res,gensym("2D Mouse"));
	}
	if( vendorPointerType == 0x094 )
	{
		SETSYM(&Res,gensym("4D Mouse"));
	}
	if( vendorPointerType == 0x096 )
	{
		SETSYM(&Res,gensym("Lens Cursor"));
	}
	if( vendorPointerType == 0x022 )
	{
		SETSYM(&Res,gensym("Graphire Stylus"));
	}
	if( vendorPointerType == 0x296 )
	{
		SETSYM(&Res,gensym("Graphire Mouse"));
	}
// Intuos 3
	if( vendorPointerType == 0x823 )
	{
		SETSYM(&Res,gensym("Standard (Grip) Stylus"));
	}
	if( vendorPointerType == 0x885 )
	{
		SETSYM(&Res,gensym("Rotation Stylus"));
	}
	if( vendorPointerType == 0x801 )
	{
		SETSYM(&Res,gensym("Inking Stylus"));
	}
	if( vendorPointerType == 0x913 )
	{
		SETSYM(&Res,gensym("Airbrush"));
	}
	if( vendorPointerType == 0x017 )
	{
		SETSYM(&Res,gensym("2 D mouse"));
	}
	
	return Res;
}

//////////////////////////////////////////////////////////////////////////////
// CheckDriver
//
//  This function finds out what the version of the tablet driver is,
//  post it, and find the names of the difterents tablets available for the driver. 
//
//////////////////////////////////////////////////////////////////////////////
void CheckDriver(t_wacom *x)
{
	OSErr			err = noErr;
	TAEObject	theTabletDriverObject;
	NumVersion 	theVerData;
	UInt32		numTablets;
//	UInt32		numTransducers;
  	char 		nameStr[256];
//  	char 		nameStr2[256];
	long	XDimension;
	long	YDimension;
	long	resolution;
	long     orientation;
	short	tiltLevels;
	UInt32 i;
	Boolean gTabletDriver475OrHigher;
//	UInt32 j;
//	Boolean mCurs = false;
//	Boolean contextEn = true;
//	Boolean   specialContextValue = true;
	
	// Use the Wacom supplied Apple Event helper function to ask the tablet
	// driver for it's version.
	theTabletDriverObject.objectType = cWTDDriver;
	err = GetData_ofSize_ofType_ofTabletObject_ForAttribute(&theVerData,
																			sizeof(theVerData),
																			typeVersion,
																			&theTabletDriverObject,
																			pVersion);
	if(err == noErr)
	{
		// Post the version number 
//		post("Wacom driver V.%d.%d.%d", theVerData.majorRev, (theVerData.minorAndBugRev/10), theVerData.minorAndBugRev-(((int)(theVerData.minorAndBugRev/10)) * 10));
		gTabletDriver475OrHigher = TRUE;		
/*		if ( ( theVerData.majorRev > 4 ) || ((theVerData.majorRev >= 4) && (theVerData.minorAndBugRev >= 75)) )
		{
			// Set a global flag so that we know we can use 4.7.5 features.
			gTabletDriver475OrHigher = TRUE;
		}
		else
		{
		post("Wacom driver: too old version");			
		} 
 */
	}
	else
	{
		// Dang, this means that you are running a pre 4.7.5 driver, or
		// running on pre 10.2. That's a bummer.
		gTabletDriver475OrHigher = FALSE;
		post("No Wacom driver detected");
	}
 
	
	theTabletDriverObject.objectType = cWTDTablet;
	err = CountTabletObjects(&theTabletDriverObject, &numTablets);
	if(err == noErr)
		x->w_nbTablets = numTablets;
	else
		x->w_nbTablets = 0;
//	if(numTablets < 2)
//		post("%i tablet on the driver.", numTablets);
//	else		
//		post("%i tablets on the driver.", numTablets);		

// check names of tablets in the driver
	for(i=1;i<=x->w_nbTablets;i++) {

		theTabletDriverObject.tabletIndex = i;

		memset(nameStr, 0, 256);
		err = GetData_ofSize_ofType_ofTabletObject_ForAttribute(&nameStr,
																			sizeof(nameStr),
																			typeChar,
																			&theTabletDriverObject,
																			pName);
		
		if(err == noErr)
			SETSYM(&x->w_tablet[i-1].t_name,gensym(nameStr));
		else
			SETSYM(&x->w_tablet[i-1].t_name,gensym("Unknown"));		

//			post("wacom tablet %i: %s",i,nameStr);

		err = GetData_ofSize_ofType_ofTabletObject_ForAttribute(&XDimension,
																			sizeof(XDimension),
																			typeLongInteger,
																			&theTabletDriverObject,
																			pXDimension);
		
		if(err == noErr)
			x->w_tablet[i-1].t_XDimension = XDimension;
		else
			x->w_tablet[i-1].t_XDimension = 0;

		err = GetData_ofSize_ofType_ofTabletObject_ForAttribute(&YDimension,
																			sizeof(YDimension),
																			typeLongInteger,
																			&theTabletDriverObject,
																			pYDimension);
		
		if(err == noErr)
			x->w_tablet[i-1].t_YDimension = YDimension;
		else
			x->w_tablet[i-1].t_YDimension = 0;

		err = GetData_ofSize_ofType_ofTabletObject_ForAttribute(&resolution,
																			sizeof(resolution),
																			typeLongInteger,
																			&theTabletDriverObject,
																			pResolution);
		if(err == noErr)
			x->w_tablet[i-1].t_resolution = resolution;
		else
			x->w_tablet[i-1].t_resolution = 0;


		err = GetData_ofSize_ofType_ofTabletObject_ForAttribute(&orientation,
																			sizeof(orientation),
																			typeEnumerated,
																			&theTabletDriverObject,
																			pOrientation);
		if(err == noErr)
			x->w_tablet[i-1].t_orientation = orientation;
		else
			x->w_tablet[i-1].t_orientation = 0;

		err = GetData_ofSize_ofType_ofTabletObject_ForAttribute(&tiltLevels,
																			sizeof(tiltLevels),
																			typeShortInteger,
																			&theTabletDriverObject,
																			pTiltLevels);
		if(err == noErr)
			x->w_tablet[i-1].t_tiltLevels = tiltLevels;
		else
			x->w_tablet[i-1].t_tiltLevels = 0;

/*
// this code doesn't give correct results because of a bug into the wacom driver

		theTabletDriverObject.tabletIndex = i;
		theTabletDriverObject.objectType = cWTDCustomizedApp;
 		err = CountTabletObjects(&theTabletDriverObject, &numTransducers);
		post("numApp %i",numTransducers);
		for(j=1;j<=numTransducers;j++) {
			theTabletDriverObject.applicationIndex = j;
			err = GetData_ofSize_ofType_ofTabletObject_ForAttribute(&nameStr2,
																				sizeof(nameStr2),
																				typeChar,
																				&theTabletDriverObject,
																				pAppID);

//		if(err == noErr)
//			post("appID %i: %s",j,nameStr2);
//		else
//			post("appID %i: error",j);
			x->w_nbTablets = numTablets;
		}
		
		theTabletDriverObject.objectType = cWTDTransducer;
 		err = CountTabletObjects(&theTabletDriverObject, &numTransducers);
		x->w_tablet[i-1].t_nbTools = numTransducers;
//		post("numTransducers %i",numTransducers);
		for(j=1;j<=numTransducers;j++) {
			theTabletDriverObject.transducerIndex = j;
//			post("TransducersID %i",theTabletDriverObject.transducerIndex);

		err = GetData_ofSize_ofType_ofTabletObject_ForAttribute(&tiltLevels,
																			sizeof(tiltLevels),
																			typeShortInteger,
																			&theTabletDriverObject,
																			DeviceID);
//		post("wacom tool %i: deviceID %i",j,tiltLevels);

			memset(nameStr2, 0, 256);
			err = GetData_ofSize_ofType_ofTabletObject_ForAttribute(&nameStr2,
																				sizeof(nameStr2),
																				typeChar,
																				&theTabletDriverObject,
																				pName);

			if(err == noErr) {
				if(!nameStr2[0]||(nameStr2[0] == '\\') && (nameStr2[1] == '~')) {
					sprintf(nameStr2,"tool %d",j);
					SETSYM(&x->w_tablet[i-1].t_tools[j-1].t_name,gensym(nameStr2));
//					post("wacom tool %i: tool %i",j,j);
				}
				else {
					SETSYM(&x->w_tablet[i-1].t_tools[j-1].t_name,gensym(nameStr2));
//					post("wacom tool %i: %s",j,nameStr2);
				}
			}
			else
				post("wacom tool %i: error",j);
		}
		theTabletDriverObject.objectType = cWTDTablet;
*/
	}
	

}
