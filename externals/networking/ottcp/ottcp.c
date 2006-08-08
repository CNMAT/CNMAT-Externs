/* 
Copyright (c) 1999,2000,01,02,03,04,05,06.  The Regents of the
University of California (Regents).  All Rights Reserved.

Permission to use, copy, modify, and distribute this software and its
documentation for educational, research, and not-for-profit purposes, without
fee and without a signed licensing agreement, is hereby granted, provided that
the above copyright notice, this paragraph and the following two paragraphs
appear in all copies, modifications, and distributions.  Contact The Office of
Technology Licensing, UC Berkeley, 2150 Shattuck Avenue, Suite 510, Berkeley,
CA 94720-1620, (510) 643-7201, for commercial licensing opportunities.

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
NAME: ottcp
DESCRIPTION: Open Transport TCP object for Max - client (a.k.a. active) side only.  Never really tested or debugged.
AUTHORS: Matt Wright
COPYRIGHT_YEARS: 1999,2000,01,02,03,04,05,06
SVN_REVISION: $LastChangedRevision$
VERSION 0.0: Matt's initial version 5/19/99  (based on OTUDP object). Still needs substantial debugging.  Use at your own risk.
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  

 
 Arguments specify IP address and TCP port of server we want to talk to.
 Our (ephermeral) TCP port number is generated automagically.
 
 Two ways to read:
 
 - nbytes <num>: reads the next <num> bytes, then outputs the symbol OTTCP_nbytes
   followed by the number of bytes, followed by a pointer to a buffer containing the
   bytes that were read.
   
 - delim <symbol>: keeps reading until it sees the sequence of bytes given in the symbol
   argument, then outputs the symbol OTTCP_delim followed by the number of bytes that it
   read, including the delimiter bytes, followed by a pointer to a buffer containing
   those bytes, including the delimiter bytes. 
 
 - delim-bytes <num> [<num> [<num> ...]]: Same as delim, but the delimiter bytes
   are given explicitly as a bunch of integers instead of as a Max Symbol.
 
 Buffers:  
 
 - For incoming data, we use a double buffer strategy.  We receive incoming
   data into o_currentReadBuf, incrementing o_bytesRead.  Once we have gathered
   whatever unit of data we want to output to Max, we switch the current/next
   read buffers, so that new incoming data goes into the other buffer while
   we output the first buffer.
  
 - For outgoing data, flow control restrictions may prevent us from sending
   when we're ready to send, so we use a circular buffer, o_WriteBuf.  The
   "read position" in the write buffer points to the next byte that we'll give
   OpenTransport when it's ready for more.  The "write position" is where we'll
   write additional data we want to send but can't yet.



To Do: 
On disconnect, reset all buffers


 
 */

#define DEBUG
#define OTTCP_VERSION "0.0"
#define DEFAULT_BUFFER_SIZE  1024
#define MAX_TO_READ_LOOKING_FOR_DELIMITER 128

#include "ext.h"

/* We need to access the field "error" in the Open Transport TUDErr structure,
   so we have to undo the Max function table macro for error() */
#undef error

#include <SetUpA4.h>
#include <A4Stuff.h>
#include <OpenTransport.h>
#include <OpenTptInternet.h>
#include <LowMem.h>				// For A5 stuff for 68K notifier
#include <OSUtils.h>			// Same reason


// The states we can be in, in terms of reading incoming data:

typedef enum {
	SETTING_UP,		// Still binding, connecting, etc. - not ready to read
	NO_REQUEST,		// User hasn't asked for anything, so we don't read yet	
	GET_NBYTES,		// We're reading a given number of bytes
	GET_DELIM,		// We're reading until we see a certain delimiter string
	OUTPUTTING		// Waiting to or in the middle of outputting data
} State;

/* structure definition */
typedef struct ottcp {
	// Max stuff
	Object o_ob;
	long o_a5;			    /* Holds a5 so notifier callback can work on 68k */
	void *o_outlet;
	void *o_clock;
	void *o_connectedclock;
	Symbol *o_connectionSymbol;	/* "connected" or "disconnected", output by o_connectedclock */

	int o_errorreporting;	/* 1 for error reporting; 0 for silent */
	State o_state;
	State o_whatToOutput;	/* When o_state is OUTPUTTING, this is GET_NBYTES or GET_DELIM */
	State o_nextRequest;	/* In case user requests another read while o_state is OUTPUTTING */
	Boolean o_datawaiting;	/* If we know there's data we haven't read. */
	long o_numBytesWeWant;	/* Number of bytes we've been requested to gather */
	char *o_delimiterBytes;	/* The delimiter string we're searching for. */
	long o_delimiterLen;		/* Size of delimiter string (not necessarily null-terminated) */
	Boolean o_needToFreeDelimiter;  /* True if memory in o_delimiterBytes came from getbytes() */

	// Internet addresses and ports associated with this object
	char *o_inetHostName;   /* String the user gave for desired host to send to */
	InetPort o_inetPort;	/* OT's representation of the port to send to */
	InetHost o_inetHost;	/* OT's representation of the host to send to.  When 0
							   it means we don't know who to connect to. */

	InetPort o_receiveInetPort;	/* All objects: port user asked for */
	TBind o_bindAddrWeActuallyGot;  /* So OT can tell us what port number we really bound to */
	struct InetAddress o_addrWeActuallyGot;  /* Same deal */

	// Open Transport stuff	
	EndpointRef o_tcp_ep;
	TEndpointInfo epinfo;
	OTLock o_readlock;
	
	// Buffer management
	long o_ReadBufSize;
	char *o_ReadBufA, *o_ReadBufB;			// Original buffers we allocated
	char *o_currentReadBuf, *o_nextReadBuf;	// These two pointers swap AB/BA
	long o_bytesRead;							// Current position in o_currentReadBuf
	long o_bytesReadForNextTime;			// Current position in o_nextReadBuf

	long o_WriteBufSize;
	char *o_WriteBuf;					// Circular buffer
	long o_WBReadPos;					// read position for write buffer
	long o_WBWritePos;					// write position for write buffer
} OTTCP;

/* globals */
fptr *FNS;
void *ottcp_class;
Symbol *ps_readbufsize, *ps_writebufsize, *ps_OTTCP_nbytes, *ps_OTTCP_delim, *ps_connected,
       *ps_disconnected;


