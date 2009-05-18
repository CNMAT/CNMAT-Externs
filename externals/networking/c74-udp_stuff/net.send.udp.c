//#include "ext.h" ext_syssock.h will take care of including necessary ext_h stuff
#include "ext_syssock.h"
#include "ext_systhread.h"
#include "ext_sysmem.h"
#include "ext_linklist.h"

#if !TARGET_RT_MAC_MACHO
#include "basic_c_strings.c"
#else
#include "ext_strings.h"
#endif

#include <math.h>
#include <errno.h>

#include "ext_bytebuf.c"

#define UDP_SEND_DEFAULT_PORT 7374
#define UDP_SEND_DEFAULT_IP 127.0.0.1
#define MAX_DGRAM_SIZE 65536 
#define DEFAULT_MAX_QUEUE_SIZE 512 
#define DEFAULT_MAX_PACKETSIZE 5096
#define true 1
#define false 0
typedef struct udpsend
{
    t_object x_ob;
    t_syssocket s;
    t_systhread sender;
    int    sending;
    t_sysaddr	server;
	int port;
	t_symbol*   ip;
	t_linklist *queue;
	t_systhread_mutex queue_mutex;
    t_systhread_cond  cond_empty;
    int droppedpackets;
	int maxqueuesize;
	int maxpacketsize;
} t_udpsend;


#define LOCK_QUEUE    systhread_mutex_lock(x->queue_mutex);
#define UNLOCK_QUEUE  systhread_mutex_unlock(x->queue_mutex);

void *udpsend_class;

void *udpsend_new(Symbol *s, short argc, Atom *argv);
void udpsend_free(t_udpsend *x);
void udpsend_bang(t_udpsend *x);
void udpsend_int(t_udpsend *x,long i);
void udpsend_float(t_udpsend *x,double f);
void udpsend_anything(t_udpsend *x, Symbol *msg, short argc, t_atom *argv);
void udpsend_assist(t_udpsend *x, void *b, long m, long a, char *s);
void udpsend_init();
int  udpsend_netset(t_udpsend *x);
void udpsend_stop(t_udpsend *x);
void udpsend_send(t_udpsend *x);
void udpsend_clock_tick(t_udpsend *x);
void udpsend_fullpacket(t_udpsend *x, long size, long pbuf);
void udpsend_setport(t_udpsend *x,long port);
void udpsend_sethost(t_udpsend *x,t_symbol *host);
void udpsend_setmaxqueuesize(t_udpsend *x,int maxqueuesize);
void udpsend_setmaxpacketsize(t_udpsend *x,int maxpacketsize);
void udpsend_enqueue_dgram(t_udpsend *x, t_bytebuf *bb);
t_bytebuf* udpsend_dequeue_dgram(t_udpsend *x);
void udpsend_deferdroppedpacketpost(t_udpsend *x,t_symbol *msg, short argc, t_atom *argv);
void udpsend_cleanlinklist(t_udpsend *x);

//OSC sending support
#define MAX_ARGS_TO_OSC_MSG 1024
void osc_enqueue_max_mess(t_udpsend *x, char *messageName, short argc, Atom *argv,int packet_size);
int osc_effective_string_length(char *string);
int osc_write_float_arg(char *buf, float arg);
int osc_write_int_arg(char *buf, int arg);
int osc_write_string_arg(char *buf, char *arg);
static int osc_pad_string(char *dest, char *str);
static int osc_write_padding(char *dest, int i); 
void osc_packet_print_contents(t_bytebuf *bb); 

//symbols
static t_symbol *ps_fullpacket = NULL;
#if TARGET_RT_MAC_MACHO
int main(void)
#else
void main(void)
#endif
{
	setup((t_messlist **) &udpsend_class, (method)udpsend_new, (method)udpsend_free, (short)sizeof(t_udpsend), 0L, A_GIMME, 0); 

	addbang((method)udpsend_bang);
	addint((method)udpsend_int);
	addfloat((method)udpsend_float);
	addmess((method)udpsend_anything,"list", A_GIMME, 0);
	addmess((method)udpsend_anything,"anything", A_GIMME, 0);    	
	addmess((method)udpsend_assist,"assist",A_CANT,0);
	addmess((method)udpsend_setport,"port",A_LONG,0);
	addmess((method)udpsend_sethost,"host",A_SYM,0);
	addmess((method)udpsend_fullpacket,"FullPacket",A_LONG,A_LONG,0);
	addmess((method)udpsend_setmaxqueuesize,"maxqueuesize",A_LONG,0);
	addmess((method)udpsend_setmaxqueuesize,"maxpacketsize",A_LONG,0);
	udpsend_init();
	
	#if TARGET_RT_MAC_MACHO
	return 0;
	#endif
}

