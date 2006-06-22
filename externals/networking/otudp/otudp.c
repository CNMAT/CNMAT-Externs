/* 
Copyright (c) 1997,1998,1999,2000,2001.  The Regents of the University of California (Regents).
All Rights Reserved.

Permission to use, copy, modify, and distribute this software and its
documentation for educational, research, and not-for-profit purposes, without
fee and without a signed licensing agreement, is hereby granted, provided that
the above copyright notice, this paragraph and the following two paragraphs
appear in all copies, modifications, and distributions.  Contact The Office of
Technology Licensing, UC Berkeley, 2150 Shattuck Avenue, Suite 510, Berkeley,
CA 94720-1620, (510) 643-7201, for commercial licensing opportunities.

This code may not be used to make another Max UDP object, but only as an example
to add Open Transport UDP support to other Mac applications.

Written by Matt Wright, The Center for New Music and Audio Technologies,
University of California, Berkeley.

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
*/




/*
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
NAME: otudp
DESCRIPTION: Open Transport UDP object (now superseded by udp objects from Cycling 74 that cooperate much better with Max's threading system)
AUTHORS: Matt Wright
COPYRIGHT_YEARS: 1997,98,99,2000,01,02,03,04,05,06
VERSION 0.0: Initial version
VERSION 0.1:  Made error reporting slightly more verbose 10/16/97
VERSION 0.2:  Fixed bugs, made more stable, in 11/97 for The Hub's "Points of Presence"
VERSION 0.3:  Converted to asynchronous mode, 2/18/99
VERSION 0.4:  Used OTEnterNotifier() to solve synchronization problems, 3/9/99
VERSION 0.5: More syncoronization debugging, 4/27/99
VERSION 0.6:  Allowed changing the host on the fly, 5/18/99
VERSION 1.7:  Made pendingBuffers a FIFO 10/18/99
VERSION 1.8: Allowed changing the receive port on the fly
VERSION 1.9: Allowed #1 through #9 as host and port arguments
VERSION 2.0: MacOS 9.1 compatibility: Only calls OTSndUData at Deferred Task (not interrupt) time
VERSION 2.1: Fixed bizarre packet-dropping bugs
VERSION 2.2: Changed error reporting to post() instead of ouchstring()
VERSION 2.3: PPC only, allows "host" and "receiveport" even in overdrive.  No dialog boxes.
VERSION 3.0: OSX version: InContext, nothing deferred...
VERSION 3.0.1:  Used error() instead of post() for error messages
VERSION 3.0.1b: Complains less often about semaphoreTest Failed
VERSION 3.0.2:  Moved the semaphoreTest to the right place
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  


Now that this thing uses asynchronous mode, it's nearly impossible to tell how
the flow of control works, so here are some hints:

- Everything's pretty normal through main() and the new() procedure, until the
  call to OTAsyncOpenEndpoint().  This returns immediately, but when OT really 
  opens my endpoint it invokes my "notifier function", OTUDPNotifier().  Almost
  every call to OT results in OT calling my notifier when it's done.
  
- Next we have to bind the endpoint to the desired address.  Again this is
  asynchronous, and our notifier is called when it completes.  Each object's
  o_ready variable is false until its endpoint is bound.  (When we change the
  host, we reset this to false until the endpoint is rebound.)

- Writes are also asynchronous, but we don't get called back when they complete.
  However, if there's an error, our notifier will get a T_UDERR event.
  
- Reads are asynchronous.  When new data comes in our notifier gets called.
  We read the data from inside the notifier using OTRcvUData(), which returns
  immediately.  But since the notifier runs at interrupt level, we stash the
  incoming data into a PacketBuffer and put it on the object's pendingBuffers
  FIFO, then set a Max "clock" to invoke otudp_output() at non-interrupt time.

- We have two linked list of buffers to hold PacketBuffers - the free list of
  currently unused buffers and the pending list of buffers we've filled but haven't
  outputted yet.
  
 */


#define OTUDP_VERSION "3.0.1b"
#define MAX_PACKET_SIZE 65536   // This is the limit for a UDP packet
#define DEFAULT_BUFFER_SIZE  1024
#define DEFAULT_NUM_BUFFERS 20 


#include "ext.h"

/* We need to access the field "error" in the Open Transport TUDErr structure,
   so we have to undo the Max function table macro for error() */
#undef error

#include <OpenTransport.h>
#include <OpenTptInternet.h>

/* #include <LowMem.h>				// For A5 stuff for 68K notifier
#include <OSUtils.h>			// Same reason
*/

#include "packetbuffer-pool.h"  // Must be after OpenTransport.h because of return address.