/* prototypes */
void *ottcp_new(Symbol *s, short argc, Atom *argv);
int ParseArgs(short argc, Atom *argv, char **inetHostNamep, InetPort *portp, long *rbufsizep, long *wbufsizep);
void ottcp_free(OTTCP *x);
char *StateName(State s);
void ChangeState(OTTCP *x, State s);
static short AcquireLock(OTTCP *x);
static void ReleaseLock(OTTCP *x, short oldLockout);
static int LookUpInetHost(char *name, InetHost *result);
void BindTheEndpoint(OTTCP *x);
pascal void OTTCPNotifier(void* vobj, OTEventCode code, OTResult result, void* cookie);
static void change_connection(OTTCP *x);
void PostUDERR(char *source, EndpointRef ep);
void ottcp_handleIncomingData(OTTCP *x);
Boolean FoundDelimiter(OTTCP *x, char **afterDelimiter);
void CopyAfterDelimiterToOtherBuffer(OTTCP *x, char *afterDelimiter);
Boolean SeeIfWeCanAlreadyOutput(OTTCP *x);
void do_output(OTTCP *x);
void printOutput(OTTCP *x);
void do_output_connected(OTTCP *x);
void ottcp_read_nbytes(OTTCP *x, long size);
void ottcp_read_until_delimiter_symbol(OTTCP *x, Symbol *delimiter);
void ottcp_read_until_delimiter_bytes(OTTCP *x, Symbol *selector, short argc, Atom *argv);
int do_read_until_delimiter(OTTCP *x, char *delimiterBytes, long delimiterLen);
static void ottcp_connect(OTTCP *x, Symbol *hostName, long port);
static void ottcp_disconnect(OTTCP *x);
void ottcp_write(OTTCP *x, long size, long bufferPointer);
void do_write(OTTCP *x, long length, void *bytes);
void AddToWriteBuffer(OTTCP *x, long length, char *bytes);
void ottcp_assist (OTTCP *x, void *box, long msg, long arg, char *dstString);
void ottcp_version(OTTCP *x);
void ottcp_ErrorReporting(OTTCP *x, long yesno);
void ottcp_tellmeeverything(OTTCP *x);
long strlen (const char *s);


#define YEAR 1999
#define MONTH 8
#define MONTH1 -1
#define DAY 1
#define EXPIRATION_NOTICE "ottcp object version " OTTCP_VERSION " has expired!"

/* initialization routine */

void main(fptr *f) {
	long oldA4;
	OSStatus err;
	DateTimeRec date;
	
	oldA4 = SetCurrentA4();
	RememberA4();
	FNS = f;	

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

	ps_readbufsize = gensym("readbufsize");
	ps_writebufsize = gensym("writebufsize");
	ps_OTTCP_nbytes = gensym("OTTCP_nbytes");
	ps_OTTCP_delim = gensym("OTTCP_delim");
	ps_connected = gensym("connected");
	ps_disconnected = gensym("disconnected");

	setup(&ottcp_class, ottcp_new, (method) ottcp_free, (short)sizeof(OTTCP), 0L, A_GIMME, 0);
	
	post("ottcp object version " OTTCP_VERSION " by Matt Wright. ");
#ifdef DAVID_LIKES_EXPIRING_MAX_OBJECTS
	post("Expires %d/%d/%d", MONTH, DAY, YEAR);
#endif
	post("Copyright й 1999 Regents of the University of California. All Rights Reserved.");

#ifdef DAVID_LIKES_EXPIRING_MAX_OBJECTS
	GetTime(&date);
	if((date.year > YEAR)  || 
       (date.year == YEAR && ((date.month > MONTH) ||
							  (date.month == MONTH && date.day > DAY)))) {
            ouchstring(EXPIRATION_NOTICE);
            goto quit;
    }
#endif
    
    // post ("*** before  InitOpenTransport();");
	err = InitOpenTransport();
	if (err != kOTNoError) {
		ouchstring("Couldn't InitOpenTransport (err %d).  Perhaps Open Transport is not installed.",
					err);
		goto quit;
	}
	
	/* bind methods */
	addmess((method) ottcp_connect, "connect", A_SYM, A_LONG, 0);
	addmess((method) ottcp_disconnect, "disconnect", 0);
	addmess((method) ottcp_read_nbytes, "nbytes", A_LONG, 0);
	addmess((method) ottcp_read_until_delimiter_symbol, "delim", A_SYM, 0);
	addmess((method) ottcp_read_until_delimiter_bytes, "delim-bytes", A_GIMME, 0);
	addmess((method) ottcp_write, "write", A_LONG, A_LONG, 0);
	addmess((method) ottcp_version, "version", 0);
	addmess((method) ottcp_ErrorReporting, "errorreporting", 0);
	addmess((method) ottcp_tellmeeverything, "tellmeeverything", 0);
	addmess((method) ottcp_assist, "assist", A_CANT, 0);

	finder_addclass("System","ottcp");

quit:
	RestoreA4(oldA4);
}


void *ottcp_new(Symbol *s, short argc, Atom *argv) {
	OTTCP *x;
	OSStatus err;
	
	// These variables will be filled with the args we parse
	char *inetHostName = 0;
	InetPort port = 0;
	long readbufsize = DEFAULT_BUFFER_SIZE;
	long writebufsize = DEFAULT_BUFFER_SIZE;
	
	EnterCallback();
		
	if (ParseArgs(argc, argv, &inetHostName, &port, &readbufsize, &readbufsize) == 0) {
		ouchstring("ottcp usage: \"ottcp [<hostname> <port>] [readbufsize <nbytes>] [writebufsize <nbytes>]\"");
		ExitCallback();
		return 0;
	}
	
	post("Parsed args: inetHostName %s, port %ld, readbufsize %ld, writebufsize %ld",
		 inetHostName, port, readbufsize, writebufsize);
	
	x = newobject(ottcp_class);
	x->o_a5 = (long) LMGetCurrentA5();
	x->o_inetHostName = inetHostName;
	x->o_inetPort = port;
	x->o_inetHost = 0;

	if (x->o_inetHostName != 0) {
		// They gave the server's IP address and port # as arguments
		if (LookUpInetHost(x->o_inetHostName, &(x->o_inetHost)) == 0) {
			post("е OTTCP: can't understand host \"%s\"; not connecting",  x->o_inetHostName);
		}
	}

	x->o_state = SETTING_UP;
	x->o_datawaiting = 0;
	x->o_errorreporting = 1;
	x->o_outlet = outlet_new(x,0L);
	x->o_clock = clock_new(x, (method) do_output);
	x->o_connectedclock = clock_new(x, (method) do_output_connected);
	x->o_connectionSymbol = 0;
	
	OTClearLock(&(x->o_readlock));
	
	/* Allocate buffers */
	x->o_ReadBufSize = readbufsize;
	x->o_WriteBufSize = writebufsize;
	
	x->o_ReadBufA = NewPtr(readbufsize);
	x->o_ReadBufB = NewPtr(readbufsize);
	x->o_WriteBuf = NewPtr(writebufsize);
	if (x->o_ReadBufA == 0 || x->o_ReadBufB == 0 || x->o_WriteBuf == 0) {
		ouchstring("ottcp: not enough memory for two %ld byte read buffers and a %ld byte write buffer.",
				   readbufsize, writebufsize);
	}
	
	x->o_currentReadBuf = x->o_ReadBufA;
	x->o_nextReadBuf = x->o_ReadBufB;
	x->o_bytesRead = 0;
	x->o_bytesReadForNextTime = 0;
	x->o_WBReadPos = 0;
	x->o_WBWritePos = 0;
	

	/* Make the endpoint */	
	x->o_tcp_ep = 0;		// Indicates endpoint hasn't been created yet.
	err = OTAsyncOpenEndpoint(OTCreateConfiguration(kTCPName), 0, &(x->epinfo), OTTCPNotifier, x);
	if (err != noErr) {
		ouchstring("otudp: Error %d from OTAsyncOpenEndpoint. This is bad.", err);
		return 0;
	}
	// This didn't actually make the endpoint; my notifier should get the T_OPENCOMPLETE
	// event after the endpoint is opened.


	ExitCallback();
	return (x);
}

