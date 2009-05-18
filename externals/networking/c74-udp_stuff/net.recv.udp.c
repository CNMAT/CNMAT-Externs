//#include "ext.h" ext_syssock.h will take care of including necessary ext_h stuff
#include "ext_syssock.h"
#include "ext_systhread.h"
#include "ext_sysmem.h"
#include "ext_linklist.h"

#if TARGET_RT_MAC_MACHO
#include "ext_strings.h"
#else
#include "basic_c_strings.c"
#endif

#include <math.h>
#include <errno.h>
#include "ext_bytebuf.c"


#define true 1
#define false 0
/////////////BEGIN UDPRECEIVER

#define MAX_DGRAM_SIZE 65536 
#define DEFAULT_MAX_QUEUE_SIZE 2048

#define LOCK_QUEUE systhread_mutex_lock(x->queue_mutex);
#define UNLOCK_QUEUE systhread_mutex_unlock(x->queue_mutex);



typedef struct udpreceiver
{
    t_syssocket s;
    char* readbuf;//buffer we use to read into from the socket
    t_systhread listener;
    int     listening;
    t_sysaddr server,from;
	int port;
	t_linklist *queue;
	t_systhread_mutex queue_mutex;
    void *clock;
    int droppedpackets;
	int maxqueuesize;
	t_linklist *clients;
} t_udpreceiver;

typedef void (*t_udpreceiver_callback)(void *x, t_bytebuf *b);

typedef struct udpreceiver_client
{
	void* client;
	t_udpreceiver_callback callback;			
}t_udpreceiver_client;

//global
static t_linklist *s_active_udp_receivers = 0;
int udpreceiver_init();//initialize syssock?, create udp-receivers linklist
int udpreceiver_add_global_receiver(t_udpreceiver *x); 
void udpreceiver_remove_global_receiver(t_udpreceiver *x);
t_udpreceiver *udpreceiver_get_global_receiver(int port);

//internal


int udpreceiver_netset(t_udpreceiver *x);
void udpreceiver_listen(t_udpreceiver *x);
void udpreceiver_enqueue_dgram(t_udpreceiver *x, t_bytebuf *bb);
t_bytebuf* udpreceiver_dequeue_dgram(t_udpreceiver *x);
void udpreceiver_deferdroppedpacketpost(t_udpreceiver *x,t_symbol *msg, short argc, t_atom *argv);
void udpreceiver_clock_tick(t_udpreceiver *x);
void udpreceiver_cleanlinklist(t_udpreceiver *x);
void udpreceiver_shutdown(t_udpreceiver *x);
void udpreceiver_free(t_udpreceiver *x);

//public
t_udpreceiver* udpreceiver_new(int port);
int udpreceiver_addclient(t_udpreceiver *x,void *client, t_udpreceiver_callback callback);
int udpreceiver_removeclient(t_udpreceiver *x,void *client);//if clients is 0 free
void udpreceiver_maxqueuesize(t_udpreceiver *x,int maxqueuesize);

//if no port exists create a new udp_receiver at specified port and start it up
//otherwise just return t_udpreceiver already bound to port.

int udpreceiver_start(t_udpreceiver *x,void *client,t_udpreceiver_callback callback);//add to clients
int udpreceiver_stop(t_udpreceiver *x,void *client);//remove from clients

int udpreceiver_setport();//figure out later do we need this???
int udpreceiver_setmaxqueuesize();//figure out later do we need this???


int udpreceiver_init()
{
	if(!s_active_udp_receivers)
	{
		s_active_udp_receivers = (t_linklist *)linklist_new();	
	}
	syssock_init();	
	return 0;
}

int udpreceiver_add_global_receiver(t_udpreceiver *x)
{
	//post("adding global recveicer on port %d ",x->port);
	return linklist_append(s_active_udp_receivers,x);		
}

void udpreceiver_remove_global_receiver(t_udpreceiver *x)
{
	int i;
	long size = linklist_getsize(s_active_udp_receivers);
	for(i = 0; i < size;i++)
	{
		if(((t_udpreceiver*)linklist_getindex(s_active_udp_receivers,i)) == x)
		{
			linklist_chuckindex(s_active_udp_receivers,i);	
			//free the receiver when it is no longer in the global list
			//post("freeing global receiver on port %d",x->port);
			udpreceiver_free(x);	
			break;
		}
			
	}
}

