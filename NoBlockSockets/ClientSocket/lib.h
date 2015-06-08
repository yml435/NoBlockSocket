#ifndef LIB_H
#define LIB_H
#include    <stdio.h>
#include	<sys/types.h>	/* basic system data types */
#include	<sys/socket.h>	/* basic socket definitions */
#if TIME_WITH_SYS_TIME
#include	<sys/time.h>	/* timeval{} for select() */
#include	<time.h>		/* timespec{} for pselect() */
#else
#if HAVE_SYS_TIME_H
#include	<sys/time.h>	/* includes <time.h> unsafely */
#else
#include	<time.h>		/* old system? */
#endif
#endif
#include	<netinet/in.h>	/* sockaddr_in{} and other Internet defns */
#include	<arpa/inet.h>	/* inet(3) functions */
#include	<errno.h>
#include	<fcntl.h>		/* for nonblocking */
#include	<netdb.h>
#include	<signal.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<sys/stat.h>	/* for S_xxx file mode constants */
#include	<sys/uio.h>		/* for iovec{} and readv/writev */
#include	<unistd.h>
#include	<sys/wait.h>
#include	<sys/un.h>		/* for Unix domain sockets */
#define	SA	struct sockaddr
#define	MAXLINE		4096	/* max text line length */
#define err_sys(str)  printf("%s\n",str);
#define sys_quit(str) printf("%s\n",str); exit(0);
#define max(a,b) (a)>(b)? (a):(b)
static int	read_cnt;
void Bind(int fd, const struct sockaddr *sa, socklen_t salen);
void Listen(int fd, int backlog);
void
Listen(int fd, int backlog);
int
Socket(int family, int type, int protocol);
int
Accept(int fd, struct sockaddr *sa, socklen_t *salenptr);
void
Close(int fd);
void
Writen(int fd, void *ptr, size_t nbytes);
ssize_t						/* Write "n" bytes to a descriptor. */
writen(int fd, const void *vptr, size_t n);
void
Connect(int fd, const struct sockaddr *sa, socklen_t salen);
char *
Fgets(char *ptr, int n, FILE *stream);
ssize_t
Readline(int fd, void *ptr, size_t maxlen);
static ssize_t
my_read(int fd, char *ptr);
ssize_t
readline(int fd, void *vptr, size_t maxlen);
#endif
