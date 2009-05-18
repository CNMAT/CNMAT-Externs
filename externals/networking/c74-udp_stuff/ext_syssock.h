// ext_syssock.h
#ifndef _EXT_SYSSOCK_H_
#define _EXT_SYSSOCK_H_

#define _WINSOCKAPI_

#ifndef WIN32
//we shouldnt need this system dependent stuff anymore.
//it should be in the prefix headers
//#include <MacTypes.h>
//#include <devices.h> 
//#include <MacWindows.h> 
//#include <controls.h> 
#endif

#include "ext.h"

#ifdef WIN_VERSION

#include <winsock2.h>

typedef int socklen_t;
typedef int int32_t;
typedef unsigned int u_int32_t;
typedef	u_int32_t  in_addr_t;	/* base type for internet address */
typedef WSABUF data_chunk;


#endif

#ifdef MAC_VERSION

#if TARGET_RT_MAC_MACHO
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <netdb.h>
	#include <unistd.h>
	#include <signal.h>
	#include <stdio.h>
	#include <fcntl.h>
	#include <errno.h>
	#include <sys/time.h>
	#include <sys/uio.h>		
	#include <stdlib.h>
	#include <memory.h>
	
	typedef struct iovec data_chunk;
#else 

#define	AF_INET		2		/* internetwork: UDP, TCP, etc. */

#define	SOCK_STREAM	1		/* stream socket */
#define	SOCK_DGRAM	2		/* datagram socket */
#define	SOCK_RAW	3		/* raw-protocol interface */
#define	SOCK_RDM	4		/* reliably-delivered message */
#define	SOCK_SEQPACKET	5	/* sequenced packet stream */

#define	INADDR_ANY		(u_int32_t)0x00000000
#define	INADDR_LOOPBACK		(u_int32_t)0x7f000001
#define	INADDR_BROADCAST	(u_int32_t)0xffffffff	/* must be masked */


/*
 * Option flags per-socket.
 */
#define	SO_DEBUG	0x0001		/* turn on debugging info recording */
#define	SO_ACCEPTCONN	0x0002		/* socket has had listen() */
#define	SO_REUSEADDR	0x0004		/* allow local address reuse */
#define	SO_KEEPALIVE	0x0008		/* keep connections alive */
#define	SO_DONTROUTE	0x0010		/* just use interface addresses */
#define	SO_BROADCAST	0x0020		/* permit sending of broadcast msgs */
#define	SO_USELOOPBACK	0x0040		/* bypass hardware when possible */
#define	SO_LINGER	0x0080		/* linger on close if data present */
#define	SO_OOBINLINE	0x0100		/* leave received OOB data in line */
#define	SO_REUSEPORT	0x0200		/* allow local address & port reuse */
#define	SO_TIMESTAMP	0x0400		/* timestamp received dgram traffic */
#ifndef __APPLE__
#define	SO_ACCEPTFILTER	0x1000		/* there is an accept filter */
#else
#define SO_DONTTRUNC	0x2000		/* APPLE: Retain unread data */
					/*  (ATOMIC proto) */
#define SO_WANTMORE		0x4000		/* APPLE: Give hint when more data ready */
#define SO_WANTOOBFLAG	0x8000		/* APPLE: Want OOB in MSG_FLAG on receive */
#endif

#define _BSD_SOCKLEN_T_ int32_t                 /* socklen_t (duh) */
#ifdef	_BSD_SOCKLEN_T_
typedef	_BSD_SOCKLEN_T_	socklen_t;
#undef	_BSD_SOCKLEN_T_
#endif

/*
 * Level number for (get/set)sockopt() to apply to socket itself.
 */
#define	SOL_SOCKET	0xffff		/* options for socket level */

typedef	unsigned char	u_char;
typedef	unsigned short	u_short;
typedef	unsigned int	u_int;
typedef	unsigned long	u_long;

typedef	u_int32_t  in_addr_t;	/* base type for internet address */

//gather/scatter memory writes and reads to/from the network interface
//with writev and readv use this struct
typedef struct iovec {
	void	*iov_base;	//starting address of buffer
	size_t	iov_len;	//size of the buffer
} data_chunk;

/*
 * Internet address (a structure for historical reasons)
 */
struct in_addr {
	in_addr_t s_addr;
};

/*
 * Structure used by kernel to store most
 * addresses.
 */
struct sockaddr {
	u_char	sa_len;			/* total length */
	u_char	sa_family;		/* address family */
	char		sa_data[14];		/* actually longer; address value */
};

