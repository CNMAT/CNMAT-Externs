/*
Written by Matt Wright, The Center for New Music and Audio Technologies,
University of California, Berkeley.  Copyright (c) 1999,2000,01,02,03,04,
The Regents of the University of California (Regents).  

Permission to use, copy, modify, distribute, and distribute modified versions
of this software and its documentation without fee and without a signed
licensing agreement, is hereby granted, provided that the above copyright
notice, this paragraph and the following two paragraphs appear in all copies,
modifications, and distributions.

IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, ARISING
OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF REGENTS HAS
BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE. THE SOFTWARE AND ACCOMPANYING DOCUMENTATION, IF ANY, PROVIDED
HEREUNDER IS PROVIDED "AS IS". REGENTS HAS NO OBLIGATION TO PROVIDE
MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.


adapted from the OSC Kit, by Matt Wright*/

#include <OpenTptInternet.h>		// For struct InetAddress in the .h file
#include "packetbuffer-pool.h"
#include "ext.h"
#include <Memory.h>

/**************************************************
   Managing packet data structures
 **************************************************/

#define MIN_REASONABLE_RCV_BUFSIZE 128

PacketBuffer InitPackets(int receiveBufferSize, int numReceiveBuffers) {
    int i;
    PacketBuffer allPackets;

	// post("InitPackets %ld, %ld", (long) receiveBufferSize, (long) numReceiveBuffers);

    if (receiveBufferSize < MIN_REASONABLE_RCV_BUFSIZE) {
		ouchstring("otudp: InitPackets: receiveBufferSize of %d is unreasonably small.",
		    receiveBufferSize);
		return 0;
    }

    allPackets = (PacketBuffer) NewPtr(numReceiveBuffers * sizeof(*allPackets));
    if (allPackets == 0) return 0;

    for (i = 0; i < numReceiveBuffers; ++i) {
		allPackets[i].buf = (char*)NewPtr(receiveBufferSize);
		
		if (allPackets[i].buf == 0) return 0;

		allPackets[i].next = &(allPackets[i+1]);
	}

    allPackets[numReceiveBuffers-1].next = ((struct PacketBuffer_struct *) 0);

    return allPackets;
}

void DestroyPackets(PacketBuffer allPackets, int numReceiveBuffers) {
	int i;
	
	for (i = 0; i < numReceiveBuffers; ++i) {
		DisposePtr(allPackets[i].buf);
	}
	DisposePtr((char *) allPackets);
}
	
	
void PostPacketList(PacketBuffer l) {
	while (l != 0) {
		post("PacketBuffer %p (buf %p, size %ld, next %p)", l, l->buf, l->n, l->next);
		l = l->next;
	}
} 

PacketBuffer PacketBufferListPop(PacketBuffer *list) {
	/* This routine is not reentrant, and needs to be performed atomically */
    PacketBuffer result;
    
    if (*list == 0) {
		/* Could try to call the real-time memory allocator here */
		return 0;
    }

    result = *list;
    *list = result->next;

    return result;
}

void PacketBufferListPush(PacketBuffer p, PacketBuffer *list) {
	/* This routine is not reentrant, and needs to be performed atomically */

	if (p == 0) {
		ouchstring("PacketBufferListPush: tried to push a null packet!");
	} else {
	    p->next = *list;
    	*list = p;
    }
}

void InitPBFIFO(PacketBufferFIFO *f) {
	f->first = f->last = 0;
}

void PBFIFOEnqueue(PacketBuffer p, PacketBufferFIFO *f) {
	/* This routine is not reentrant, and needs to be performed atomically */
	
	p->next = 0;
	if (f->last ==  0)  {
		f->first = f->last = p;
	} else {
		f->last->next = p;
		f->last = p;
	}
}	
	
PacketBuffer PBFIFODequeue(PacketBufferFIFO *f) {
	if (f->first == 0) {
		return 0;
	} else {
		PacketBuffer firstinline = f->first;
		f->first = firstinline->next;
		if (f->first == 0) f->last = 0;
		return firstinline;
	}
}