int ParseArgs(short argc, Atom *argv, char **inetHostNamep, InetPort *portp, 
			  long *rbufsizep, long *wbufsizep) {
	int i;

	i = 0;
	
	while (i < argc) {
		if (argv[i].a_type != A_SYM) return 0;
		
		if (argv[i].a_w.w_sym == ps_readbufsize) {
			if (i+1 >= argc || argv[i+1].a_type != A_LONG) return 0;
			*rbufsizep = argv[i+1].a_w.w_long;
			i += 2;
		} else if (argv[i].a_w.w_sym == ps_writebufsize) {
			if (i+1 >= argc || argv[i+1].a_type != A_LONG) return 0;
			*wbufsizep = argv[i+1].a_w.w_long;
			i += 2;
		} else if (i+1 < argc && argv[i+1].a_type == A_LONG) {
			*inetHostNamep = argv[i].a_w.w_sym->s_name;
			*portp = (InetPort) argv[i+1].a_w.w_long;
			i += 2;
		} else {
			return 0;
		}
	}
	
	return 1;
}

void ottcp_free(OTTCP *x) {
	freeobject(x->o_clock);
	freeobject(x->o_connectedclock);
    if (x->o_tcp_ep != 0) {
		OTCloseProvider(x->o_tcp_ep);
	}
	
	DisposePtr(x->o_ReadBufA);
	DisposePtr(x->o_ReadBufB);
	DisposePtr(x->o_WriteBuf);
}


char *StateName(State s) {
	if (s == SETTING_UP) {
		return "SETTING_UP";
	} else if (s == NO_REQUEST) {
		return "NO_REQUEST";
	} else if (s == GET_NBYTES) {
		return "GET_NBYTES";
	} else if (s == GET_DELIM) {
		return "GET_DELIM";
	} else if (s == OUTPUTTING) {
		return "OUTPUTTING";
	} else {
		post("е OTTCP: error: Unrecognized state %ld.  Uh-oh.", s);
		return "Yucky!";
	}
}

void ChangeState(OTTCP *x, State s) {
#ifdef DEBUG
	post("Changing state from %s to %s", StateName(x->o_state), StateName(s));
#endif
	x->o_state = s;
}

/* Mutual exclusion stuff */

static short AcquireLock(OTTCP *x) {
	// Mutual exclusion:  Make sure that neither Max not Open Transport will interrupt us

	int i;
	short oldLockout;

	
	oldLockout = lockout_set(1);
	OTEnterNotifier(x->o_tcp_ep);

#define TOO_MANY_SPINS 1
	i = 0;
	while (OTAcquireLock(&(x->o_readlock)) == false) {
		if ((++i) > TOO_MANY_SPINS) {
			ouchstring("OTAcquireLock keeps failing inside ottcp_read!");
		}
	}

	return oldLockout;
}

static void ReleaseLock(OTTCP *x, short oldLockout) {
	OTClearLock(&(x->o_readlock));
	OTLeaveNotifier(x->o_tcp_ep);
	lockout_set(oldLockout);
}


/* Open Transport helper procedures */
static int LookUpInetHost(char *name, InetHost *result) {
	OSStatus err;

	// If the hostname is dots and numbers, like "128.32.122.13", we don't neet DNR
	err = OTInetStringToHost(name, result);
	if (err != noErr) {
		// We do need DNR.
		InetSvcRef tcpipProvider;
		InetHostInfo hinfo;
		
		tcpipProvider = OTOpenInternetServices(kDefaultInternetServicesPath, 0, &err);
		
		if (err != noErr) {
			post("е otudp: Error %d from OTOpenInternetServices().", err);
			return 0;
		}
		
		err = OTInetStringToAddress(tcpipProvider, name, &hinfo);
		OTCloseProvider(tcpipProvider);
		if (err != noErr) {
			post("е otudp: Error %d from OTInetStringToAddress", err);
			return 0;
		}
		*result = hinfo.addrs[0];
	}
	return 1;
}

void BindTheEndpoint(OTTCP *x) {
	OSStatus err;
	
	/* Set up the data structure OTBind will use to say what port number we get */
	x->o_bindAddrWeActuallyGot.addr.maxlen = sizeof(struct InetAddress);
	x->o_bindAddrWeActuallyGot.addr.buf = (unsigned char *) &(x->o_addrWeActuallyGot);
	
	/* Let OTBind pick a return address port number for us */
    err = OTBind(x->o_tcp_ep, nil, &(x->o_bindAddrWeActuallyGot));
	if (err != kOTNoError) {
		ouchstring("otudp: Error %d from OTBind", err);
		return;
	}
}