/*
 * Socket address, internet style.
 */
struct sockaddr_in {
	u_char	sin_len;
	u_char	sin_family;
	u_short	sin_port;
	struct	in_addr sin_addr;
	char	sin_zero[8];
};

/*
 * Structures returned by network data base library.  All addresses are
 * supplied in host order, and returned in network order (suitable for
 * use in system calls).
 */
 //this structure is returned by gethostname
struct	hostent {
	char	*h_name;	/* official name of host */
	char	**h_aliases;	/* alias list */
	int	h_addrtype;	/* host address type */
	int	h_length;	/* length of address */
	char	**h_addr_list;	/* list of addresses from name server */
#define	h_addr	h_addr_list[0]	/* address, for backward compatiblity */
};

#define	IPPROTO_TCP		6		/* tcp */
#define	IPPROTO_UDP		17		/* user datagram protocol */

#define	TCP_NODELAY	0x01	/* don't delay send to coalesce packets */

#define SIGPIPE 13 	// write on a pipe with no one to read it

#if defined(_ANSI_SOURCE) || defined(__cplusplus)
/*
 * Language spec sez we must list exactly one parameter, even though we
 * actually supply three.  Ugh!
 */
#define	SIG_DFL		(void (*)(int))0
#define	SIG_IGN		(void (*)(int))1
#define	SIG_ERR		(void (*)(int))-1
#else
#define	SIG_DFL		(void (*)())0
#define	SIG_IGN		(void (*)())1
#define	SIG_ERR		(void (*)())-1
#endif




//from errno.h

/*
 * Error codes
 */

#define	EPERM		1		/* Operation not permitted */
#define	ENOENT		2		/* No such file or directory */
#define	ESRCH		3		/* No such process */
#define	EINTR		4		/* Interrupted system call */
#define	EIO		5		/* Input/output error */
#define	ENXIO		6		/* Device not configured */
#define	E2BIG		7		/* Argument list too long */
#define	ENOEXEC		8		/* Exec format error */
#define	EBADF		9		/* Bad file descriptor */
#define	ECHILD		10		/* No child processes */
#define	EDEADLK		11		/* Resource deadlock avoided */
					/* 11 was EAGAIN */
#define	ENOMEM		12		/* Cannot allocate memory */
#define	EACCES		13		/* Permission denied */
#define	EFAULT		14		/* Bad address */
#ifndef _POSIX_SOURCE
#define	ENOTBLK		15		/* Block device required */
#endif
#define	EBUSY		16		/* Device busy */
#define	EEXIST		17		/* File exists */
#define	EXDEV		18		/* Cross-device link */
#define	ENODEV		19		/* Operation not supported by device */
#define	ENOTDIR		20		/* Not a directory */
#define	EISDIR		21		/* Is a directory */
#define	EINVAL		22		/* Invalid argument */
#define	ENFILE		23		/* Too many open files in system */
#define	EMFILE		24		/* Too many open files */
#define	ENOTTY		25		/* Inappropriate ioctl for device */
#ifndef _POSIX_SOURCE
#define	ETXTBSY		26		/* Text file busy */
#endif
#define	EFBIG		27		/* File too large */
#define	ENOSPC		28		/* No space left on device */
#define	ESPIPE		29		/* Illegal seek */
#define	EROFS		30		/* Read-only file system */
#define	EMLINK		31		/* Too many links */
#define	EPIPE		32		/* Broken pipe */

/* math software */
#define	EDOM		33		/* Numerical argument out of domain */
#define	ERANGE		34		/* Result too large */

/* non-blocking and interrupt i/o */
#define	EAGAIN		35		/* Resource temporarily unavailable */
#ifndef _POSIX_SOURCE
#define	EWOULDBLOCK	EAGAIN		/* Operation would block */
#define	EINPROGRESS	36		/* Operation now in progress */
#define	EALREADY	37		/* Operation already in progress */

/* ipc/network software -- argument errors */
#define	ENOTSOCK	38		/* Socket operation on non-socket */
#define	EDESTADDRREQ	39		/* Destination address required */
#define	EMSGSIZE	40		/* Message too long */
#define	EPROTOTYPE	41		/* Protocol wrong type for socket */
#define	ENOPROTOOPT	42		/* Protocol not available */
#define	EPROTONOSUPPORT	43		/* Protocol not supported */
#define	ESOCKTNOSUPPORT	44		/* Socket type not supported */
#endif /* ! _POSIX_SOURCE */
#define ENOTSUP	45		/* Operation not supported */
#ifndef _POSIX_SOURCE
#define	EOPNOTSUPP	 ENOTSUP		/* Operation not supported */
#define	EPFNOSUPPORT	46		/* Protocol family not supported */
#define	EAFNOSUPPORT	47		/* Address family not supported by protocol family */
#define	EADDRINUSE	48		/* Address already in use */
#define	EADDRNOTAVAIL	49		/* Can't assign requested address */