/* structure definition */
typedef struct otudp {
	// Max stuff
	Object o_ob;
	long o_a5;			    /* Holds a5 so notifier callback can work on 68k */
	void *o_outlet;
	void *o_clock;

	int o_writer;		    /* 1 if this is a write object; 0 if read  */
	int o_ready;			/* Will be 0 until endpoint is created and bound */
	int o_errorreporting;	/* 1 for error reporting; 0 for silent */
	int o_ever_dropped_a_packet; /* 1 if we ever dropped a packet */
	int o_num_dropped_packets;  /* Number of packets dropped since last complaint */
	int o_complained_about_old_msgs;	/* So we can complain about old-style write/gimme once */


	// Internet addresses and ports associated with this object
	char *o_inetHostName;   /* write objects: String the user gave for desired host to send to */
	InetHost o_inetHost;	/* write objects: OT's representation of the host to send to */
	InetPort o_inetPort;	/* write objects: OT's representation of the port to send to */

	InetPort o_receiveInetPort;	/* All objects: port user asked for */
	TBind o_bindAddrWeActuallyGot;  /* So OT can tell us what port number we really bound to */
	struct InetAddress o_addrWeActuallyGot;  /* Same deal */
	char *o_desiredNewHostName; /* arg to "host" message, saved until we can call DNS @ deferred task time */
	InetPort o_desiredNewPort;  /* ditto */

	// Open Transport stuff	
	EndpointRef o_udp_ep;
	TEndpointInfo epinfo;
	OTLock o_readlock;
	int o_semaphoreTest;		// To see if mutual exclusion on linked lists is working
	int o_semaphoreTestFailureCount;
	
	// Buffer management
	long nbufs;
	long bufsize;
	PacketBuffer freeBuffers;		  /* Pool of available PacketBuffer objects */
	PacketBufferFIFO pendingBuffers;  /* FIFO of received but unoutputted buffers */
	PacketBuffer allBuffers;		  /* So we can free them */
		
	// Qelem fun (using DNR can't even happen at deferred task time)
	void *UnbindQelem ;    /* Qelem to change the host not at interrupt or deferred task level */
	
	int foo;
} OTUDP;

/* globals */
fptr *FNS;
void *otudp_class;
Symbol *ps_read, *ps_write, *ps_PartialPacket, *ps_FullPacket, *ps_nbufs, *ps_bufsize;
OTClientContextPtr OTContext;



/* prototypes */
void *otudp_new(Symbol *s, short argc, Atom *argv);
void otudp_free(OTUDP *x);
int ParseArgs(short argc, Atom *argv, int *writerp, char **inetHostNamep, InetPort *portp, 
			  InetPort *rportp, long *nbufsp, long *bufsizep);
static int LookUpInetHost(char *name, InetHost *result);
void BindTheEndpoint(OTUDP *x);
void PostUDERR(char *source, EndpointRef ep);
pascal void OTUDPNotifier(void* vobj, OTEventCode code, OTResult result, void* cookie);
void otudp_output(OTUDP *x);
static short AcquireLock(OTUDP *x);
static void ReleaseLock(OTUDP *x, short oldLockout);
static Boolean BufferSanityCheck(OTUDP *x, int *freep, int *pendingp, int *outgoingp);
static int CountPacketList(PacketBuffer l);

void otudp_assist (OTUDP *x, void *box, long msg, long arg, char *dstString);
void otudp_version(OTUDP *x);
void otudp_debugstats(OTUDP *x);
void otudp_resetdebugstats(OTUDP *x);
void otudp_toggleErrorReporting(OTUDP *x);
void otudp_read(OTUDP *x);

void try_write(OTUDP *x, long length, void *bytes);
void do_write(OTUDP *x, long length, void *bytes);
void otudp_write(OTUDP *x, long size, long bufferPointer);
void otudp_old_write(OTUDP *x, Symbol *s, short argc, Atom *argv);
void otudp_tellmeeverything(OTUDP *x);
void otudp_changeHost(OTUDP *x, Symbol *hostName, long port);
void unbind_from_qelem (void* arg);
void otudp_changeReceivePort(OTUDP *x, long port);

#define YEAR 1999
#define MONTH 8
#define MONTH1 -1
#define DAY 1
#define EXPIRATION_NOTICE "otudp object version " OTUDP_VERSION " has expired!"




/* initialization routine */

void main(fptr *f) {
	OSStatus err;
	DateTimeRec date;
	
#ifdef MULTI_SEGMENT
	/* Multi-segment code resource stuff */
	{
		Str255 buf;
		short i, n, theID;
		char **ch;
		OSType theType;
		n = Count1Resources('Cccc');
	    post("This external has %ld resources", (long) n);
		for (i = 1; i <= n; i++) {
			ch = Get1IndResource('Cccc', i);
			GetResInfo(ch, &theID, &theType, buf);
			rescopy('Cccc', theID);
		}
	}
#endif

#ifdef EXTENDED_CODE_RESOURCE
	// Cause it's an extended code resource...
	// rescopy('Cccc',22222);
#endif

	setup((t_messlist **)&otudp_class, (method)otudp_new, (method) otudp_free, (short)sizeof(OTUDP), 0L, A_GIMME, 0);
	
	post("otudp object version " OTUDP_VERSION " by Matt Wright. ");
#ifdef DAVID_LIKES_EXPIRING_MAX_OBJECTS
	post("Expires %d/%d/%d", MONTH, DAY, YEAR);
#endif
	post("Copyright © 1997,98,99,2000,2001 Regents of the University of California. All Rights Reserved.");

#ifdef DAVID_LIKES_EXPIRING_MAX_OBJECTS
	GetTime(&date);
	if((date.year > YEAR)  || 
       (date.year == YEAR && ((date.month > MONTH) ||
							  (date.month == MONTH && date.day > DAY)))) {
            ouchstring(EXPIRATION_NOTICE);
            return;
    }
#endif
        
    // post ("*** before  InitOpenTransport();");
	err = InitOpenTransportInContext(kInitOTForExtensionMask, &OTContext);
	if (err != kOTNoError) {
		error("¥ OTUDP: Couldn't InitOpenTransport (err %d).  Perhaps Open Transport is not installed.",
					err);
		return;
	}
	
	/* bind methods */
	addmess((method) otudp_write, "FullPacket", A_LONG, A_LONG, 0);
	addmess((method) otudp_version, "version", 0);
	addmess((method) otudp_toggleErrorReporting, "errorreporting", 0);
	addmess((method) otudp_tellmeeverything, "tellmeeverything", 0);
	addmess((method) otudp_old_write, "write", A_GIMME, 0);
	addmess((method) otudp_old_write, "gimme", A_GIMME, 0);
	addmess((method) otudp_assist, "assist", A_CANT, 0);
	addmess((method) otudp_changeHost, "host", A_SYM, A_LONG, 0);
	addmess((method) otudp_changeReceivePort, "receiveport", A_LONG, 0);

	/* These methods are just for debugging. */
	addmess((method) otudp_debugstats, "debugstats", 0);
	addmess((method) otudp_resetdebugstats, "resetdebugstats", 0);
	addmess((method) otudp_read, "readitagainsam", 0);

	finder_addclass("System","otudp");

	ps_read = gensym("read");
	ps_write = gensym("write");
	ps_FullPacket = gensym("FullPacket");
	ps_PartialPacket = gensym("PartialPacket");
	ps_nbufs = gensym("nbufs");
	ps_bufsize = gensym("bufsize");
}