void udpsend_init()
{
	ps_fullpacket = gensym("FullPacket");
	syssock_init();
}

void udpsend_bang(t_udpsend *x)
{
	osc_enqueue_max_mess(x,"bang",0, 0,12);	
}

void udpsend_int(t_udpsend *x,long i)
{
	t_atom av;
	SETLONG(&av,i);
	osc_enqueue_max_mess(x,"int",1, &av,12);	
}

void udpsend_float(t_udpsend *x,double f)
{
	t_atom av;
	SETFLOAT(&av,f);
	osc_enqueue_max_mess(x,"float",1,&av,16);	
}

void udpsend_anything(t_udpsend *x, Symbol *msg, short ac, t_atom *av)
{
	osc_enqueue_max_mess(x,msg->s_name,ac,av,x->maxpacketsize);
}

void udpsend_fullpacket(t_udpsend *x,long size, long p_buf)
{
	t_bytebuf *bb = bytebuf_new(size);
	bytebuf_copyto(bb,(char *)p_buf,size);
	udpsend_enqueue_dgram(x, bb);
}

void udpsend_setport(t_udpsend *x,long port)
{
	if(port > 0)
	{
		x->port = port;
		udpsend_stop(x);
		udpsend_netset(x);	
	}
}

void udpsend_sethost(t_udpsend *x,t_symbol *host)
{
	if(host)
	{
		x->ip = host;
		udpsend_stop(x);
		udpsend_netset(x);
	}
}

void udpsend_setmaxqueuesize(t_udpsend *x,int maxqueuesize)
{
	//could we get a deadlock here ??
	if(maxqueuesize > 0 )
	{
		x->maxqueuesize = maxqueuesize;
	}
}


void udpsend_setmaxpacketsize(t_udpsend *x,int maxpacketsize)
{
	if(maxpacketsize > 12)
	{
		x->maxpacketsize = maxpacketsize;	
	}
	
}

void udpsend_assist(t_udpsend *x, void *b, long m, long a, char *s)
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
				sprintf(s,"info out"); 
				break;
		}
	}
}


void udpsend_stop(t_udpsend *x)	
{   
    if (x->sender) 
    {   
		x->sending = false;
		
		LOCK_QUEUE
		systhread_cond_signal(x->cond_empty);//signal thread if it is waiting so we can join it and die	
		UNLOCK_QUEUE
		
		systhread_join(x->sender,NULL);  	//wait for the thread to stop
		x->sender = NULL;
	}	
}


int udpsend_netset(t_udpsend *x)
{
	int length;
	struct hostent *hp;
	int yes = 1;

	if ((x->s = syssock_socket(AF_INET, SOCK_DGRAM, 0)) == SYSSOCK_INVALID_SOCKET) {
        object_error((t_object *)x, "socket method unsuccessful");
        return -1;
    }

	length = sizeof(x->server);	
	setmem(&(x->server), length, 0);
	syssock_setfamily(&(x->server), AF_INET);

	//do hostname lookup
	hp = syssock_gethostbyname(x->ip->s_name);
	if(hp == 0)
	{
		object_error((t_object *)x, "unable to resolve host %s.",x->ip->s_name);
		return -3;
	}
	sysmem_copyptr((const void *)hp->h_addr,(void *)&(x->server.sin_addr), hp->h_length);
  	//strncpy((char *)&(x->server.sin_addr),(const char *)hp->h_addr, hp->h_length);     
  	syssock_setport(&(x->server), syssock_htons(x->port));  
  	
  	 syssock_setsockopt(x->s,SOL_SOCKET,SO_BROADCAST,(void *)&yes, sizeof(int));
    //post("udp.send: ready to send to host %s on port %d.",hp->h_name,x->port);

    x->droppedpackets = 0;
	x->sending = true;
	systhread_create((method)udpsend_send,x,0,0,0,&(x->sender));
	return 0;
}

void udpsend_send(t_udpsend *x)
{
	
	int n;
	t_bytebuf *bb;
	
	while (x->sending) {
		bb = udpsend_dequeue_dgram(x); //thread could wait in here	
		if(bb)//we will get a NULL when we stop the and the sender was waiting
		{
			n = syssock_sendto(x->s,bb->buf,bb->size,0,(struct sockaddr*)&(x->server),sizeof(struct sockaddr_in));
			//if (n < 0)
			//	error("udp.send: error sending bytebuf");
			//else
				//post("sent %d bytes",n);
			
			bytebuf_free(bb);
		}
	}
	systhread_exit(0);
}