t_udpreceiver *udpreceiver_get_global_receiver(int port)
{
	t_udpreceiver *x;
	int i;
	long size = linklist_getsize(s_active_udp_receivers);
	for(i = 0; i < size;i++)
	{
		x = (t_udpreceiver*)linklist_getindex(s_active_udp_receivers,i);
		if(x->port == port)
			return x;
	}
		return 0;
}

int udpreceiver_addclient(t_udpreceiver *x,void *client, t_udpreceiver_callback callback)
{
	t_udpreceiver_client *c = (t_udpreceiver_client*)getbytes(sizeof(t_udpreceiver_client));
	c->client = client;
	c->callback = callback;
	//post("adding client for port %d",x->port);
	return linklist_append(x->clients,c);	
}

int udpreceiver_removeclient(t_udpreceiver *x,void *client)//if clients is zero free the whole enchilada
{
	int i;
	t_udpreceiver_client *c;
	long size = linklist_getsize(x->clients);
	for(i = 0; i < size;i++)
	{
		c = (t_udpreceiver_client*)linklist_getindex(x->clients,i);
		if(c->client == client)
		{
			//get rid of the client from the list
			linklist_chuckindex(x->clients,i);
			freebytes(c,sizeof(t_udpreceiver_client));
			break;
		}
	}
	
	//if we have no more clients close down this connection
	size = linklist_getsize(x->clients);
	if(size == 0)
	{
		udpreceiver_remove_global_receiver(x);
	}
	return 0;
}

t_udpreceiver* udpreceiver_new(int port)
{
	t_udpreceiver *x = udpreceiver_get_global_receiver(port);
	if(x)
		return x;
	
	x = (t_udpreceiver *)getbytes(sizeof(t_udpreceiver));
	if(systhread_mutex_new(&(x->queue_mutex), 0) != 0)
	{
		object_error((t_object *)x, "could not create queue mutex. uh oh.");
		freebytes(x, sizeof(t_udpreceiver));
		return NULL;
	} 
	
	x->port  = port;
	x->s     = (t_syssocket)NULL;
	x->queue = (t_linklist *)linklist_new();
	x->listening = false;
	x->clock =  clock_new(x,(method)udpreceiver_clock_tick);
	x->droppedpackets = 0;
	x->readbuf = sysmem_newptr(MAX_DGRAM_SIZE);
	x->maxqueuesize = DEFAULT_MAX_QUEUE_SIZE;
	x->clients = (t_linklist *)linklist_new();
	udpreceiver_netset(x);//side effect of starting listener thread		
	udpreceiver_add_global_receiver(x);
	return(x);
			
}


int udpreceiver_netset(t_udpreceiver *x)
{
	int length;
	
	length = sizeof(x->server);
	
	setmem(&(x->server), length, 0);
	
	syssock_setfamily(&(x->server), AF_INET);
	syssock_setip(&(x->server), syssock_htonl(INADDR_ANY));
	syssock_setport(&(x->server), syssock_htons(x->port));   
	
	//jit_net_recv_stop(x);
	if ((x->s = syssock_socket(AF_INET, SOCK_DGRAM, 0)) == SYSSOCK_INVALID_SOCKET) {
        object_error((t_object *)x, "socket method unsuccessful");
        return -1;
    }
	    
	//this option allows the port to be released right away when we close it
    syssock_reuseaddr(x->s);   

	object_post((t_object *)x, "binding to port %d",x->port);
	if (syssock_bind(x->s, &(x->server)) == SYSSOCK_BIND_ERROR) {
        object_error((t_object *)x, "bind to port %d unsuccessful.",x->port);
		object_post((t_object *)x, "there is probably something already bound to this port.");
        return -2;
    }
    
    x->droppedpackets = 0;
	x->listening = true;
	systhread_create((method)udpreceiver_listen,x,0,0,0,&(x->listener));
    
    return 1;
}