void *otudp_new(Symbol *s, short argc, Atom *argv) {
	OTUDP *x;
	OSStatus err;
	InetHost host;
	
	// These variables will be filled with the args we parse
	int writer;
	char *inetHostName = 0;
	InetPort port = 0;
	InetPort receivePort = 0;
	long nbufs = DEFAULT_NUM_BUFFERS;
	long bufsize = DEFAULT_BUFFER_SIZE;

		
	if (ParseArgs(argc, argv, &writer, &inetHostName, &port, &receivePort, &nbufs, &bufsize) == 0) {
		error("¥ OTUDP usage: \"otudp read <port> [bufsize <nbytes>] [nbufs <n>]\" or "
				   "\"otudp write <hostname> <port> [bufsize <nbytes>] [nbufs <n>]\"");
		return 0;
	}
	
	if (writer) {
		if (LookUpInetHost(inetHostName, &host) == 0) {
			error("otudp: Couldn't make sense of Internet host \"%s\"", inetHostName);
			return 0;
		}
	}
	
	x = newobject(otudp_class);
/*	x->o_a5 = (long) LMGetCurrentA5(); */
	x->o_writer = writer;
	x->o_inetHostName = inetHostName;
	x->o_inetPort = port;
	x->o_inetHost = host;
	x->o_receiveInetPort = receivePort;
	x->o_desiredNewHostName = 0;
	x->o_desiredNewPort = 0;

	x->o_ready = 0;
	x->o_errorreporting = 1;
	x->o_ever_dropped_a_packet = 0;
	x->o_num_dropped_packets = 0;
	x->o_outlet = outlet_new(x,0L);
	x->o_clock = clock_new(x, (method) otudp_output);
	InitPBFIFO(&(x->pendingBuffers));
	
	x->o_complained_about_old_msgs = 0;
	
	OTClearLock(&(x->o_readlock));
	x->o_semaphoreTest = x->o_semaphoreTestFailureCount = 0;
	
	/* Allocate packet buffers */
	x->bufsize = bufsize;
	x->nbufs = nbufs;
	if (nbufs == 0) {
		x->allBuffers = x->freeBuffers = 0; 
	} else {
		x->allBuffers = InitPackets(bufsize, nbufs);
		if (x->allBuffers == 0) {
			error("¥ OTUDP: Out of memory (Couldn't allocate buffers to store incoming packets)");
			return 0;
		}

		x->freeBuffers = x->allBuffers;
	}
	
	x->o_udp_ep = 0; 	// Indicates endpoint hasn't been created yet.
	err = OTAsyncOpenEndpointInContext(OTCreateConfiguration(kUDPName), 0, &(x->epinfo), 
									   NewOTNotifyUPP(OTUDPNotifier), x, OTContext);
	if (err != noErr) {
		error("¥ otudp: Error %d from OTAsyncOpenEndpoint. This is bad.", err);
		return 0;
	}
	// This didn't actually make the endpoint; my notifier should get the T_OPENCOMPLETE
	// event after the endpoint is opened.
	
	/* Create a Qelem that will let us change the host at non-interrupt, non-defered-task-time level */
	x->UnbindQelem = qelem_new(x, (method)unbind_from_qelem);

	return (x);
}

#define IS_FORMAL_PARAMETER_SYMBOL(x) \
  ((x).a_type == A_SYM && (x).a_w.w_sym->s_name[0] == '#' && \
   (x).a_w.w_sym->s_name[1] >= '1' && (x).a_w.w_sym->s_name[1] <= '9')