void udpsend_deferdroppedpacketpost(t_udpsend *x,t_symbol *msg, short argc, t_atom *argv)
{
	object_post((t_object *)x, "Dropped packet. Total dropped packets:%d",(int)msg);
}

void udpsend_enqueue_dgram(t_udpsend *x, t_bytebuf *bb)
{
	LOCK_QUEUE
	if(linklist_getsize(x->queue) > x->maxqueuesize)
	{
		x->droppedpackets++;
		defer_low(x,(method)udpsend_deferdroppedpacketpost,(t_symbol *)x->droppedpackets,0,0);		
		bytebuf_free(bb);
		UNLOCK_QUEUE
		return;
	}	
	else
	{	
		linklist_append(x->queue,bb);		
		systhread_cond_signal(x->cond_empty);									
		UNLOCK_QUEUE
	}
	
}

t_bytebuf* udpsend_dequeue_dgram(t_udpsend *x)
{
	t_bytebuf *bb=NULL;

	LOCK_QUEUE
	while(linklist_getsize(x->queue) == 0 && x->sending) {
		systhread_cond_wait(x->cond_empty, x->queue_mutex);	
	}
	if (x->sending) {
		bb = (t_bytebuf *)linklist_getindex(x->queue,0);
		linklist_chuckindex(x->queue,0);
	}
	UNLOCK_QUEUE
	return bb;
}

void udpsend_cleanlinklist(t_udpsend *x)
{
	int size;
	t_bytebuf *bb;
	
	LOCK_QUEUE
	size = linklist_getsize(x->queue);
	while (size--) {
		bb = (t_bytebuf *)linklist_getindex(x->queue,0);
		linklist_chuckindex(x->queue,0);
		bytebuf_free(bb);
	}
	UNLOCK_QUEUE
}

void *udpsend_new(Symbol *s, short argc, Atom *argv)
{
	t_udpsend *x;

	if (x = (t_udpsend *)newobject(udpsend_class)) {
		x->sender = NULL;
		x->cond_empty = NULL;
		x->s     = (t_syssocket)NULL;
		x->queue = (t_linklist *)NULL;
		x->sending = false;
		x->droppedpackets = 0;
		x->maxqueuesize  = DEFAULT_MAX_QUEUE_SIZE;
		x->maxpacketsize = DEFAULT_MAX_PACKETSIZE;
		x->queue_mutex = NULL;
		
		if(argc < 2)
		{
			object_post((t_object *)x, "usage: host port");
			goto bail;
		}
		if (argv[0].a_type != A_SYM)
		{
			object_post((t_object *)x, "invalid hostname argument");
			goto bail;	
		}
		x->ip = argv[0].a_w.w_sym;
		if(argv[1].a_type != A_LONG)
		{
			object_post((t_object *)x, "invalid port argument");
			goto bail;
		}
		x->port = argv[1].a_w.w_long;
		if(systhread_mutex_new(&(x->queue_mutex), 0) != 0)
		{
			object_error((t_object *)x, "could not create queue mutex. uh oh.");
			goto bail;
		} 
		if(systhread_cond_new(&(x->cond_empty), 0) != 0)
		{
			error("could not create queue cond. uh oh.");
			goto bail;
		} 
		x->queue = (t_linklist *)linklist_new();
		
		if(udpsend_netset(x) != 0)
		{
			goto bail;
		}
	}
	return(x);
	
bail:
	freeobject((t_object *)x);
	return NULL;
}

void udpsend_free(t_udpsend *x)
{
	//probaby should clean the queue unless linklist free just takes care of it which it probably does
	if (x->s)
		syssock_close(x->s);
	udpsend_stop(x);
	
	if(x->queue) {
		udpsend_cleanlinklist(x);
		freeobject((t_object *)x->queue);
	}
	
	if(x->queue_mutex)
		systhread_mutex_free(x->queue_mutex);
	if(x->cond_empty)
		systhread_cond_free(x->cond_empty);	
}

///OSC SEND STUFF cribbed/adapted from matt.............
//this stuff can be optimized with regards to calculating the initial buffer size
//also prone to crash if someone puts in more data than packet_size
#define PRINTABLE(c) ((char) (c>= 0x20 && c <= 0x7e ? c : 'û'))
#if !TARGET_RT_MAC_MACHO
#define isprint(c) ((c) >= 0x20 && (c) <= 0x7e)
#endif

