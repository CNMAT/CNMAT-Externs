#include "ext_syssock.h"

void (*syssock_signal(int signo, void (*func)(int)))(int);

#ifdef MAC_VERSION
//function pointers for calling Mach-O (from CFM)
//lazily using t_mp for convenience, but could use more 
//explicit typedefs for each function call if typechecking is desired
t_mp 	fp_socket=NULL,
		fp_setsockopt=NULL,
		fp_bind=NULL,
		fp_htons=NULL,
		fp_htonl=NULL,
		fp_ntohs=NULL,
		fp_ntohl=NULL,
		fp_listen=NULL,
		fp_connect=NULL,
		fp_accept=NULL,
		fp_send=NULL,
		fp_sendto=NULL,
		fp_recv=NULL,
		fp_recvfrom=NULL,
		fp_read=NULL,
		fp_close=NULL,
		fp_inet_addr=NULL,
		fp_writev=NULL,
		fp_signal=NULL;
t_mp_hostent fp_gethostbyname=NULL;
__mygeterror_ptr fp_errno=NULL;
t_mp_in_addr 	fp_inet_ntoa=NULL;

#if !TARGET_RT_MAC_MACHO
// Apple utility code for CFM callback glue
UInt32 template[6] = {0x3D800000, 0x618C0000, 0x800C0000, 0x804C0004, 0x7C0903A6, 0x4E800420};
void	*MachOFunctionPointerForCFMFunctionPointer( void *cfmfp );

//	utility code taken from from Apple's CFM_MachO_CFM example:
//
//	This function allocates a block of CFM glue code which contains the instructions to call CFM routines
//
void	*MachOFunctionPointerForCFMFunctionPointer( void *cfmfp )
{
    UInt32	*mfp = (UInt32*) NewPtr( sizeof(template) );		//	Must later dispose of allocated memory
    															//	(this is freed with app heap in this object)
    mfp[0] = template[0] | ((UInt32)cfmfp >> 16);
    mfp[1] = template[1] | ((UInt32)cfmfp & 0xFFFF);
    mfp[2] = template[2];
    mfp[3] = template[3];
    mfp[4] = template[4];
    mfp[5] = template[5];
    MakeDataExecutable( mfp, sizeof(template) );
    return( mfp );
}
#endif//TARGET_RT_MAC_MACHO
#endif