int ParseArgs(short argc, Atom *argv, int *writerp, char **inetHostNamep, InetPort *portp, 
			  InetPort *rportp, long *nbufsp, long *bufsizep) {
	Symbol *selector;
	int rest;

	if (argc == 0 || argv[0].a_type != A_SYM)
		return 0;

	selector = argv->a_w.w_sym;
	if (selector != ps_read && selector != ps_write)
		return 0;

	if (selector == ps_write) {
		// Must be args for host and port
		if (argc < 3 || argv[1].a_type != A_SYM) return 0;
		
		if (IS_FORMAL_PARAMETER_SYMBOL(argv[2])) {
			/* The Max programmer is trying to make a patch that will be
			   a subpatch with arguments.  This particular otudp object can't be
			   used (unless the host is changed dynamically), but we have to
			   create the object correctly with the right arguments. */
			*portp = 1;
		} else if (argv[2].a_type == A_LONG) {
			*portp = (InetPort) argv[2].a_w.w_long;
		} else {
			return 0;
		}
		
		*writerp = 1;
		
		if (IS_FORMAL_PARAMETER_SYMBOL(argv[1])) {
			/* Same issue */
			*inetHostNamep = "1.1.1.1";
		} else {
			*inetHostNamep = argv[1].a_w.w_sym->s_name;
		}
		rest = 3;
	} else if (selector == ps_read) {
		// Must be arg for port
		if (argc < 2) return 0;
		
		if (argv[1].a_type == A_LONG) {
			*rportp = (InetPort) argv[1].a_w.w_long;
		} else if (IS_FORMAL_PARAMETER_SYMBOL(argv[1])) {
			*rportp = 1;
		} else {
			return 0;
		}
		
		*writerp = 0;
		
		rest = 2;
	} else {
		error("¥ OTUDP: first arg must be either \"read\" or \"write\"");
		return 0;
	}
	
	while (rest < argc) {
		if (argv[rest].a_type != A_SYM) return 0;
		
		if (argv[rest].a_w.w_sym == ps_nbufs) {
			if (rest+1 >= argc || argv[rest+1].a_type != A_LONG) return 0;
			*nbufsp = argv[rest+1].a_w.w_long;
			rest += 2;
		} else if (argv[rest].a_w.w_sym == ps_bufsize) {
			if (rest+1 >= argc || argv[rest+1].a_type != A_LONG) return 0;
			*bufsizep = argv[rest+1].a_w.w_long;
			rest += 2;
		} else {
			return 0;
		}
	}
	
	return 1;
}

#define debug_close_post(x) post(x)

void otudp_free(OTUDP *x) {
	debug_close_post("* entered otudp_free");
	
	freeobject(x->o_clock);
    if (x->o_udp_ep != 0) {
    	debug_close_post("* about to close Open Transport endpoint...");
		OTCloseProvider(x->o_udp_ep);
		debug_close_post("* ... closed Open Transport endpoint");
	} else {
		debug_close_post("* This OTUDP object didn't have an Open Transport endpoint");
	}
	
	if (x->allBuffers != 0) {
		DestroyPackets(x->allBuffers, x->nbufs);
	}
	
	qelem_free(x->UnbindQelem);
}

/* I don't know why these aren't defined in some Max #include file. */
#define ASSIST_INLET 1
#define ASSIST_OUTLET 2

void otudp_assist (OTUDP *x, void *box, long msg, long arg, char *dstString) {	
	if (x->o_writer) {
		if (msg==ASSIST_INLET) {
			sprintf(dstString, "buffers to send via UDP");
		} else if (msg==ASSIST_OUTLET) {
			sprintf(dstString, "UDP packets to this sender's return address");
		} else {
			error("¥ otudp_assist: unrecognized message %ld", msg);
		}
	} else {
		if (msg==ASSIST_INLET) {
			sprintf(dstString, "version, errorreporting, etc.");
		} else if (msg==ASSIST_OUTLET) {
			sprintf(dstString, "Incoming UDP packets");
		} else {
			error("¥ otudp_assist: unrecognized message %ld", msg);
		}
	}
}


static int LookUpInetHost(char *name, InetHost *result) {
	OSStatus err;

	// If the hostname is dots and numbers, like "128.32.122.13", we don't neet DNR
	err = OTInetStringToHost(name, result);
	if (err != noErr) {
		// We do need DNR.
		InetSvcRef tcpipProvider;
		InetHostInfo hinfo;
		
		tcpipProvider = OTOpenInternetServicesInContext(kDefaultInternetServicesPath, 0, &err, OTContext);
		
		if (err != noErr) {
			error("¥ otudp: Error %d from OTOpenInternetServices().", err);
			return 0;
		}
		
		err = OTInetStringToAddress(tcpipProvider, name, &hinfo);
		OTCloseProvider(tcpipProvider);
		if (err == kOTBadNameErr) {
			error("¥ otudp: Bad host name \"%s\"; not changing.", name);
		} else if (err != noErr) {
			error("¥ otudp: Error %d from OTInetStringToAddress; not changing host.", err);
			return 0;
		}
		*result = hinfo.addrs[0];
	}
	return 1;
}


long gHandlingSomethingAlready = 0;   // for reentrancy
long numTimesGaveUpForReentrancy = 0; // debugstats
long numTimesNotifierCalled = 0;	  // debugstats