void udpreceiver_listen(t_udpreceiver *x)
{
	int fromlen = sizeof(x->from);
	int n;
	t_bytebuf *bb;
	
	while(x->listening)
	{
		n = syssock_recvfrom(x->s,x->readbuf,MAX_DGRAM_SIZE,0,(struct sockaddr *)&(x->from),&fromlen);
		if(n > 0)
		{
			bb = bytebuf_new(n);
			bytebuf_copyto(bb,x->readbuf,n);
			udpreceiver_enqueue_dgram(x,bb);
		}
		//post("read %d bytes",n);
	}	
}

void udpreceiver_shutdown(t_udpreceiver *x)	
{   
    if (x->listener) 
    {   
		x->listening = false;
		if (x->s)
			syssock_close(x->s);	
		systhread_join(x->listener,NULL);  	//wait for the thread to stop
		x->listener = NULL;
	}	
}

void udpreceiver_maxqueuesize(t_udpreceiver *x,int maxqueuesize)
{
	x->maxqueuesize = maxqueuesize;	
}

void udpreceiver_enqueue_dgram(t_udpreceiver *x, t_bytebuf *bb)
{
	LOCK_QUEUE
	if(linklist_getsize(x->queue) > x->maxqueuesize)
	{
		x->droppedpackets++;
		//when we obex this thing we should be able to report the port from
		//deferdroppedpacketpost. It seems like deferlow wants a valid object
		//as the first argument if non null...
		defer_low(0,(method)udpreceiver_deferdroppedpacketpost,(t_symbol *)x,0,0);		
		bytebuf_free(bb);
		UNLOCK_QUEUE
		return;
	}	
	else
	{	
		linklist_append(x->queue,bb);	
	}
	UNLOCK_QUEUE
	clock_delay(x->clock,0);
}

t_bytebuf* udpreceiver_dequeue_dgram(t_udpreceiver *x)
{
	t_bytebuf *bb=NULL;
	LOCK_QUEUE
	if(linklist_getsize(x->queue)>0)
	{
		bb = (t_bytebuf *)linklist_getindex(x->queue,0);
		linklist_chuckindex(x->queue,0);		
	}
	UNLOCK_QUEUE
	return bb;
}

void udpreceiver_deferdroppedpacketpost(t_udpreceiver *x,t_symbol *msg, short argc, t_atom *argv)
{
	t_udpreceiver *xx = (t_udpreceiver *)msg;
	object_post((t_object *)x, "[port %d]: Dropped packet. Total dropped packets:%d",xx->port,xx->droppedpackets);
}


void udpreceiver_clock_tick(t_udpreceiver *x)
{
	t_bytebuf *bb;
	int i;
	t_udpreceiver_client *c;
	int numclients = linklist_getsize(x->clients);
	while((bb = udpreceiver_dequeue_dgram(x)) != NULL)
	{
		for(i = 0;i < numclients;i++)
		{
			c = (t_udpreceiver_client *)linklist_getindex(x->clients,i);
			(*(c->callback))(c->client,bb);
		}
		bytebuf_free(bb);
	}
}
//do we need to lock the queue here?
void udpreceiver_cleanlinklist(t_udpreceiver *x)
{
	t_bytebuf *bb;
	while((bb = udpreceiver_dequeue_dgram(x)) != NULL)
	{
		bytebuf_free(bb);			
	}
	
}

void udpreceiver_free(t_udpreceiver *x)
{
	//probaby should clean the queue unless linklist free just takes care of it which it probably does
	udpreceiver_shutdown(x);	
	if(x->queue)
	{
		udpreceiver_cleanlinklist(x);	
		freeobject((t_object *)x->queue);
	}	
	if(x->clock)
	{
		clock_unset(x->clock);
		freeobject(x->clock);
	}
	if(x->queue_mutex)
		systhread_mutex_free(x->queue_mutex);
	
	sysmem_freeptr(x->readbuf);
}



///////////////END UDPRECEIVER

#define UDP_DEFAULT_PORT 7374


typedef struct udprecv
{
    t_object x_ob;
	int port;
	int cnmatcompat; // are we outletting fullpacket messages or not?
	t_udpreceiver *recv;
	void *o_outlet;
} t_udprecv;


void *udprecv_class;