int syssock_init()
{
#ifdef MAC_VERSION
    #if !TARGET_RT_MAC_MACHO
	CFBundleRef bundle=NULL;

    //load pthread function pointers
    if (bundle = CFBundleGetBundleWithIdentifier(CFSTR("com.apple.Carbon")))
	{
		fp_socket 				= (t_mp) CFBundleGetFunctionPointerForName(bundle, CFSTR("socket"));
		fp_bind					= (t_mp) CFBundleGetFunctionPointerForName(bundle, CFSTR("bind"));
		fp_htons				= (t_mp) CFBundleGetFunctionPointerForName(bundle, CFSTR("htons"));
		fp_htonl				= (t_mp) CFBundleGetFunctionPointerForName(bundle, CFSTR("htonl"));
		fp_ntohs				= (t_mp) CFBundleGetFunctionPointerForName(bundle, CFSTR("ntohs"));
		fp_ntohl				= (t_mp) CFBundleGetFunctionPointerForName(bundle, CFSTR("ntohl"));
		fp_listen				= (t_mp) CFBundleGetFunctionPointerForName(bundle, CFSTR("listen"));
		fp_connect				= (t_mp) CFBundleGetFunctionPointerForName(bundle, CFSTR("connect"));
		fp_accept				= (t_mp) CFBundleGetFunctionPointerForName(bundle, CFSTR("accept"));
		fp_inet_ntoa			= (t_mp_in_addr) CFBundleGetFunctionPointerForName(bundle, CFSTR("inet_ntoa"));
		fp_send					= (t_mp) CFBundleGetFunctionPointerForName(bundle, CFSTR("send"));
		fp_recv					= (t_mp) CFBundleGetFunctionPointerForName(bundle, CFSTR("recv"));
		fp_sendto				= (t_mp) CFBundleGetFunctionPointerForName(bundle, CFSTR("sendto"));
		fp_recvfrom				= (t_mp) CFBundleGetFunctionPointerForName(bundle, CFSTR("recvfrom"));
		fp_read					= (t_mp) CFBundleGetFunctionPointerForName(bundle, CFSTR("read"));
		fp_close				= (t_mp) CFBundleGetFunctionPointerForName(bundle, CFSTR("close"));
		fp_setsockopt			= (t_mp) CFBundleGetFunctionPointerForName(bundle, CFSTR("setsockopt"));
		fp_inet_addr			= (t_mp) CFBundleGetFunctionPointerForName(bundle, CFSTR("inet_addr"));
		fp_writev				= (t_mp) CFBundleGetFunctionPointerForName(bundle, CFSTR("writev"));
		fp_signal				= (t_mp) CFBundleGetFunctionPointerForName(bundle, CFSTR("signal"));
		//fp_sigaction			= (t_mp) CFBundleGetFunctionPointerForName(bundle, CFSTR("sigaction"));
		fp_errno = (__mygeterror_ptr) CFBundleGetFunctionPointerForName(bundle, CFSTR("__error"));
		//fp_getaddrinfo			= (t_imp) CFBundleGetFunctionPointerForName(bundle, CFSTR("getaddrinfo"));
		fp_gethostbyname		= (t_mp_hostent) CFBundleGetFunctionPointerForName(bundle, CFSTR("gethostbyname"));
    }
	
	#endif //!TARGET_RT_MAC_MACHO
//do error checking in here?    
// 	if (fp_socket==NULL||fp_pthread_create==NULL) { //||fp_cc_simplethread_threadproc==NULL) {
//		error("could not create function pointers necessary for Macho-O <-> CFM communication");
//		return NULL;
//	}

	// according to http://tangentsoft.net/wskfaq/articles/bsd-compatibility.html
	// this signal call is not necessary in Windows world: 
	/*
	Under Unix, if you're blocking on send() and your program is ignoring the SIGPIPE signal, 
	it will return with a -1 when the remote peer disconnects, and errno will be EPIPE. 
	Otherwise, your program will be sent the SIGPIPE signal, which will terminate your program 
	if you don't handle it. Under Winsock, the SIGPIPE/EPIPE functionality does not exist at 
	all: send() will either return 0 for a normal disconnect or -1 for an abnormal disconnect, 
	with WSAGetLastError() returning the same errors as in the recv() discussion above.
	*/
	syssock_signal(SIGPIPE, SIG_IGN); 
#endif
#ifdef WIN_VERSION
	static Boolean already_initialized = false;
	WSADATA info;
	if (!already_initialized) {
		//if (WSAStartup(MAKEWORD(2,0), &info) != 0)  //winsock library 1.1 -- maybe should be 2,0?
		if (WSAStartup(0x0202, &info) != 0)  //winsock library 1.1 -- maybe should be 2,0?
			return (int)(syssock_errno);
//			error("cannot initialize WinSock, error %i", syssock_errno);
		else 
		{
			//post("winsock %i", info.wVersion);
			already_initialized = true;
		}
	}
#endif
	return 0;
}


#ifdef MAC_VERSION
void (*syssock_signal(int signo, void (*func)(int)))(int)
{
	#if TARGET_RT_MAC_MACHO
		return signal(signo,func);
	#else
		return fp_signal((void *)signo, (void *)func);
	#endif
}
#endif


t_syssocket syssock_socket(int domain, int type, int protocol)
{
#if defined(MAC_VERSION) && !TARGET_RT_MAC_MACHO
	return (int)(fp_socket((void *)domain, (void *)type, (void *)protocol));
#else 
	return socket(domain, type, protocol);
#endif
}


int syssock_setsockopt(t_syssocket sockfd, int level, int option_name, const void *option_value, int option_len)
{
#if defined(MAC_VERSION) && !TARGET_RT_MAC_MACHO
	return (int)fp_setsockopt(	(void *)sockfd,
								(void *)level,
								(void *)option_name,
								option_value,
								(void *)option_len);
#else
	return setsockopt(sockfd, level, option_name, option_value, option_len);
#endif
}