pascal void OTUDPNotifier(void* vobj, OTEventCode code, OTResult result, void* cookie) {
	OTUDP *x = (OTUDP *) vobj;
/*	long oldA5; */
	
	// Maybe want to call OTEnterNotifier here...
	
/*	oldA5 = SetA5(x->o_a5); */

	numTimesNotifierCalled++;
	
	if (gHandlingSomethingAlready) {
		numTimesGaveUpForReentrancy++;
		goto done;
	}
	gHandlingSomethingAlready = 1;
	
#ifdef DEBUG
	post("*** OTUDPNotifier(code %ld, result %ld)", (long) code, (long) result);
#endif
	
	if (code == T_OPENCOMPLETE) {
		if (result != noErr) {
			error("OTUDPNotifier got a T_OPENCOMPLETE code with error %ld", result);
		}
		
		// Even if we got an error, we'll bind the endpoint.  
		x->o_udp_ep = (EndpointRef) cookie;
		BindTheEndpoint(x);
		// This is asynchronous; now we expect the notifier to be called with 
		// a T_BINDCOMPLETE message.

	} else if (code == T_BINDCOMPLETE) {
		// See what address we actually got
		
		if (x->o_writer) {
			// We let OT pick an address for us, so look up the port just in case
			// we ever care to know what our return address port number is
			x->o_receiveInetPort = x->o_addrWeActuallyGot.fPort;
		} else {
			// We asked OT for a particular port, but didn't necessarily get it
			if (x->o_receiveInetPort != x->o_addrWeActuallyGot.fPort) {
				error("¥ otudp: Tried to bind to port %ld, but got %ld instead!!",
						   x->o_receiveInetPort, x->o_addrWeActuallyGot.fPort);
				x->o_receiveInetPort = x->o_addrWeActuallyGot.fPort;
			} else {
#ifdef DEBUG
				post("*** successfully bound to port %ld", (long) x->o_addrWeActuallyGot.fPort);
#endif
			}
		}
		
		// Now the endpoint has been bound, so we're ready to go
		x->o_ready = 1;
	} else if (code == T_UNBINDCOMPLETE) {
		// Only otudp_changeHost and otudp_changeReceivePort unbind the port, so now 
		// it's time to rebind with the new host info
		BindTheEndpoint(x);		
	} else if (code == T_DATA) {
		otudp_read(x);
	} else if (code == T_UDERR) {
		if (x->o_errorreporting) {
			PostUDERR("OTUDPNotifier was called for a T_UDERR code", x->o_udp_ep);
		} else {
			/* We still have to receive the error to clear the condition */
			OTRcvUDErr(x->o_udp_ep, 0);
		}		
	} else {
	 	if (x->o_errorreporting) {
			error("OTUDP: Unrecognized OTEventCode %ld in event handler.  Oh well.", (long) code);
		}
	}

done:
	gHandlingSomethingAlready = 0;
	/* SetA5(oldA5); */
}


void BindTheEndpoint(OTUDP *x) {
	// This will be called from within our notifier function after the endpoint is created,
	// or, when changing hosts, to re-bind the endpoint after unbinding it.
	OSStatus err;

	
	/* Set up the data structure OTBind will use to say what port number we got */
	x->o_bindAddrWeActuallyGot.addr.maxlen = sizeof(struct InetAddress);
	x->o_bindAddrWeActuallyGot.addr.buf = (unsigned char *) &(x->o_addrWeActuallyGot);
	
	if (x->o_writer) {
		/* Let OTBind pick a return address port number for us */
  	    err = OTBind(x->o_udp_ep, nil, &(x->o_bindAddrWeActuallyGot));
		if (err != kOTNoError) {
			error("¥ otudp: Error %d from OTBind.  You may not be able to write.", err);
			return;
		}
	} else {
		/* Tell OTBind what port number we want to use */
		struct InetAddress desiredAddress;
		TBind bindDesiredAddress;

		OTInitInetAddress(&desiredAddress, x->o_receiveInetPort, (InetHost) 0);
		
		bindDesiredAddress.addr.maxlen = sizeof(struct InetAddress);
		bindDesiredAddress.addr.len = sizeof(struct InetAddress);
		bindDesiredAddress.addr.buf = (unsigned char *) &desiredAddress;

		err = OTBind(x->o_udp_ep, &bindDesiredAddress, &(x->o_bindAddrWeActuallyGot));

		if (err != kOTNoError) {
			error("¥ otudp: Error %d from OTBind.  You may not be able to read.", err);
			return;
		}
		
	}
	
	/* Could output something to let the Max programmer know that we're now re-bound... */

}

void otudp_changeHost(OTUDP *x, Symbol *hostName, long port) {	
	if (!x->o_writer) {
		error("¥ OTUDP: You can only change the host on write objects, not readers.");
		return;
	}
	
	if (x->o_desiredNewHostName != 0) {
		post("¥ OTUDP: Warning: we were already in the middle of changing the host");
		post("  to \"%s\".", x->o_desiredNewHostName);
	}
	
	// We're in the un-ready state until we unbind and rebind
	// (This eliminates the need for locks+semaphores)
	x->o_ready = 0;

	// Stash away new host info
	x->o_desiredNewHostName = hostName->s_name;
	x->o_desiredNewPort = port;
	
	/* Can't call OTUnbind @ Interrupt level or access DNR (to translate new symbolic host to an IP address)
	   at deferred task time, so set a Qelem to do it. */
	qelem_set(x->UnbindQelem);
	
	// post("*** Leaving otudp_changeHost()");
}

void unbind_from_qelem (void* arg) {
	int tries;
	OSStatus s;
	OTUDP *x;

	x = (OTUDP *) arg;

	// post("*** Entering unbind_from_qelem()");

	/* First, if we're trying to change the host, make sure the desired new host is OK. */
	if (x->o_desiredNewHostName != 0) {
		InetHost host;
		// post("*** about to call LookUpInetHost");
		if (LookUpInetHost(x->o_desiredNewHostName, &host) == 0) {
			error("¥ Can't change Internet host: \"%s\" makes no sense.", x->o_desiredNewHostName);
			
			/* No harm done; just ignore the host change request. */
			x->o_desiredNewHostName = 0;
			return;
		}
		
		// post("*** looked up new host");
		// Remember new host info so we can rebind when the Unbind completes.
		x->o_inetHostName = x->o_desiredNewHostName;
		x->o_inetHost = host;
		x->o_inetPort = x->o_desiredNewPort;
		
		x->o_desiredNewHostName = 0;
	}



	tries = 0;
	while (1) {
		// post("*** About to unbind");
		s = OTUnbind(x->o_udp_ep);
		if (s == kOTNoError) {
			// Groovy; now we just wait for T_UNBINDCOMPLETE in OTUDPNotifier()
		} else if (s == kOTLookErr) {
			// Probably more data has arrived, so we have to read it all out
			// before this can work.
			if (tries > 3) {
				error("¥ OTUDP: kept getting kOTLookErr when trying to unbind; I give up.");
				break;
			}
			error("OTUDP: Couldn't unbind because kOTLookErr; reading then trying again.");
			otudp_read(x);
			++tries;
			continue;
		} else {
			error("¥ OTUDP: OTUnbind returned %ld; can't change host.", s);
		}
		break;
	}
}	