void *udprecv_new(Symbol *s, short argc, Atom *argv);
void udprecv_free(t_udprecv *x);
void udprecv_assist(t_udprecv *x, void *b, long m, long a, char *s);
void udprecv_init();
void udprecv_setport(t_udprecv *x,long port);
void udprecv_setmaxqueuesize(t_udprecv *x,int maxqueuesize);
void udprecv_callback(void *x,t_bytebuf *bb);

//symbols
static t_symbol *sym_fullpacket = NULL;


//OSC SUPPORT
char *htm_error_string;	// Used for error messages
void osc_parse_osc_packet(t_udprecv *x, char *buf, long n);
char *osc_data_after_aligned_string(char *string, char *boundary); 
int osc_is_nice_string(char *string, char *boundary);
static void osc_2max_message(t_udprecv *x, char *address, void *v, long n); 
int osc_effective_string_length(char *string);

#if TARGET_RT_MAC_MACHO
int main(void)
#else
void main(void)
#endif
{
	setup((t_messlist **) &udprecv_class, (method)udprecv_new, (method)udprecv_free, (short)sizeof(t_udprecv), 0L, A_GIMME, 0); 
    addmess((method)udprecv_assist,"assist",A_CANT,0);
    addmess((method)udprecv_setport,"port",A_LONG,0);
    addmess((method)udprecv_setmaxqueuesize,"maxqueuesize",A_LONG,0);
	udprecv_init();
	//init our udpreceiver class as well
	udpreceiver_init();
	#if TARGET_RT_MAC_MACHO
	return 0;
	#endif
}

void udprecv_init()
{
	sym_fullpacket = gensym("FullPacket");
	//-syssock_init();
}


void udprecv_setport(t_udprecv *x,long port)
{

	if(port > 0)
	{
		x->port = port;
		udpreceiver_removeclient(x->recv,x);
		x->recv  = udpreceiver_new(x->port);
		udpreceiver_addclient(x->recv,x,udprecv_callback);	
	}

}

void udprecv_setmaxqueuesize(t_udprecv *x,int maxqueuesize)
{

	if(maxqueuesize > 0 )
	{
		udpreceiver_maxqueuesize(x->recv,maxqueuesize);
	}

}

void udprecv_assist(t_udprecv *x, void *b, long m, long a, char *s)
{
	// this system eliminates the need for a STR# resource
	if (m==1) {
		switch (a) 
		{
			case 0: 
				sprintf(s,"messages in"); 
				break;
		}
	} 
	else if (m==2) {
		switch (a) 
		{
			case 0: 
				sprintf(s,"data out"); 
				break;
		}
	}
}

/*
void udprecv_stop(t_udprecv *x)	
{   
    if (x->listener) 
    {   
		x->listening = false;
		if (x->s)
			syssock_close(x->s);	
		systhread_join(x->listener,NULL);  	//wait for the thread to stop
		x->listener = NULL;
	}	
}
*/


void udprecv_callback(void *x,t_bytebuf *bb)
{

	t_atom aout[2];	
			
	if(((t_udprecv *)x)->cnmatcompat)
	{
		/*full packet mode*/
		SETLONG(&aout[0], bb->size);
		SETLONG(&aout[1],(long)(bb->buf));
		outlet_anything(((t_udprecv *)x)->o_outlet,sym_fullpacket,2,&(aout[0]));
	}
	else
	{
		osc_parse_osc_packet((t_udprecv *)x,bb->buf,bb->size);	
	}

}


void *udprecv_new(Symbol *s, short argc, Atom *argv)
{
	t_udprecv *x;

	x = (t_udprecv *)newobject(udprecv_class);	
	x->o_outlet = outlet_new(x,0L);	
	
	if (argc == 0)
	{
		x->port = UDP_DEFAULT_PORT;	
		x->cnmatcompat = 0;
	}
	else if(argc >= 1 && argv[0].a_type == A_LONG)
	{
		x->port = argv[0].a_w.w_long;
		x->cnmatcompat = 0;
		if(argc >1)
			x->cnmatcompat = 1;
	}
	x->recv  = udpreceiver_new(x->port);
	udpreceiver_addclient(x->recv,x,udprecv_callback);
	return(x);
}