int syssock_bind(t_syssocket sockfd, t_sysaddr *my_addr)
{
	long addrlen = sizeof(t_sysaddr);
#if defined(MAC_VERSION) && !TARGET_RT_MAC_MACHO
	return (int)fp_bind((void *)sockfd, (void *)my_addr, (void *)addrlen);	
#else
	return bind(sockfd, (struct sockaddr *)my_addr, addrlen);
#endif
}


int syssock_connect(t_syssocket sockfd, t_sysaddr *serv_addr)
{
	int addrlen = sizeof(t_sysaddr);
#if defined(MAC_VERSION) && !TARGET_RT_MAC_MACHO
	return (int)(fp_connect((void *)sockfd, (void *)serv_addr, (void *)addrlen));
#else
	return connect(sockfd, (struct sockaddr *)serv_addr, addrlen);
#endif
}


int syssock_listen(t_syssocket sockfd) 
{
	int backlog = SYSSOCK_BACKLOG;
#if defined(MAC_VERSION) && !TARGET_RT_MAC_MACHO
	return (int)fp_listen((void *)sockfd, (void *)backlog);
#else
	return listen(sockfd, backlog);
#endif
}


int syssock_accept(t_syssocket sockfd, t_sysaddr *cliaddr)
{
	socklen_t len = sizeof(t_sysaddr);
#if defined(MAC_VERSION) && !TARGET_RT_MAC_MACHO
	return (int)fp_accept((void *)sockfd, (void *)cliaddr, (void *)&len);
#else
	return accept(sockfd, (struct sockaddr *)cliaddr, &len);
#endif
}


u_short syssock_htons(u_short s)
{
#if defined(MAC_VERSION) && !TARGET_RT_MAC_MACHO
	return (u_short)fp_htons((void *)s);
#else
	return htons(s);
#endif
}


u_long syssock_htonl(u_long l)
{
#if defined(MAC_VERSION) && !TARGET_RT_MAC_MACHO
	return (u_long)fp_htonl((void *)l);
#else
	return htonl(l);
#endif
}


u_short syssock_ntohs(u_short s)
{
#if defined(MAC_VERSION) && !TARGET_RT_MAC_MACHO
	return (u_short)fp_ntohs((void *)s);
#else
	return ntohs(s);
#endif
}


u_long syssock_ntohl(u_long l)
{
#if defined(MAC_VERSION) && !TARGET_RT_MAC_MACHO
	return (u_long)fp_ntohl((void *)l);
#else
	return ntohl(l);
#endif
}


char *syssock_inet_ntoa(struct in_addr inaddr)
{
#if defined(MAC_VERSION) && !TARGET_RT_MAC_MACHO
	return (char *)fp_inet_ntoa(inaddr);
#else
	return inet_ntoa(inaddr);
#endif
}


int syssock_recv(t_syssocket sockfd, void *buf, int len, unsigned int flags)
{
#if defined(MAC_VERSION) && !TARGET_RT_MAC_MACHO
	return (int)fp_recv((void *)sockfd, (void *)buf, (void *)len, (void *)flags);
#else
	return recv(sockfd, buf, len, flags);
#endif
}


int syssock_send(t_syssocket sockfd, const void *buff, size_t nbytes, int flags)
{
#if defined(MAC_VERSION) && !TARGET_RT_MAC_MACHO
	return (int)fp_send((void *)sockfd, (void *)buff, (void *)nbytes, (void *)flags);
#else
	return send(sockfd, buff, nbytes, flags);
#endif
}



int syssock_recvfrom(t_syssocket sockfd, void *buf, int len, unsigned int flags, struct sockaddr *from,int *fromlen)
{
#if defined(MAC_VERSION) && !TARGET_RT_MAC_MACHO
	return (int)fp_recvfrom((void *)sockfd, (void *)buf, (void *)len, (void *)flags, (void *)from, (void *)fromlen);
#else
	return recvfrom(sockfd, buf, len, flags, from, (socklen_t *) fromlen);
#endif
}

int syssock_sendto(t_syssocket sockfd, const void *buff, size_t nbytes, int flags, const struct sockaddr *to,int tolen)
{
#if defined(MAC_VERSION) && !TARGET_RT_MAC_MACHO
	return (int)fp_sendto((void *)sockfd, (void *)buff, (void *)nbytes, (void *)flags,(void *) to, (void *) tolen);
#else
	return sendto(sockfd, buff, nbytes, flags, to, tolen);
#endif
}