void otudp_changeReceivePort(OTUDP *x, long port) {
	if (x->o_writer) {
		error("¥ OTUDP: You can only change the receive port on read objects, not writers.");
		return;
	}

	// We're in the un-ready state until we unbind and rebind
	x->o_ready = 0;
	
	// Stash away new port so we can rebind when the Unbind completes.
	x->o_receiveInetPort = port;


	/* Unbinding happens via a Qelem (because for the case of "write" objects, the Unbind could
	   require DNR, which can't happen at interrupt or deferred-task time). */
	qelem_set(x->UnbindQelem);
}


/* Mutual exclusion stuff */

static short AcquireLock(OTUDP *x) {
	// Mutual exclusion:  Make sure that neither Max not Open Transport will interrupt us

	int i;
	short oldLockout;

	
	oldLockout = lockout_set(1);
	OTEnterNotifier(x->o_udp_ep);

#define TOO_MANY_SPINS 1
	i = 0;
	while (OTAcquireLock(&(x->o_readlock)) == false) {
		if ((++i) > TOO_MANY_SPINS) {
			error("¥ otudp_read: OTAcquireLock keeps failing!");
		}
	}

	if (x->o_semaphoreTest != 0) {
		++(x->o_semaphoreTestFailureCount);
		if (x->o_semaphoreTestFailureCount == 1 || (x->o_semaphoreTestFailureCount % 100) == 0) {
			error("¥ otudp_read: new semaphore test failed (%d times)");
		}
	}

	++(x->o_semaphoreTest);
	return oldLockout;
}

static void ReleaseLock(OTUDP *x, short oldLockout) {
	if (x->o_semaphoreTest != 1) {
		error("¥ otudp_read: about to release lock, but semaphoreTest is %ld", x->o_semaphoreTest);
	}
	--(x->o_semaphoreTest);

	OTClearLock(&(x->o_readlock));
	OTLeaveNotifier(x->o_udp_ep);
	lockout_set(oldLockout);
}

/* The buffer we use when we have to throw away received data */
UInt8 DevNullBuffer[MAX_PACKET_SIZE];


/* Here's where we actually do the read.  This is called by our notifier, so it
   will run at "deferred task time", i.e., interrupt level.  */
void otudp_read(OTUDP *x) {
	TUnitData	unitdata;
	OTFlags		flags;
	OSStatus	err;
	OTBuffer *theOTBuffer;
	long shouldCheckAgain;
	PacketBuffer myPB;
	UInt8 *bufferToUse;
	int i;
	int numPacketsThisTime;
	short oldLockout;
	
//	post("** Entering otudp_read");
		
	// Set up the data structures OT wants to put the incoming data into
	unitdata.addr.maxlen = sizeof(struct InetAddress);
	unitdata.opt.maxlen = 0;
	unitdata.opt.buf = 0;
	unitdata.udata.maxlen = x->bufsize;

	// Mutual exclusion:  Make sure that neither Max not Open Transport will interrupt
	// this until we've read all there is to read.  That makes it safe to access the global
	// buffer lists in my object.

	oldLockout = AcquireLock(x);

	numPacketsThisTime = 0;
	shouldCheckAgain = 1;		// By default, after we do one read we should do another
	while (shouldCheckAgain) {
	
		// Try to allocate an available PacketBuffer
		myPB = PacketBufferListPop(&(x->freeBuffers));
		if (myPB == 0) {
			// Out of buffers, but we're still going to do the read just so we can
			// drop the packet (gracefully) without Open Transport blowing up.
			if (x->o_errorreporting) {
				if (!(x->o_num_dropped_packets)) {
					error("otudp: Out of packet buffers; have to drop packet(s)");
				}
				++(x->o_num_dropped_packets);
				x->o_ever_dropped_a_packet = 1;
			}
			unitdata.udata.buf = DevNullBuffer;
			unitdata.addr.buf = DevNullBuffer;
		} else {
			unitdata.udata.buf = (UInt8 *) myPB->buf;
			unitdata.addr.buf = (UInt8*) &(myPB->returnAddress);
		}
			
		// post("** About to call OTRcvUData");
		err = OTRcvUData(x->o_udp_ep, &unitdata, &flags);
		
		if (err == kOTNoError ) {
			if (flags == 0) {
				if (myPB != 0) {
	 				myPB->n = unitdata.udata.len;
					PBFIFOEnqueue(myPB, &(x->pendingBuffers));
					++numPacketsThisTime;
					clock_delay(x->o_clock, 0);
				}
			} else if (flags == T_MORE) {
				// Only the beginning of the (large) UDP packet fit in our buffer
				if (x->o_errorreporting) {
					error("OTUDP: packet too big; dropping.");
				}
				
				++numPacketsThisTime;
				while (flags == T_MORE) {
					err = OTRcvUData(x->o_udp_ep, &unitdata, &flags);
				}
				if (myPB) PacketBufferListPush(myPB, &(x->freeBuffers));
			} else {
				if (x->o_errorreporting) {
					error("OTUDP: OTRcvUData returned flags of %ld; dropped packet", (long) flags);
				}
				if (myPB) PacketBufferListPush(myPB, &(x->freeBuffers));
			}
		} else if ( err == kOTNoDataErr ) {
			/* We're supposed to keep trying to read incoming data until we get this
			   "error", so it's not really an error; it just says we've read enough
			   for now. */
			shouldCheckAgain = 0;
			if (myPB) {
				PacketBufferListPush(myPB, &(x->freeBuffers));
			} else {
				// We're out of buffers, but we didn't actually need one, so we need to un-count
				// this as a dropped packet
				--(x->o_num_dropped_packets);
			}
		} else {
			error("OTUDP: OTRcvUData returned error %ld; dropped packet", (long) err);
			if (myPB) PacketBufferListPush(myPB, &(x->freeBuffers));
		}
	}

end:
	
	ReleaseLock(x, oldLockout);

/*
	if (numPacketsThisTime > 1) {
		post("* Read %ld packets in one call to otudp_read", numPacketsThisTime);
	}
*/
}