pascal void OTTCPNotifier(void* vobj, OTEventCode code, OTResult result, void* cookie) {
	OTTCP *x = (OTTCP *) vobj;
	long oldA5;
	OSStatus s;
		
	EnterCallback();
	oldA5 = SetA5(x->o_a5);

	post("*** OTTCPNotifier(code %x, result %ld)", (long) code, (long) result);
	
	if (code == T_OPENCOMPLETE) {
		if (result != noErr) {
			post("OTTCPNotifier got a T_OPENCOMPLETE code with error %ld", result);
		}

		// Even if we got an error, we'll use the endpoint.  
		x->o_tcp_ep = (EndpointRef) cookie;
		
		if (x->o_inetHostName != 0) {
			BindTheEndpoint(x);
			// Now we expect a T_BINDCOMPLETE message.
		}
		
	} else if (code == T_BINDCOMPLETE) {
		TCall sndCall;
		InetAddress inAddr;
		
		// See what ephemeral port number OT gave us
		x->o_receiveInetPort = x->o_addrWeActuallyGot.fPort;
		
		// Now the endpoint has been bound; next trick is connecting
		OTInitInetAddress(&inAddr, x->o_inetPort, x->o_inetHost);
		OTMemzero(&sndCall, sizeof(TCall));
		sndCall.addr.buf = (void *) &inAddr;
		sndCall.addr.len = sizeof(InetAddress);

		post("** About to OTConnect");
		s = OTConnect(x->o_tcp_ep, &sndCall, NULL);
		if (s == kOTNoDataErr) {
			// No problem; that means connection is in progress
			post("** OTConnect returned kOTNoDataErr - good");
		} else {
			post("е OTTCP: error: OTConnect returned %ld; not connecting.", s);
		}
		// Now we expect T_CONNECT
		
	} else if (code == T_CONNECT) {
		if (result == kOTBadAddressErr) {
			post("е OTTCP: error: bad address.  Disconnecting");
			x->o_inetHost = 0;
			change_connection(x);
		} else {
			post("** Got T_CONNECT");
			post("** Result passed to notifier: %ld", (long) result);
			post("** Endpoint state before OTRcvConnect: %ld", (long) OTGetEndpointState(x->o_tcp_ep));
			// We could pass in buffers to find out the actual address we connected to, etc.
			s = OTRcvConnect(x->o_tcp_ep, nil);
			if (s != kOTNoError) {
				post("е OTTCP: error: RcvConnect returned %ld", s);
			}
			post("** Endpoint state after OTRcvConnect: %ld", (long) OTGetEndpointState(x->o_tcp_ep));
			ChangeState(x, NO_REQUEST);
			x->o_connectionSymbol = ps_connected;
			clock_delay(x->o_connectedclock, 0);
		}
	} else if (code == T_UNBINDCOMPLETE) {
		// If we have a new inetHost, try to bind it
		if (x->o_inetHost != 0) {
			BindTheEndpoint(x);
			// Now we expect T_BINDCOMPLETE
		} else {
			// No inetHost, so we're disonnected
			x->o_connectionSymbol = ps_disconnected;
			clock_delay(x->o_connectedclock, 0);
		}
	} else if (code == T_DISCONNECTCOMPLETE) {
		// We always want to rebind when we reconnect, so after disconnecting we unbind
		s = OTUnbind(x->o_tcp_ep);
		if (s != kOTNoError) {
			post("OTTCP: warning: OTUnbind returned %ld", s);
		}		
	} else if (code == T_DATA) {
		ottcp_handleIncomingData(x);
	} else if (code == T_UDERR) {
		if (x->o_errorreporting) {
			PostUDERR("OTTCPNotifier was called for a T_UDERR code", x->o_tcp_ep);
		} else {
			/* We still have to receive the error to clear the condition */
			OTRcvUDErr(x->o_tcp_ep, 0);
		}
	} else if (code == T_GODATA) {
		// Flow control restriction has lifted; now it's OK to send more data.
		do_write(x, 0, 0);
	} else if (code == T_DISCONNECT) {
		// The other dude wants to disconnect
		post("OTTCP: peer wants to disconnect");
		s = OTRcvDisconnect(x->o_tcp_ep, NULL);
		if (s == kOTNoError) {
			x->o_inetHost = 0;
			change_connection(x);
		} else if (s == kOTNoDisconnectErr) {
			// false alarm
		} else {
			post("е OTTCP: error: OTRcvDisconnect returned %ld", s);
		}
	} else if (code == T_ORDREL) {
		post("*** Got T_ORDREL");
		s = OTRcvOrderlyDisconnect(x->o_tcp_ep);
		if (s == kOTNoReleaseErr) {
			post("...false alarm!");
		} else if (s == kOTNoError) {
			x->o_inetHost = 0;
			change_connection(x);
		} else {
			post("е OTTCP: error: OTRcvOrderlyDisconnect returned %ld", s);
		}
	} else {
	 	if (x->o_errorreporting) {
			post("OTTCP: Unrecognized OTEventCode %ld in event handler.  Oh well.", (long) code);
		}
	}

done:
	SetA5(oldA5);
	ExitCallback();
}

static void change_connection(OTTCP *x) {
	// This sets in motion the following sequence of events:
	//   - Disconnect the endpoint
	//   - Unbind the endpoint
	//   - If (x->o_inetHost != 0), bind the endpoint again
	//   - connect the endpoint
	// Each of these events is triggered in the notifier function when the previous
	// event completes, so the job of this procedure is just to see what state we're
	// in and start the process.
	
	OTResult r;
	OSStatus s;
	
	ChangeState(x, SETTING_UP);

	r = OTGetEndpointState(x->o_tcp_ep);
	
	if (r == T_UNINIT) {
		// This shouldn't happen
		ouchstring("OTTCP: Connect: didn't expect endpoint state to be T_UNINIT!");
	} else if (r == T_UNBND) {
		// Endpoint is already unbound, so we're completely disconnected.
		// If we have a new inetHost, try to bind it
		if (x->o_inetHost != 0) {
			BindTheEndpoint(x);
			// Now we expect T_BINDCOMPLETE
		}
		// Now we expect T_BINDCOMPLETE
	} else if (r == T_IDLE) {
		// Endpoint is bound, but not connected.
		s = OTUnbind(x->o_tcp_ep);
		if (s != kOTNoError) {
			post("OTTCP: warning: OTUnbind returned %ld", s);
		}
		// Now we expect T_UNBINDCOMPLETE
	} else if (r == T_OUTCON || r == T_DATAXFER) {
		// We're either starting a connection or the connection is currently established, 
		// so send an orderly disconnect
		post ("** Sending orderly disconnect ");
		s = OTSndOrderlyDisconnect(x->o_tcp_ep);
		if (s != kOTNoError) {
			post("OTTCP: warning: OTSndOrderlyDisconnect returned %ld", s);
		}
		// I guess now we expect T_DISCONNECTCOMPLETE

	} else if (r == T_INCON) {
		ouchstring("OTTCP: Connect: didn't expect endpoint state to be T_INCON!");
	} else if (r == T_OUTREL) {
		// We already tried an orderly disconnect, but it still hasn't been acknowledged,
		// so screw orderliness and try a regular diconnect
		TCall call;
		call.addr.len = 0;
		call.opt.len = 0;
		call.udata.len = 0;
		call.addr.maxlen = 0;
		call.opt.maxlen = 0;
		call.udata.maxlen = 0;
		
		s = OTSndDisconnect(x->o_tcp_ep, &call);
		if (s != kOTNoError) {
			post("OTTCP: warning: OTSndDisconnect returned %ld", s);
		}
		// Now we expect T_DISCONNECTCOMPLETE
	} else if (r == T_INREL) {
		// Our peer wants an orderly disonnect, so let's acknowlegde it!
		
	} else {
		post("   еее Unrecognized endpoint state %ld!", r);
	}	
}


void PostUDERR(char *source, EndpointRef ep) {
	OSStatus	err;
	TUDErr errBlock;
	struct InetAddress addr;
#define OPTIONS_LENGTH 2048
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
	post("OTUDP: %s  \"protocol-dependent\" error code %ld", source, errBlock.error);
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
		post("Got kOTLookErr; OTLook returned %d", r);
	} else if (err == kOTFlowErr) {
		post("Flow control error.");
	} else if (err == kOTBufferOverflowErr) {
	    post("Lame Open Transport says it doesn't have enough memory0 to tell me what my problem is.");
	} else {	
		post("OTRcvUDErr returned %ld", err);
	}
}