/* ipc/network software -- operational errors */
#define	ENETDOWN	50		/* Network is down */
#define	ENETUNREACH	51		/* Network is unreachable */
#define	ENETRESET	52		/* Network dropped connection on reset */
#define	ECONNABORTED	53		/* Software caused connection abort */
#define	ECONNRESET	54		/* Connection reset by peer */
#define	ENOBUFS		55		/* No buffer space available */
#define	EISCONN		56		/* Socket is already connected */
#define	ENOTCONN	57		/* Socket is not connected */
#define	ESHUTDOWN	58		/* Can't send after socket shutdown */
#define	ETOOMANYREFS	59		/* Too many references: can't splice */
#define	ETIMEDOUT	60		/* Operation timed out */
#define	ECONNREFUSED	61		/* Connection refused */

#define	ELOOP		62		/* Too many levels of symbolic links */
#endif /* _POSIX_SOURCE */
#define	ENAMETOOLONG	63		/* File name too long */

/* should be rearranged */
#ifndef _POSIX_SOURCE
#define	EHOSTDOWN	64		/* Host is down */
#define	EHOSTUNREACH	65		/* No route to host */
#endif /* _POSIX_SOURCE */
#define	ENOTEMPTY	66		/* Directory not empty */

/* quotas & mush */
#ifndef _POSIX_SOURCE
#define	EPROCLIM	67		/* Too many processes */
#define	EUSERS		68		/* Too many users */
#define	EDQUOT		69		/* Disc quota exceeded */

/* Network File System */
#define	ESTALE		70		/* Stale NFS file handle */
#define	EREMOTE		71		/* Too many levels of remote in path */
#define	EBADRPC		72		/* RPC struct is bad */
#define	ERPCMISMATCH	73		/* RPC version wrong */
#define	EPROGUNAVAIL	74		/* RPC prog. not avail */
#define	EPROGMISMATCH	75		/* Program version wrong */
#define	EPROCUNAVAIL	76		/* Bad procedure for program */
#endif /* _POSIX_SOURCE */

#define	ENOLCK		77		/* No locks available */
#define	ENOSYS		78		/* Function not implemented */

#ifndef _POSIX_SOURCE
#define	EFTYPE		79		/* Inappropriate file type or format */
#define	EAUTH		80		/* Authentication error */
#define	ENEEDAUTH	81		/* Need authenticator */
#endif /* _POSIX_SOURCE */

/* Intelligent device errors */
#define	EPWROFF		82	/* Device power is off */
#define	EDEVERR		83	/* Device error, e.g. paper out */

#ifndef _POSIX_SOURCE
#define	EOVERFLOW	84		/* Value too large to be stored in data type */

/* Program loading errors */
#define EBADEXEC	85	/* Bad executable */
#define EBADARCH	86	/* Bad CPU type in executable */
#define ESHLIBVERS	87	/* Shared library version mismatch */
#define EBADMACHO	88	/* Malformed Macho file */

#define	ECANCELED	89		/* Operation canceled */

#define EIDRM		90		/* Identifier removed */
#define ENOMSG		91		/* No message of desired type */   
#define EILSEQ		92		/* Illegal byte sequence */
#define ENOATTR		93		/* Attribute not found */

#define	ELAST		93		/* Must be equal largest errno */
#endif /* _POSIX_SOURCE */

#ifdef KERNEL
/* pseudo-errors returned inside kernel to modify return to process */
#define	ERESTART	-1		/* restart syscall */
#define	EJUSTRETURN	-2		/* don't modify regs, just return */
#endif




#ifdef syssock_errno
#undef syssock_errno
#endif // #ifdef errno
//#define syssock_errno  (*fp_errno())

#endif //TARGET_RT_MAC_MACHO
typedef int	SOCKET;
typedef void *(*t_mp)(void *x,...); //pointer to method
typedef void *(*t_fpv)(void); //pointer to void function
//typedef int *(*t_imp)(void *x, ...);  //pointer to method that returns an int
typedef char *(*t_mp_in_addr)(struct in_addr x);
typedef struct hostent *(*t_mp_hostent)(const char *c);
// $rbs$ -- we need to use this to get the proper errno
typedef int * (*__mygeterror_ptr)();
#endif //MAC_VERSION