void otudp_output(OTUDP *x) {
	// This is called by the Max clock.  It's guaranteed not to be called
	// at notifier level, although if we're in Overdrive this will be called
	// at interrupt level.
	Atom arguments[2];
	PacketBuffer b;
	short oldLockout;

	
	// BufferSanityCheck(x, 0, 0, 0);

	while (1) {
		oldLockout = AcquireLock(x);
		b = PBFIFODequeue(&(x->pendingBuffers));
		ReleaseLock(x,oldLockout);
		
		if (b == 0) break;
		
		SETLONG(&arguments[0], b->n);
		SETLONG(&arguments[1], (long) b->buf);
		outlet_anything(x->o_outlet, ps_FullPacket, 2, arguments);
		
		oldLockout = AcquireLock(x);
		PacketBufferListPush(b, &(x->freeBuffers));
		ReleaseLock(x,oldLockout);		
		
	}
	
	// BufferSanityCheck(x, 0, 0, 0);
}


#define OPTIONS_LENGTH 2048

void PostUDERR(char *source, EndpointRef ep) {
	OSStatus	err;
	TUDErr errBlock;
	struct InetAddress addr;
	unsigned char RoomForStupidOptions[OPTIONS_LENGTH];
	
	errBlock.addr.buf = (void *) &addr;
	errBlock.addr.maxlen = sizeof(addr);
#ifdef IGNORE_OPTIONS
	errBlock.opt.len = 0;
	errBlock.opt.maxlen = 0;
	errBlock.opt.buf = 0;
#else
	errBlock.opt.len = 0;
	errBlock.opt.maxlen = OPTIONS_LENGTH;
	errBlock.opt.buf = RoomForStupidOptions;
#endif
		
	err = OTRcvUDErr(ep, &errBlock);
	error("OTUDP: %s  \"protocol-dependent\" error code %ld", source, errBlock.error);
	{
		char hostNameString[255];
		OTInetHostToString(addr.fHost, hostNameString);
		post(" Addr. assoc w/ error is %s, port %d.", hostNameString, addr.fPort);
	}

	if (errBlock.error == 49) {
		post("Error 49 seems to mean that you're sending to a machine that's listening to");
		post("UDP but not on the port you're sending to.");
	}


	if (err == kOTNoError ) {
		// No further error; don't worry about it
	} else if (err == kOTLookErr) {
		OTResult r = OTLook(ep);
		error("Got kOTLookErr; OTLook returned %d", r);
	} else if (err == kOTFlowErr) {
		error("Flow control error.");
	} else if (err == kOTBufferOverflowErr) {
	    error("Open Transport says it doesn't have enough memory to tell me what the error is.");
	} else {	
		error("OTRcvUDErr returned %ld", err);
	}
}

/* My methods */

void otudp_version (OTUDP *x) {
	post("OTUDP (Open Transport UDP) Version " OTUDP_VERSION
		  ", by Matt Wright. Compiled " __TIME__ " " __DATE__);	
}

void otudp_toggleErrorReporting(OTUDP *x) {
	if (x->o_errorreporting) {
		post("OTUDP: Turning off error reporting.  What you don't know can't hurt you.");
		x->o_errorreporting = 0;
	} else {
		post("OTUDP: Turning on error reporting.");
		x->o_errorreporting = 1;
	}
}

void otudp_debugstats (OTUDP *x) {
	post("OTUDP debug stats: notifier invoked %ld times, gave up %ld times",
		 numTimesNotifierCalled, numTimesGaveUpForReentrancy);	
}

void otudp_resetdebugstats (OTUDP *x) {
	numTimesNotifierCalled = 0;
	numTimesGaveUpForReentrancy = 0;
}


void try_write(OTUDP *x, long length, void *bytes) {
	short oldLockout;
	PacketBuffer pb;
	
	
	// post("** try_write(%p, %ld, %p", x, length, bytes);
	
	if (length <= 0 || length > MAX_PACKET_SIZE) {
		error("¥ OTUDP: bad packet length %ld", length);
		return;
	}

	do_write(x, length, bytes);
}


void do_write(OTUDP *x, long length, void *bytes) {
	/* This should never be called at interrupt level */
	OSStatus err;
	InetAddress dest_addr;
	TUnitData udata;

			
	OTInitInetAddress(&dest_addr, x->o_inetPort, x->o_inetHost);
	udata.addr.len = sizeof(dest_addr);
	udata.addr.buf = (unsigned char *) &dest_addr;
	udata.opt.len = 0;
	udata.opt.buf = nil;
	
	udata.udata.len = length;
	udata.udata.buf = bytes;
	
	err = OTSndUData(x->o_udp_ep, &udata);
	
	if (err == kOTBadSyncErr) {
		error("¥ OTUDP: failed to send data at interrupt level.  Try turning off overdrive.");
	} else if (err == kEHOSTUNREACHErr) {
		error("OTUDP: Host address is unreachable; couldn't send packet");
		post("Make sure you're really on a network.");
	} else if (err != noErr) {
		error("¥ OTUDP: OTSndUData returned error code %ld.", err);
	}
}