void ottcp_handleIncomingData(OTTCP *x) {
	/* Here's where we actually do the read.  This is called by our notifier, so it
  	   will run at "deferred task time", i.e., interrupt level.  */
	
	OTFlags flags;
	OTResult r;
	short oldLockout;

	EnterCallback();

//	post("** Entering ottcp_handleIncomingData");

	// Are we in a state where we want to read data that comes in?
	
	if (x->o_state == SETTING_UP) {
		post("е OTTCP: How could ottcp_handleIncomingData() be called in state SETTING_UP?");
	} else if (x->o_state == NO_REQUEST) {
		// The rest of this Max program hasn't asked us for any data yet, so
		// we won't read it out of OpenTransport.  That way, when OT's buffers
		// fill up, TCP flow control will happen correctly.
		x->o_datawaiting = 1;
	} else if (x->o_state == OUTPUTTING) {
		// We're still busy trying to output the last chunk of data, so again
		// we just leave the new data in OT's buffers so flow control will work.
		x->o_datawaiting = 1;
	} else if (x->o_state == GET_NBYTES) {
		// Try to read N bytes
		long numLeftToRead = x->o_numBytesWeWant - x->o_bytesRead;
		if (numLeftToRead <= 0) {
			post("е OTTCP: internal error: read too much.");
		} else {
			oldLockout = AcquireLock(x);
			r = OTRcv(x->o_tcp_ep, x->o_currentReadBuf+x->o_bytesRead, numLeftToRead, &flags);
			ReleaseLock(x,oldLockout);
			if (r == numLeftToRead) {
				// We read everything we wanted, so output the darn buffer
				x->o_bytesRead += r;
				ChangeState(x, OUTPUTTING);
				x->o_whatToOutput = GET_NBYTES;
				clock_delay(x->o_clock, 0);
				// Maybe there's more data to be read
				x->o_datawaiting = 1;	
			} else if (r > 0) {
				// We read some, but not all, of our N bytes.
				x->o_bytesRead += r;
				x->o_datawaiting = 0;
			} else if (r == kOTNoDataErr) {
				// We didn't read squat, but that's supposed to be OK.
				x->o_datawaiting = 0;
			} else if (r == kOTLookErr) {
				OTResult lookr = OTLook(x->o_tcp_ep);
				post("OTTCP: OTRcv returned kOTLookErr; OTLook returned %ld", lookr);
			} else {
				post("е OTTCP: warning: OTRcv returned %ld", r);
			}
		}
	} else if (x->o_state == GET_DELIM) {
		long capacity = x->o_ReadBufSize - x->o_bytesRead;
		long numToRead;
		
		if (capacity > MAX_TO_READ_LOOKING_FOR_DELIMITER) {
			numToRead = MAX_TO_READ_LOOKING_FOR_DELIMITER;
		} else {
			numToRead = capacity;
		}
	readagain:
		oldLockout = AcquireLock(x);
		r = OTRcv(x->o_tcp_ep, x->o_currentReadBuf+x->o_bytesRead, numToRead, &flags);
		ReleaseLock(x,oldLockout);

		if (r == numToRead) {
			// We read everything we asked for
			char *afterDelimiter;
			
			x->o_datawaiting = 1;	// Maybe there's more data to be read
			x->o_bytesRead += r;
			if (FoundDelimiter(x, &afterDelimiter)) {
				ChangeState(x, OUTPUTTING);
				x->o_whatToOutput = GET_DELIM;
				CopyAfterDelimiterToOtherBuffer(x, afterDelimiter);
				clock_delay(x->o_clock, 0);
			} else if (x->o_bytesRead == x->o_ReadBufSize) {
				post("е OTTCP: error: filled %ld byte read buffer but never found delimiter string \"%s\"",
					 x->o_ReadBufSize, x->o_delimiterBytes);
				post("Discarding buffer and calling off the search.  You probably want to reconnect.");
				ChangeState(x, NO_REQUEST);
				x->o_bytesRead = 0;
			} else {
				// We didn't find the delimiter in the MAX_TO_READ_LOOKING_FOR_DELIMITER bytes
				// we read, but we've got more room in our buffer and there may be more
				// data in the OT buffer, so let's do another read.
				goto readagain;
			}
		} else if (r > 0) {
			// We read everything that was available
			char *afterDelimiter;
			x->o_datawaiting = 0;
			x->o_bytesRead += r;
			
			if (FoundDelimiter(x, &afterDelimiter)) {
				ChangeState(x, OUTPUTTING);
				x->o_whatToOutput = GET_DELIM;
				CopyAfterDelimiterToOtherBuffer(x, afterDelimiter);
				clock_delay(x->o_clock, 0);
			} else {
				// We didn't see the delimiter, TCP's buffer is empty, and our buffer
				// still has room, so there's nothing else to do until the next T_DATA event.
			}
		} else if (r == kOTNoDataErr) {
			// We didn't read squat, but that's supposed to be OK.
			x->o_datawaiting = 0;
		} else if (r == kOTLookErr) {
			OTResult lookr = OTLook(x->o_tcp_ep);
			post("OTTCP: OTRcv returned kOTLookErr; OTLook returned %ld", lookr);
		} else {
			post("е OTTCP: warning: OTRcv returned %ld", r);
		}
	} else {
		post("е OTTCP: error: unrecognized state %ld", x->o_state);
	}

bag:
	ExitCallback();
}

Boolean FoundDelimiter(OTTCP *x, char **afterDelimiter) {
	// See if the delimiter string appears in x->o_currentReadBuf
	// If so, return true, and set *afterDelimiter to point to the next character
	// after the delimiter string.  Else return false.

	// Naive algorithm is O(n^2)
	// If we ever make this use regular expressions, check all the logic calling
	// this procedure that looks at numbers of bytes of things
	
	int i, j;
	
	for (i = 0; i < x->o_bytesRead; ++i) {
		for (j = 0; i < x->o_delimiterLen; ++j) {
			if (x->o_currentReadBuf[i+j] != x->o_delimiterBytes[j]) break;
		}
		*afterDelimiter = x->o_currentReadBuf+i+j+1;
		return 1;
	}
	return 0;
}

void CopyAfterDelimiterToOtherBuffer(OTTCP *x, char *afterDelimiter) {
	// afterDelimiter points to the first character in x->o_currentReadBuf after
	// the delimiter we were looking for.  Copy the rest of the contents of that
	// buffer to the other buffer.
	int i;
	char *p, *boundary;
	short oldLockout;
	
	boundary = x->o_currentReadBuf + x->o_bytesRead;

	oldLockout = AcquireLock(x);
	
	for (i = 0, p = afterDelimiter; p < boundary; ++p, ++i) {
		x->o_nextReadBuf[i] = *afterDelimiter;
	}
	x->o_bytesReadForNextTime = i;
	x->o_bytesRead = afterDelimiter - x->o_currentReadBuf;
	ReleaseLock(x,oldLockout);

}

