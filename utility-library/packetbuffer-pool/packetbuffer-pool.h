/* Copyright (c) 1999.  The Regents of the University of California (Regents).
All Rights Reserved.

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

adapted from the OSC Kit, by Matt Wright
	10/19/99 Matt added FIFO
	1/21/1 Matt added return address
	
*/

typedef struct PacketBuffer_struct {
    char *buf;			/* Contents of network packet go here */
    int n;				/* Overall size of packet */
    struct InetAddress returnAddress;
    struct PacketBuffer_struct *next;	/* For linked list of free packets */
} *PacketBuffer;


typedef struct PacketBufferFIF0_struct {
	struct PacketBuffer_struct *first;
	struct PacketBuffer_struct *last;
} PacketBufferFIFO;

/* procedure declatations */
PacketBuffer InitPackets(int receiveBufferSize, int numReceiveBuffers);
void DestroyPackets(PacketBuffer allPackets , int numReceiveBuffers);
PacketBuffer PacketBufferListPop(PacketBuffer *listp);
void PacketBufferListPush(PacketBuffer p, PacketBuffer *listp);
void PostPacketList(PacketBuffer l);

/* FIFO stuff */
void InitPBFIFO(PacketBufferFIFO *f);
void PBFIFOEnqueue(PacketBuffer p, PacketBufferFIFO *f);
PacketBuffer PBFIFODequeue(PacketBufferFIFO *f);