void osc_enqueue_max_mess(t_udpsend *x, char *messageName, short argc, t_atom *argv,int packet_size) {
	/* Format the given Max data into buf. */
	   
	int i;	
	unsigned long bb_buf_start;
	t_bytebuf *bb;
	char type_string[MAX_ARGS_TO_OSC_MSG+2]; /* Space for comma and null */
	
	bb = bytebuf_new(packet_size);
	bb_buf_start = (long)bb->buf;
	
	//actual_mess_len = 0;
	//write address as padded string
	bb->buf += osc_pad_string(bb->buf, messageName);

	/* Write type string */
	type_string[0] = ',';		
	for (i=0; i < argc; i++) {
		switch (argv[i].a_type) {
			case A_LONG:
				type_string[i+1] = 'i';
				break;
					
			case A_FLOAT:
				type_string[i+1] = 'f';
				break;
					
			case A_SYM:
				type_string[i+1] = 's';
				break;
					
			default:
				object_error((t_object *)x, "OpenSoundControl: unrecognized argument type");
				break;
			}
		}
		type_string[i+1] = '\0';
	//write the type string			
	bb->buf += osc_pad_string(bb->buf, type_string);		

	for (i=0; i < argc; i++) {
		switch (argv[i].a_type) {
			case A_LONG:
				bb->buf += osc_write_int_arg(bb->buf, argv[i].a_w.w_long);
				break;				
			case A_FLOAT:		
				bb->buf += osc_write_float_arg(bb->buf, argv[i].a_w.w_float);
				break;				
			case A_SYM:
			    bb->buf += osc_write_string_arg(bb->buf, argv[i].a_w.w_sym->s_name);
				break;
			default:
				object_error((t_object *)x, "osc unrecognized argument type");
				break;
		}
	}
	//set the size so we are only sending this number of bytes over the network
	bb->size = (int)((unsigned long)bb->buf - bb_buf_start);
	bb->buf = (char*)bb_buf_start;
	udpsend_enqueue_dgram(x, bb);
	return;
}


void osc_packet_print_contents(t_bytebuf *bb)
{
	char *m, buf[100], *p;
	int n, i;	
	
	m = bb->buf;
	n = bb->size;
	
	if (n % 4 != 0) {
		post("Hey, the size isn't a multiple of 4!");
	} else {
	    for (i = 0; i < n; i += 4) {
	    	p = buf;
	    	
	    	p += sprintf(p, "  %x", m[i]);
	    	if (isprint(m[i])) {
	    		p += sprintf(p, "  (%c)", m[i]);
	    	} else {
	    		p += sprintf(p, "  ()");
	    	}
	    	
	    	p += sprintf(p, "  %x", m[i+1]);
	    	if (isprint(m[i+1])) {
	    		p += sprintf(p, "  (%c)", m[i+1]);
	    	} else {
	    		p += sprintf(p, "  ()");
	    	}
	    	
	    	p += sprintf(p, "  %x", m[i+2]);
	    	if (isprint(m[i+2])) {
	    		p += sprintf(p, "  (%c)", m[i+2]);
	    	} else {
	    		p += sprintf(p, "  ()");
	    	}
	    	
	    	p += sprintf(p, "  %x", m[i+3]);
	    	if (isprint(m[i+3])) {
	    		p += sprintf(p, "  (%c)", m[i+3]);
	    	} else {
	    		p += sprintf(p, "  ()");
	    	}
	    	
	    	*p = '\0';
	    	post(buf);
	    }
	
	}
}

#define STRING_ALIGN_PAD 4
int osc_effective_string_length(char *string) {
    int len = strlen(string) + 1;  /* We need space for the null char. */
     
     /* Round up len to next multiple of STRING_ALIGN_PAD to account for alignment padding */
     if ((len % STRING_ALIGN_PAD) != 0) {
         len += STRING_ALIGN_PAD - (len % STRING_ALIGN_PAD);
     }
     return len;
 }
 
 
int osc_write_float_arg(char *buf, float arg) {
     int *intp;
 
     /* Pretend arg is a long int so we can use htonl() */
     intp = ((int *) &arg);
     *((int *) buf) = syssock_htonl(*intp);

     return 4;
}

int osc_write_int_arg(char *buf, int arg){ 
     *((int*) buf) = syssock_htonl(arg);
     return 4;
 }
 
int osc_write_string_arg(char *buf, char *arg) {

	return osc_pad_string(buf, arg);
 
}
  
static int osc_pad_string(char *dest, char *str) 
 {
     
     int i;
     char c;
     
     for (i = 0; (c = *(str+i)) != '\0'; i++) {
         dest[i] = c;
     }     
     return osc_write_padding(dest, i);
 }
 
static int osc_write_padding(char *dest, int i) {
     dest[i] = '\0';
     i++;
 
     for (; (i % STRING_ALIGN_PAD) != 0; i++) {
         dest[i] = '\0';
    }
     return i;
}