Boolean SeeIfWeCanAlreadyOutput(OTTCP *x) {
	// We just got a request to read something, so maybe it's in the buffer of
	// what we've already read
	
	if (x->o_state == GET_NBYTES) {
		if (x->o_bytesRead >= x->o_numBytesWeWant) {
			// yes; we've read enough
			ChangeState(x, OUTPUTTING);
			x->o_whatToOutput = GET_NBYTES;
			if (x->o_bytesRead > x->o_numBytesWeWant) {
				x->o_bytesReadForNextTime = x->o_bytesRead - x->o_numBytesWeWant;
				OTMemcpy(x->o_nextReadBuf, x->o_currentReadBuf+x->o_numBytesWeWant,
					     x->o_bytesReadForNextTime);
				x->o_bytesRead = x->o_numBytesWeWant;
			}					
			clock_delay(x->o_clock, 0);
			return true;
		} else {
			return false;
		}
	} else if (x->o_state == GET_DELIM) {
		char *after;
		if (x->o_bytesRead >= x->o_delimiterLen) {
			if (FoundDelimiter(x, &after)) {
				// yes, we've read through the delimiter
				ChangeState(x, OUTPUTTING);
				x->o_whatToOutput = GET_DELIM;
				CopyAfterDelimiterToOtherBuffer(x, after);
				clock_delay(x->o_clock, 0);
				return true;
			}
		}
		return false;
	} else {
		post("е OTTCP: internatl error: SeeIfWeCanAlreadyOutput called, state = %ld", x->o_state);
		return false;
	}
}
	

void do_output(OTTCP *x) {
	// This is called by the Max clock.  It's guaranteed not to be called
	// at notifier level, although if we're in Overdrive this will be called
	// at interrupt level.
	Atom arguments[2];
	short oldLockout;

	x->o_nextRequest = NO_REQUEST;

#ifdef DEBUG
	printOutput(x);
#endif

	SETLONG(&arguments[0], x->o_bytesRead);
	SETLONG(&arguments[1], (long) x->o_currentReadBuf);
	if (x->o_whatToOutput == GET_NBYTES) {
		outlet_anything(x->o_outlet, ps_OTTCP_nbytes, 2, arguments);
	} else if (x->o_whatToOutput == GET_DELIM) {
		outlet_anything(x->o_outlet, ps_OTTCP_delim, 2, arguments);
	} else {
		post("е OTUDP: error: o_whatToOutput is %ld in do_output", x->o_whatToOutput);
	}
	
	// Now that we finished outputting the buffer, we can reset all our state to be
	// ready for the next one
	oldLockout = AcquireLock(x);
	
	// Swap the double buffers
	if (x->o_currentReadBuf == x->o_ReadBufA) {
		x->o_currentReadBuf = x->o_ReadBufB;
		x->o_nextReadBuf = x->o_ReadBufA;
	} else {
		x->o_currentReadBuf = x->o_ReadBufA;
		x->o_nextReadBuf = x->o_ReadBufB;
	}
	
	x->o_bytesRead = x->o_bytesReadForNextTime;
	x->o_bytesReadForNextTime = 0;
	ChangeState(x, x->o_nextRequest);
	
	ReleaseLock(x,oldLockout);

	if (x->o_state == GET_NBYTES || x->o_state == GET_DELIM) {
		if (SeeIfWeCanAlreadyOutput(x)) return;
	}
	
	if (x->o_datawaiting) {
		// We know there's something in the TCP buffer that we haven't looked at yet.  Now's the time.
		ottcp_handleIncomingData(x);
	}
}

#define isprint(c) ((c) >= 0x20 && (c) <= 0x7e)

void printOutput(OTTCP *x) {
	unsigned char *m;
	char buf[100], *p;
	int n, i;

	post("OTTCP: about to output %ld bytes", x->o_bytesRead);

	m = (unsigned char *) x->o_currentReadBuf;
	n = x->o_bytesRead;
	
    for (i = 0; i < n; i += 4) {
    	p = buf;
    	
    	p += sprintf(p, "  %x", m[i]);
    	if (isprint(m[i])) {
    		p += sprintf(p, "  (%c)", m[i]);
    	} else {
    		p += sprintf(p, "  ()");
    	}
    	
    	if (i+1 < n) {
	    	p += sprintf(p, "  %x", m[i+1]);
	    	if (isprint(m[i+1])) {
	    		p += sprintf(p, "  (%c)", m[i+1]);
	    	} else {
	    		p += sprintf(p, "  ()");
	    	}
    	}
    	
     	if (i+2 < n) {
	    	p += sprintf(p, "  %x", m[i+2]);
	    	if (isprint(m[i+2])) {
	    		p += sprintf(p, "  (%c)", m[i+2]);
	    	} else {
	    		p += sprintf(p, "  ()");
	    	}
		}

    	if (i+3 < n) {
	    	p += sprintf(p, "  %x", m[i+3]);
	    	if (isprint(m[i+3])) {
	    		p += sprintf(p, "  (%c)", m[i+3]);
	    	} else {
	    		p += sprintf(p, "  ()");
	    	}
	    }
    	
    	*p = '\0';
    	post(buf);
	}


}	

void do_output_connected(OTTCP *x) {
	// This is called by the Max clock.  It's guaranteed not to be called
	// at notifier level, although if we're in Overdrive this will be called
	// at interrupt level.
	
	outlet_anything(x->o_outlet, x->o_connectionSymbol, 0, 0);
}


void ottcp_read_nbytes(OTTCP *x, long size) {
	EnterCallback();

	if (size <= 0) {
		post("е OTTCP: nbytes: n must be positive! (You said %ld)", size);
	} else if (size > x->o_ReadBufSize) {
		post("е OTTCP: nbytes: Can't read %ld bytes because read buffer is only %ld bytes",
			 size, x->o_ReadBufSize);
	} else {
		// Figure out if this request is OK for the state we're in
		if (x->o_state == SETTING_UP) {
			post("е OTTCP: Can't read n bytes; not connected");
		} else if (x->o_state == NO_REQUEST) {
			ChangeState(x, GET_NBYTES);
			x->o_numBytesWeWant = size;
			if (SeeIfWeCanAlreadyOutput(x)) {
				// Our N bytes were already in the input buffer, so now they're
				// scheduled to be output.
			} else if (x->o_datawaiting) {
				ottcp_handleIncomingData(x);
			}
		} else if (x->o_state == GET_NBYTES) {
			post("е OTTCP: still trying to read n bytes from before; ignoring new request");
		} else if (x->o_state == GET_DELIM) {
			post("е OTTCP: still trying to read to delimiter from before; ignoring new request");
		} else if (x->o_state == OUTPUTTING) {
			if (x->o_nextRequest == NO_REQUEST) {
				x->o_nextRequest = GET_NBYTES;
				x->o_numBytesWeWant = size;
			} else if (x->o_nextRequest == GET_NBYTES) {
				post("е OTTCP: still trying to read n bytes from before; ignoring new request");
			} else if (x->o_nextRequest == GET_DELIM) {
				post("е OTTCP: still trying to read to delimiter from before; ignoring new request");
			} else {
				post("е OTTCP: Unrecognized o_nextRequest state %ld.  Uh-oh.", x->o_state);
			}
		} else {
				post("е OTTCP: Unrecognized o_nextRequest state %ld.  Uh-oh.", x->o_state);
		}
	}
	ExitCallback();
}

void ottcp_read_until_delimiter_symbol(OTTCP *x, Symbol *delimiter) {
	EnterCallback();
	if (do_read_until_delimiter(x, delimiter->s_name, strlen(delimiter->s_name))) {
		x->o_needToFreeDelimiter = 0;
	}
	ExitCallback();
}