void udprecv_free(t_udprecv *x)
{
	//probaby should clean the queue unless linklist free just takes care of it which it probably does
	udpreceiver_removeclient(x->recv,x);	
}



//OSC RECEVIER SUPPORT CRIBBED FORM MATT........................
void osc_parse_osc_packet(t_udprecv *x, char *buf, long n) {
    long size, messageLen, i;
    char *messageName;
    char *args;


    if ((n%4) != 0) {
			object_post((t_object *)x, "OSC packet size (%d) not a multiple of 4 bytes: dropping",n);
		return;
    }

    if ((n >= 8) && (strncmp(buf, "#bundle", 8) == 0)) {
		/* This is a bundle message. */
		if (n < 16) {
			object_post((t_object *)x, "OSC Bundle message too small (%d bytes) for time tag", n);
		    return;
		}

		i = 16; /* Skip "#bundle\0" and time tag */
		while(i<n) {
	  	  size = syssock_ntohl(*((long *) (buf + i)));
	  	  if ((size % 4) != 0) {
				object_post((t_object *)x, "Bad size count %d in OSC bundle (not a multiple of 4)", size);
			return;
	 	   }
	   	  if ((size + i + 4) > n) {
			 object_post((t_object *)x, "Bad size count %d in OSC bundle (only %d bytes left in entire bundle)",size, n-i-4);
			 return;	
	      }
	    
	      /* Recursively handle element of bundle */
	      osc_parse_osc_packet(x, buf+i+4, size);
	      i += 4 + size;
	    }
		if (i != n) {
	  	  object_post((t_object *)x, "OTUDP: This can't happen");
		}
    } else {
		/* This is not a bundle message */

		messageName = buf;
		args = osc_data_after_aligned_string(messageName, buf+n);
		if (args == 0) {
		   	object_post((t_object *)x, "OSC Bad message name string: %s\nDropping entire message.\n",htm_error_string);
	   	 	return;
		}
		
		messageLen = args-messageName;	    
		osc_2max_message(x, messageName, (void *)args, n-messageLen);
    }

}

#define SMALLEST_POSITIVE_FLOAT 0.000001f
#define MAXARGS 1024

static void osc_2max_message(t_udprecv *x, char *address, void *v, long n) {

    float *floats;
    long *ints;
    char *chars;
    char *typeTags, *thisType, *p;
    t_symbol *addressSymbol;
	t_atom args[MAXARGS];
	int numArgs = 0;
	
	addressSymbol = gensym(address);

    /* Go through the arguments a word at a time */
    floats = v;
    ints = v;
    chars = v;
	

	if (chars[0] == ',' && osc_is_nice_string(chars, chars+n)) {
		/* Interpret the first string argument as a type string */
				
		typeTags = chars;
		p = osc_data_after_aligned_string(chars, chars+n);
		

  		 for (thisType = typeTags+1; *thisType != 0; ++thisType) {
  		   
	       switch (*thisType) {
	            case 'i': case 'r': case 'm': case 'c':
	            SETLONG(&args[numArgs], syssock_ntohl(*((int *) p)));
				p += 4;
	            break;

	            case 'f':
				{
					long fv;
					float *fp = (float *)&fv;				
					fv =  syssock_ntohl(*((long *) p));
					SETFLOAT(&args[numArgs],*fp);
				}
				p += 4;
	            break;
	            
	            case 'h': case 't':
	            /* 64-bit int: interpret as zero since Max doesn't have long ints */
	            /* Could see if the data fits in a 32-bit int and output it like that if so... */
	            SETLONG(&args[numArgs], 0);
	            p += 8;
	            break;

	            case 'd':
	            /* 64-bit float: interpret as zero since Max doesn't have doubles */
	            /* Could see if the data fits in a 32-bit float and output it like that if so... */
	            SETFLOAT(&args[numArgs], 0.0);
	            p += 8;
	            break;

	            case 's': case 'S':
	            if (!osc_is_nice_string(p, typeTags+n)) {
	            	SETSYM(&args[numArgs], gensym("¥Bogus_String"));
	            } else {
	            	SETSYM(&args[numArgs], gensym(p));
	                p = osc_data_after_aligned_string(p, typeTags+n);
	            }
	            break;

	            case 'T': 
	            /* "True" value comes out as the int 1 */
	           	SETLONG(&args[numArgs], 1);
	           	/* Don't touch p */
	           	break;
	           	
	            case 'F': 
	            /* "False" value comes out as the int 0 */
	           	SETLONG(&args[numArgs], 0);
	           	/* Don't touch p */
	           	break;
	           	            
	            case 'N': 
	            /* Empty lists in max?  I wish!  How about the symbol "nil"? */
	            SETSYM(&args[numArgs], gensym("nil"));
	            /* Don't touch p */
	           	break;
	           	
	            case 'I': 
	            /* Empty lists in max?  Ha!  How about the symbol "nil"? */
	            SETSYM(&args[numArgs], gensym("Infinitum"));
	            /* Don't touch p */
	           	break;


	            default:
		            object_post((t_object *)x, "OSC Unrecognized type tag %c. Unable to process message.", *thisType);
	            	return;
	       }
	        ++numArgs;
	     
  		 }
			outlet_anything(x->o_outlet, addressSymbol, numArgs, args);
	}
	else
	{
		object_post((t_object *)x, "OSC expected type string. Dropping message for address %s",address);			
	}
	
}
#define STRING_ALIGN_PAD 4