void syssock_setchunk(data_chunk *chunk, void *data, long size) 
{
#ifdef MAC_VERSION
	chunk->iov_base = data;
	chunk->iov_len = size;
#endif
#ifdef WIN_VERSION
	chunk->buf = data;
	chunk->len = size;
#endif
}


int syssock_writechunks(t_syssocket sockfd, data_chunk *chunk, long count)
{
#ifdef MAC_VERSION
	#if TARGET_RT_MAC_MACHO
	return writev(sockfd, chunk,count);
	#else
	return (int)fp_writev((void *)sockfd, (void *)chunk, (void *)count);	
	#endif
#endif
#ifdef WIN_VERSION
	DWORD sent;
	return WSASend( sockfd, chunk, count, &sent, 0, NULL, NULL );
#endif
}


void syssock_close(t_syssocket sockfd)
{
#ifdef MAC_VERSION
 #if TARGET_RT_MAC_MACHO
	close(sockfd);
 #else
	fp_close((void *)sockfd);
 #endif
#endif
#ifdef WIN_VERSION
	closesocket(sockfd);
#endif
}


in_addr_t syssock_inet_addr(const char *strptr)
{
#if defined(MAC_VERSION) && !TARGET_RT_MAC_MACHO
	return (in_addr_t)fp_inet_addr((void *)strptr);
#else
	return inet_addr(strptr);
#endif
}


void syssock_nagle(t_syssocket sockfd, long val)
{
	const int on = 1;	
	if (val == 0)
		syssock_setsockopt( sockfd, IPPROTO_TCP, TCP_NODELAY, &on, sizeof(on) );
	else if (val == 1)
		syssock_setsockopt( sockfd, IPPROTO_TCP, 0, &on, sizeof(on) );
}


int syssock_reuseaddr(t_syssocket sockfd)
{
	int yes=1;	
	return syssock_setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,(void *)&yes, sizeof(int));
}


struct hostent *syssock_gethostbyname(const char *name)
{
#if defined(MAC_VERSION) && !TARGET_RT_MAC_MACHO
	return fp_gethostbyname(name);
#else
	return gethostbyname(name);
#endif
}


long syssock_readn(t_syssocket fd, char *bp, size_t len)
{
	int rc;
	int cnt=len;
	while (cnt > 0)
	{
		rc = syssock_recv( fd, bp, cnt, 0 );
		
		if (rc < 0 ) 			//read error?
		{
			if (syssock_errno() == SYSSOCK_INTERRUPTED_SYSTEM_CALL)	//interrupted?
				continue;		//restart the read
			return SYSSOCK_READN_ERROR;			//return error
		}
		if ( rc == 0 )			//eof?
			return len - cnt;	//return short count
		bp += rc;
		cnt -= rc;
	}
	return len;
}


void syssock_setfamily(t_sysaddr *addr, u_char family)
{
	short i;
	for (i=0;i<8;i++) 
	   	addr->sin_zero[i] = '\0'; // zero the rest of the struct	
	addr->sin_family = family;  // this should remain in host byte order
}


void syssock_setip(t_sysaddr *addr, in_addr_t ip)
{
	addr->sin_addr.s_addr = (ip);
}


void syssock_setport(t_sysaddr *addr, u_short port) 
{	
	addr->sin_port = (port);
}


int syssock_errno()
{
#ifdef MAC_VERSION
	#if TARGET_RT_MAC_MACHO
	return errno;
	#else
	return *fp_errno();
	#endif
#endif
#ifdef WIN_VERSION
	return WSAGetLastError();
#endif
}

float swapf32(float f){
	unsigned char c,*a;

	a = (unsigned char *)&f;
	c=a[3]; a[3]=a[0]; a[0]=c;
	c=a[2]; a[2]=a[1]; a[1]=c;

	return f;
}

double swapf64(double f){
	unsigned char c,*a;

	a = (unsigned char *)&f;
	c=a[7]; a[7]=a[0]; a[0]=c;
	c=a[6]; a[6]=a[1]; a[1]=c;
	c=a[5]; a[5]=a[2]; a[2]=c;
	c=a[4]; a[4]=a[3]; a[3]=c;

	return f;
}	