void ottcp_read_until_delimiter_bytes(OTTCP *x, Symbol *selector, short argc, Atom *argv) {
	int i;
	char *myBytes;
	EnterCallback();
	
	// First do error checking
	for (i = 0; i < argc; ++i) {
		if (argv[i].a_type != A_LONG) {
			post("е OTTCP: delim-bytes: all arguments must be ints; ignoring.");
			goto bag;
		}
		if (argv[i].a_w.w_long < 0 || argv[i].a_w.w_long >= 256) {
			post("е OTTCP: delim-bytes: argument %ld is not a valid byte value.", argv[i].a_w.w_long);
			goto bag;
		}
	}
	
	myBytes = getbytes(argc);
	if (myBytes == 0) {
		post("е OTTCP: delim-bytes: out of memory to store delimiter string; ignoring request.");
		goto bag;
	}
	
	for (i = 0; i < argc; ++i) {
		myBytes[i] = (unsigned char)  argv[i].a_w.w_long;
	}
	
	if (do_read_until_delimiter(x, myBytes, argc)) {
		x->o_needToFreeDelimiter = 1;
	} else {
		freebytes(myBytes, argc);
		x->o_needToFreeDelimiter = 0;
	}
bag:
	ExitCallback();
}


int do_read_until_delimiter(OTTCP *x, char *delimiterBytes, long delimiterLen) {
	if (delimiterLen > x->o_ReadBufSize) {
		post("е OTTCP: delim: Your delimiter is bigger than a receive buffer!");
	} else if (x->o_state == SETTING_UP) {
		post("е OTTCP: Can't read to a delimiter; not connected");
	} else if (x->o_state == NO_REQUEST) {
		ChangeState(x, GET_DELIM);
		if (x->o_needToFreeDelimiter) freebytes(x->o_delimiterBytes, x->o_delimiterLen);
		x->o_delimiterBytes = delimiterBytes;
		x->o_delimiterLen = delimiterLen;
		if (SeeIfWeCanAlreadyOutput(x)) {
			// the data we needed was already in the read buffer, so now it's scheduled for output
		} else if (x->o_datawaiting) {
			ottcp_handleIncomingData(x);
		}
		return 1;
	} else if (x->o_state == GET_NBYTES) {
		post("е OTTCP: still trying to read n bytes from before; ignoring new request");
	} else if (x->o_state == GET_DELIM) {
		post("е OTTCP: still trying to read to delimiter from before; ignoring new request");
	} else if (x->o_state == OUTPUTTING) {
		if (x->o_nextRequest == NO_REQUEST) {
			x->o_nextRequest = GET_DELIM;
			if (x->o_needToFreeDelimiter) freebytes(x->o_delimiterBytes, x->o_delimiterLen);
			x->o_delimiterBytes = delimiterBytes;
			x->o_delimiterLen = delimiterLen;
			return 1;
		} else if (x->o_nextRequest == GET_NBYTES) {
			post("е OTTCP: still trying to read n bytes from before; ignoring new request");
		} else if (x->o_nextRequest == GET_DELIM) {
			post("е OTTCP: still trying to read to delimiter from before; ignoring new request");
		} else {
			post("е OTTCP: Unrecognized o_nextRequest state %ld.  Uh-oh.", x->o_state);
		}
	} else {
		post("е OTTCP: Unrecognized o_nextRequest state %ld.  Uh-oh.", x->o_state);
	}
	return 0;
}

static void ottcp_connect(OTTCP *x, Symbol *hostName, long port) {
	OSStatus s;
	InetHost host;
	
	EnterCallback();
	
	if (LookUpInetHost(hostName->s_name, &host) == 0) {
		post("е OTTCP: can't understand host \"%s\"; not connecting",  hostName->s_name);
		return;
	}	

	x->o_inetHostName = hostName->s_name;
	x->o_inetPort = port;
	x->o_inetHost = host;
	
	change_connection(x);
	ExitCallback();
}

static void ottcp_disconnect(OTTCP *x) {
	EnterCallback();
	x->o_inetHostName = 0;
	x->o_inetPort = 0;
	x->o_inetHost = 0;
	change_connection(x);
	ExitCallback();
}


void ottcp_write(OTTCP *x, long size, long bufferPointer) {
	
	EnterCallback();

	if (size <= 0 ){
		post("е OTTCP: bad write length %ld", size);
		goto bag;
	}

	if (size > x->o_WriteBufSize) {
		post("е OTTCP: can't write %ld bytes because write buffer is only %ld bytes",
			 size, x->o_WriteBufSize);
		goto bag;
		// Strictly speaking, this isn't true, because we only use the write buffer
		// when flow control prevents us from passing the data directly to Open Transport
	}
	
	if (x->o_state == SETTING_UP) {
		if (x->o_tcp_ep == 0) {
			post("е ottcp: Can't send data yet; the endpoint hasn't been created yet.");
		} else {
			post("е otudp: Can't send data yet; the endpoint hasn't been bound yet.");
		}
		goto bag;
	}
	
	do_write(x, size, (void *) bufferPointer);	

bag:
	ExitCallback();
}

void do_write(OTTCP *x, long length, void *bytes) {
	OTResult r;
	
	if (x->o_WBReadPos == x->o_WBWritePos) {
		// Nothing in the circular buffer, so try to write the new stuff

		if (length == 0) return;
		r = OTSnd(x->o_tcp_ep, bytes, length, (OTFlags) 0);
		
		if (r == length) {
			// Great; everything was accepted
		} else if (r > 0) {
			// Partial flow control: some was accepted; the rest was not
			AddToWriteBuffer(x, length-r, ((void *) (((char *) bytes) + r)));
		} else if (r == kOTFlowErr) {
			// Full flow control situation: nothing was accepted
			AddToWriteBuffer(x, length, bytes);
		} else if (r == kOTLookErr) {
			// Maybe we got a disconnect or something
			OTResult lookr = OTLook(x->o_tcp_ep);
			post("е OTTCP: OTSnd returned kOTLookErr; OTLook returned %ld", lookr);
		} else {
			post("е OTTCP: OTSnd returned %ld", r);
		}
	} else {
		// We already have some flow-controlled bytes in the write buffer; they have to
		// go out before the new ones.  We'll try to write the old ones again, just in case 
		// they go out, and the new ones go out, without copying the new ones into the buffer
		
		if (x->o_WBReadPos > x->o_WBWritePos) {
			// Data in the buffer wraps around, so first we try to OTSnd the contiguous
			// part up to the end of the buffer
			
			r = OTSnd(x->o_tcp_ep, x->o_WriteBuf + x->o_WBReadPos, 
				      x->o_WriteBufSize - x->o_WBReadPos, (OTFlags) 0);
			
			if (r == length) {
				// Great; the latter part of the write buffer was accepted
				// Adjust the read position of the write buffer and make a recursive
				// call to try to write some more
				x->o_WBReadPos = 0;
				do_write(x, length, bytes);
			} else if (r > 0) {
				// Partial flow control: some was accepted; the rest was not
				x->o_WBReadPos += r;
				AddToWriteBuffer(x, length, bytes);
			} else if (r == kOTFlowErr) {
				// Full flow control situation: nothing was accepted
				AddToWriteBuffer(x, length, bytes);
			} else if (r == kOTLookErr) {
				// Maybe we got a disconnect or something
				OTResult lookr = OTLook(x->o_tcp_ep);
				post("е OTTCP: OTSnd returned kOTLookErr; OTLook returned %ld", lookr);
			} else {
				post("е OTTCP: OTSnd returned %ld", r);
			}
		} else {
			// Data in the write buffer is all in one contiguous chunk, so try writing it
			r = OTSnd(x->o_tcp_ep, x->o_WriteBuf+x->o_WBReadPos, 
				      x->o_WBWritePos - x->o_WBReadPos, (OTFlags) 0);
			
			if (r == length) {
				// Great; the entire write buffer was accepted.  Mark it empty
				// and make a recursive call to try to write the new stuff
				x->o_WBReadPos = x->o_WBWritePos = 0;
				do_write(x, length, bytes);
			} else if (r > 0) {
				// Partial flow control: some was accepted; the rest was not
				x->o_WBReadPos += r;
				AddToWriteBuffer(x, length, bytes);
			} else if (r == kOTFlowErr) {
				// Full flow control situation: nothing was accepted
				AddToWriteBuffer(x, length, bytes);
			} else if (r == kOTLookErr) {
				// Maybe we got a disconnect or something
				OTResult lookr = OTLook(x->o_tcp_ep);
				post("е OTTCP: OTSnd returned kOTLookErr; OTLook returned %ld", lookr);
			} else {
				post("е OTTCP: OTSnd returned %ld", r);
			}
		}
	}
}