//SYSSOCK BEGINNING

typedef SOCKET t_syssocket;
typedef struct sockaddr_in t_sysaddr;


#define SYSSOCK_CONNECT_SUCCESS 0
#define SYSSOCK_BIND_SUCCESS 0
#ifdef MAC_VERSION
#define SYSSOCK_INTERRUPTED_SYSTEM_CALL EINTR
#define SYSSOCK_INVALID_SOCKET -1
#define SYSSOCK_BIND_ERROR -1
#define SYSSOCK_LISTEN_ERROR -1
#define SYSSOCK_ACCEPT_ERROR -1
#define SYSSOCK_WRITECHUNKS_ERROR -1
#endif
#ifdef WIN_VERSION
#define SYSSOCK_INVALID_SOCKET  INVALID_SOCKET
#define SYSSOCK_INTERRUPTED_SYSTEM_CALL WSAEINTR
#define SYSSOCK_BIND_ERROR SOCKET_ERROR
#define SYSSOCK_LISTEN_ERROR SOCKET_ERROR
#define SYSSOCK_ACCEPT_ERROR SOCKET_ERROR
#define SYSSOCK_WRITECHUNKS_ERROR SOCKET_ERROR
#endif

#define SYSSOCK_READN_ERROR -1

#define SYSSOCK_BACKLOG 16 //backlog param for listen method.  ie, how many requests should be queued.


//initialize the library.  must be called before any other syssock activity. 
//returns zero if successful, errno if not.
int syssock_init();

t_syssocket syssock_socket(int domain, int type, int protocol);
int syssock_setsockopt(t_syssocket sockfd, int level, int option_name, const void *option_value, int option_len);
int syssock_bind(t_syssocket sockfd, t_sysaddr *my_addr);
int syssock_connect(t_syssocket sockfd, t_sysaddr *serv_addr);
int syssock_listen(t_syssocket sockfd); 
int syssock_accept(t_syssocket sockfd, t_sysaddr *cliaddr);
int syssock_recv(t_syssocket sockfd, void *buf, int len, unsigned int flags);
int syssock_sendto(t_syssocket sockfd, const void *buff, size_t nbytes, int flags, const struct sockaddr *to,int tolen);
int syssock_recvfrom(t_syssocket sockfd, void *buf, int len, unsigned int flags, struct sockaddr *from,int *fromlen);
int syssock_writev(t_syssocket filedes, const struct iovec *iov, int iovcnt);
int syssock_send(t_syssocket sockfd, const void *buff, size_t nbytes, int flags);
int syssock_read(t_syssocket sockfd, void *buf, int len);
void syssock_close(t_syssocket sockfd);
u_short syssock_htons(u_short s);
u_long syssock_htonl(u_long l);
u_short syssock_ntohs(u_short s);
u_long syssock_ntohl(u_long l);
char *syssock_inet_ntoa(struct in_addr inaddr);
//const char *syssock_inet_ntop(int family, const void *addrptr, char *strptr, size_t len);
in_addr_t syssock_inet_addr(const char *strptr);
struct hostent *syssock_gethostbyname(const char *name);

//turn the nagle algorithm on (val == 1) or off (val == 0)
void syssock_nagle(t_syssocket sockfd, long val);
//set the option that allows quick re-use of a port whose socket was recently closed
//returns 0 if successful
int syssock_reuseaddr(t_syssocket sockfd);
//read in len bytes into the bp data buffer.
//returns the number of bytes read if successful
//returns SYSSOCK_READN_ERROR if there is a problem (closed socket, for instance)
long syssock_readn(t_syssocket fd, char *bp, size_t len);
//assign data and size to the cross-platform "chunk" data container.
//note that data is NOT copied so do not free it until after it has been written
void syssock_setchunk(data_chunk *chunk, void *data, long size);
//write any number of chunks out as one logical packet
int syssock_writechunks(t_syssocket sockfd, data_chunk *chunk, long count);
//zeroes the sin_zero part of the t_sysaddr struct 
//and sets addr->sin_family to family
void syssock_setfamily(t_sysaddr *addr, u_char family);
void syssock_setip(t_sysaddr *addr, in_addr_t ip);
void syssock_setport(t_sysaddr *addr, u_short port);
//returns the current errno
int syssock_errno();

float swapf32(float f);
double swapf64(double f);

#endif // _EXT_SYSSOCK_H_