char *osc_data_after_aligned_string(char *string, char *boundary) 
{
    /* The argument is a block of data beginning with a string.  The
       string has (presumably) been padded with extra null characters
       so that the overall length is a multiple of STRING_ALIGN_PAD
       bytes.  Return a pointer to the next byte after the null
       byte(s).  The boundary argument points to the character after
       the last valid character in the buffer---if the string hasn't
       ended by there, something's wrong.

       If the data looks wrong, return 0, and set htm_error_string */

    int i;

    if ((boundary - string) %4 != 0) {
		ouchstring("net.recv.udp: OSC Internal error: DataAfterAlignedString: bad boundary\n");
		return 0;
    }

    for (i = 0; string[i] != '\0'; i++) {
		if (string + i >= boundary) {
		    htm_error_string = "DataAfterAlignedString: Unreasonably long string";
		    return 0;
		}
    }

    /* Now string[i] is the first null character */
    i++;

    for (; (i % STRING_ALIGN_PAD) != 0; i++) {
		if (string + i >= boundary) {
		    htm_error_string = "DataAfterAlignedString: Unreasonably long string";
		    return 0;
		}
		if (string[i] != '\0') {
		    htm_error_string = "DataAfterAlignedString: Incorrectly padded string.";
		    return 0;
		}
    }

    return string+i;
}

#if !TARGET_RT_MAC_MACHO
#define isprint(c) ((c) >= 0x20 && (c) <= 0x7e)
#endif
int osc_is_nice_string(char *string, char *boundary)  {
    /* Arguments same as DataAfterAlignedString().  Is the given "string"
       really a string?  I.e., is it a sequence of isprint() characters
       terminated with 1-4 null characters to align on a 4-byte boundary? */

    int i;

    if ((boundary - string) %4 != 0) {
		ouchstring("net.recv.udp: OSC Internal error: IsNiceString: bad boundary\n");
		return 0;
    }

    for (i = 0; string[i] != '\0'; i++) {
		if (!isprint(string[i])) return FALSE;
		if (string + i >= boundary) return FALSE;
    }

    /* If we made it this far, it's a null-terminated sequence of printing characters 
       in the given boundary.  Now we just make sure it's null padded... */

    /* Now string[i] is the first null character */
    i++;
    for (; (i % STRING_ALIGN_PAD) != 0; i++) {
		if (string[i] != '\0') return FALSE;
    }

    return TRUE;
}

int osc_effective_string_length(char *string) {
    int len = strlen(string) + 1;  /* We need space for the null char. */
     
     /* Round up len to next multiple of STRING_ALIGN_PAD to account for alignment padding */
     if ((len % STRING_ALIGN_PAD) != 0) {
         len += STRING_ALIGN_PAD - (len % STRING_ALIGN_PAD);
     }
     return len;
 }