void otudp_write(OTUDP *x, long size, long bufferPointer) {
		
	if (!(x->o_writer)) {
		error("otudp read objects can't write!  Get a grip!");
		return;
	}
	
	if (!(x->o_ready)) {
		if (x->o_udp_ep == 0) {
			error("otudp: Can't send a packet yet; the endpoint hasn't been created yet.");
		} else {
			error("otudp: Can't send a packet yet; the endpoint hasn't been bound yet.");
		}
		return;
	}
	
	try_write(x, size, (void *) bufferPointer);	
}

void otudp_old_write(OTUDP *x, Symbol *s, short argc, Atom *argv) {	
	
	// We want to discourage the old bad way of sending non-Max data to OTUDP,
	// but we provide the old interface for backwards-compatibility.
	
	if (!(x->o_complained_about_old_msgs)) {
		post("OTUDP: You shouldn't use old-style write+gimme messages.");
		post("You should upgrade the object that talks to OTUDP.");
		x->o_complained_about_old_msgs = 1;
	}
	
	if (!(x->o_writer)) {
		error("otudp read objects can't write!  Get a grip!");
		return;
	}
	
	if (!(x->o_ready)) {
		if (x->o_udp_ep == 0) {
			error("otudp: Can't send a packet yet; the endpoint hasn't been created yet.");
		} else {
			error("otudp: Can't send a packet yet; the endpoint hasn't been bound yet.");
		}
		return;
	}
	
	// Here's where we evily subvert Max's argc/argv
	try_write(x, argc, (void *) argv);
}

void otudp_tellmeeverything(OTUDP *x) {
	Boolean sane;
	int free, pending, outgoing;
	
	post("otudp_tellmeeverything: You asked for it");
	otudp_version(x);
	
	post("  Thanks: James McCartney, Adrian Freed, David Zicarelli, Chris Brown, Keith McMillen, etc.");
	
	post("  This object is a %s", x->o_writer ? "writer" : "reader");
	post("  This object error does %sreport errors", x->o_errorreporting ? "" : "not ");

	if (x->o_udp_ep == 0) {
		post("  This object has not created an endpoint yet.");
	} else {
		post("  This object's endpoint reference is %p", x->o_udp_ep);
		if (x->o_ready) {
			post("  And the endpoint has been bound, so we're ready to go.");
		} else {
			post("  But the endpoint has not been bound, so we're not ready.");
		}
		
		post("  Endpoint info: addr %ld, options %ld, tsdu %ld, etsdu %ld, connect %ld",
		     x->epinfo.addr, x->epinfo.options, x->epinfo.tsdu, x->epinfo.etsdu, x->epinfo.connect);
		post("                 discon %ld, servtype %lu, flags %lu",
			 x->epinfo.discon, x->epinfo.servtype, x->epinfo.flags);
	}
	if (x->o_writer) {
		char hostnameBuf[100];
		post("  You asked to write to internet host \"%s\"", x->o_inetHostName);
		OTInetHostToString(x->o_inetHost, hostnameBuf);
		post("  Writing to internet host %s", hostnameBuf);
		post("  Writing to port %ld", (long)( x->o_inetPort));
	}
	
	post("  This object reads from port %ld", (long) x->o_receiveInetPort);
	
	sane = BufferSanityCheck(x, &free, &pending, &outgoing);
	
	post("  Buffer sanity check %s", sane ? "passed" : "failed");
	post("  Buffers are %ld bytes", x->bufsize);
	post("  Allocated %ld buffers:", x->nbufs);
	post("     %ld currently available buffers", free);
	post("     %ld received-but-not-yet-outputted buffers", pending);
	post("     %ld Waiting-until-non-interrupt-time to be output buffers", outgoing);
	
	
	if (x->o_ever_dropped_a_packet) {
		if (x->o_num_dropped_packets) {
			post("  %d packets have been dropped since the last tellmeeverything", x->o_num_dropped_packets);
			x->o_num_dropped_packets = 0;
		} else {
			post("  Packets have been dropped, but not since the last tellmeeverything");
		}
	} else {
		post("  No packets have been dropped by this object");
	}
}

static Boolean BufferSanityCheck(OTUDP *x, int *freep, int *pendingp, int *outgoingp) {
	short oldLockout;
	int free, pending, outgoing, i;
	
	free = 100; // In case we fail to acquire lock
	
	// Mutual exclusion to prevent the lists from changing out from under me
	
	oldLockout = AcquireLock(x);
	free = CountPacketList(x->freeBuffers);
	pending = CountPacketList(x->pendingBuffers.first);
	outgoing = 0;
	ReleaseLock(x,oldLockout);

	if (freep != 0) *freep = free;
	if (pendingp != 0) *pendingp = pending;
	if (outgoingp != 0) *outgoingp = outgoing;

	if (free+pending+outgoing != x->nbufs) {
		error("otudp: BufferSanityCheck failed!  %d free + %d pending + %d outgoing = %d", 
			free, pending, free+pending+outgoing);
		return false;
	} else {
		return true;
	}

}


static int CountPacketList(PacketBuffer l) {
	int result = 0;
	while (l != 0) {
		++result;
		l = l->next;
	}
	return result;
} 