void AddToWriteBuffer(OTTCP *x, long length, char *bytes) {
	int i, capacityLeft;
	
	capacityLeft = x->o_WBWritePos - x->o_WBReadPos;
	
	if (capacityLeft <= 0) capacityLeft += x->o_WriteBufSize;
	
	if (length > capacityLeft) {
		post("е OTTCP: error: Write buffer overflow.  Dropping data.  You probably want to reconnect.");
		return;
	}
	
	// Could use OTMemcpy here
	for (i = 0; i < length; ++i) {
		x->o_WriteBuf[x->o_WBWritePos] = bytes[i];
		++(x->o_WBWritePos);
		if (x->o_WBWritePos == x->o_WriteBufSize) x->o_WriteBufSize=0;
	}
}

/* I don't know why these aren't defined in some Max #include file. */
#define ASSIST_INLET 1
#define ASSIST_OUTLET 2

void ottcp_assist (OTTCP *x, void *box, long msg, long arg, char *dstString) {
	EnterCallback();
	
	if (msg==ASSIST_INLET) {
		sprintf(dstString, "nbytes, delim...");
	} else if (msg==ASSIST_OUTLET) {
		sprintf(dstString, "binary buffers");
	} else {
		post("е ottcp_assist: unrecognized message %ld", msg);
	}
	ExitCallback();
}

void ottcp_version (OTTCP *x) {
	EnterCallback();
	post("OTTCP (Open Transport TCP) Version " OTTCP_VERSION
		  ", by Matt Wright. Compiled " __TIME__ " " __DATE__);	
	ExitCallback();
}

void ottcp_ErrorReporting(OTTCP *x, long yesno) {
	EnterCallback();
	
	if (yesno == 0) {
		post("OTTCP: Turning off error reporting.  What you don't know can't hurt you.");
		x->o_errorreporting = 0;
	} else {
		post("OTTCP: Turning on error reporting.");
		x->o_errorreporting = 1;
	}
	ExitCallback();
}

void ottcp_tellmeeverything(OTTCP *x) {
	OTResult r;

	EnterCallback();
	post("ottcp_tellmeeverything: You asked for it");
	ottcp_version(x);
	
	post("  This object error does %sreport errors", x->o_errorreporting ? "" : "not ");
	post("  state: %s", StateName(x->o_state));
	if (x->o_state == OUTPUTTING) {
		post("  whatToOutput: %s", StateName(x->o_whatToOutput));
		post("  nextRequest: %s", StateName(x->o_nextRequest));
	} else if (x->o_state == GET_NBYTES) {
		post("  Getting %ld bytes.", x->o_numBytesWeWant);
	} else if (x->o_state == GET_DELIM) {
		post("  Looking for %ld-byte delimiterthat starts with \"%c\"", x->o_delimiterLen,
			 x->o_delimiterBytes[0]);
	}
	
	if (x->o_datawaiting) {
		post("  There is data waiting in TCP's buffer");
	} else {
		post("  There doesn't seem to be any data waiting in TCP's buffer");
	}
	
	if (x->o_tcp_ep == 0) {
		post("  This object has not created an endpoint yet.");
	} else {
		post("  This object's endpoint reference is %p", x->o_tcp_ep);
		post("    Endpoint info: addr %ld, options %ld, tsdu %ld, etsdu %ld, connect %ld",
		     x->epinfo.addr, x->epinfo.options, x->epinfo.tsdu, x->epinfo.etsdu, x->epinfo.connect);
		post("                   discon %ld, servtype %lu, flags %lu",
			 x->epinfo.discon, x->epinfo.servtype, x->epinfo.flags);
	}
	
	r = OTGetEndpointState(x->o_tcp_ep);

	if (r == T_UNINIT) {
		post("   Endpoint state: T_UNINIT");
	} else if (r == T_UNBND) {
		post("   Endpoint state: T_UNBND");
	} else if (r == T_IDLE) {
		post("   Endpoint state: T_IDLE");
	} else if (r == T_OUTCON) {
		post("   Endpoint state: T_OUTCON");
	} else if (r == T_INCON) {
		post("   Endpoint state: T_INCON");
	} else if (r == T_DATAXFER) {
		post("   Endpoint state: T_DATAXFER");
	} else if (r == T_OUTREL) {
		post("   Endpoint state: T_OUTREL");
	} else if (r == T_INREL) {
		post("   Endpoint state: T_INREL");
	} else {
		post("   еее Unrecognized endpoints state %ld!", r);
	}	

	
	{
		char hostnameBuf[100];
		post("  You asked to write to internet host \"%s\"", x->o_inetHostName);
		OTInetHostToString(x->o_inetHost, hostnameBuf);
		post("  Writing to internet host %s", hostnameBuf);
		post("  Writing to port %ld", (long)( x->o_inetPort));
	}
	
	post("  This object reads from port %ld", (long) x->o_receiveInetPort);


	post("  Read buffers: size %ld,  %ld bytes in current buffer, %ld bytes in next buffer",
		 x->o_ReadBufSize, x->o_bytesRead, x->o_bytesReadForNextTime);
	post("  Write buffer: size %ld, read position %ld, write position %ld",
		 x->o_WriteBufSize, x->o_WBReadPos, x->o_WBWritePos);

	ExitCallback();
}

long strlen (const char *s) {
	const char *p;
	
	for (p = s; *p != '\0'; ++p) {
		// do nothing
	}
	
	return p-s;
}

