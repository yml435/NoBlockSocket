#include"strCli_noBlock.h"
inline void nBlockFcntl(int fd){
    int  fcntlVal;
    if((fcntlVal=fcntl(fd,F_GETFL,0))==-1){
        sys_quit("fcntl error for read sockfd");
    }else{
        if(fcntl(fd,F_SETFL,fcntlVal|O_NONBLOCK)<0){//set the NO_BLOCK for sockfd
            sys_quit("fcntl error for set sockfd");
        }
    }
}

void strCli_noBlock(int sockfd){
    int  n=0;
    char to[MAXLINE];
    char fr[MAXLINE];
    char *tooptr,*toiptr;
    char *froptr,*friptr;
    fd_set frset,wset;//用于SELECT函数的描述符
    int  maxfdp=0; //the max number fd for function select
    /*set noblock for STDOUT_FILENO STDIN_FILENO sockfd*/
    noBlockFcntl(STDOUT_FILENO);
    noBlockFcntl(STDIN_FILENO);
    noBlockFcntl(sockfd);
    tooptr=toiptr=to;//init the pointer
    froptr=friptr=fr;
    int stdinEOF=0; //a flag for the readin EOF
    maxfdp=max(sockfd,max(STDIN_FILENO,STDOUT_FILENO));
    while(1){
        FD_ZERO(&frset);
        FD_ZERO(&wset);
        if((&to[MAXLINE]-toiptr>0)&&(stdinEOF==0)){
            FD_SET(STDIN_FILENO,&frset);
        }
        if(toiptr>tooptr){
            FD_SET(sockfd,&wset);
        }
        if(&fr[MAXLINE]>friptr){
            FD_SET(sockfd,&frset);
        }
        if(friptr>froptr){
            FD_SET(STDOUT_FILENO,&wset);
        }
        if(select(maxfdp,&frset,&wset,NULL,NULL)<0){
            err_sys("ERROR FOR SELECT IN FUNCTION STRCLI_BLOCK()");
        }
        if(FD_ISSET(STDIN_FILENO,&frset)){
            if((n=read(STDIN_FILENO,toiptr,&to[MAXLINE]-toiptr))<0){
                if(errno!=EWOULDBLOCK){
                    err_sys("error for read stdin");
                }
            }
            else if(n==0){
                sys_msg("stdin EOF");
                stdinEOF=1;
                if(tooptr==toiptr){
                    shutdown(sockfd,SHUT_WR);
                }
            }else{
                printf("read %d from stdin\n",n);
                toiptr+=n;
                FD_SET(sockfd,&wset);
            }
        }
        if(FD_ISSET(sockfd,&frset)){
            if((n=read(sockfd,friptr,&fr[MAXLINE]-friptr))<0){
                if(errno!=EWOULDBLOCK){
                    err_sys("read error from sockfd");
                }
            }
            else if(n==0){
                sys_msg("read EOF of sockfd");
                if(stdinEOF){
                    return ;
                }
            }
            else{
                printf("read %d bytes form sockfd",n);
                friptr+=n;
                FD_SET(STDOUT_FILENO,&wset);
            }

        }
        if(FD_ISSET(STDOUT_FILENO,&wset)&&friptr-froptr>0){
            if((n=write(STDOUT_FILENO,froptr,friptr-froptr))<0){
                if(errno!=EWOULDBLOCK){
                    err_sys("STDOUT error");
                }
            }
            else{
                printf("write %d bytes data to stdout",n);
                froptr+=n;
                if(froptr==friptr){
                    froptr=friptr=fr;
                }
            }

        }
        if(FD_ISSET(sockfd,&wset)&&toiptr-tooptr>0){
            if((n=write(sockfd,tooptr,toiptr-tooptr))<0){
                if(errno!=EWOULDBLOCK){
                    err_sys("SOCKFD WRITE ERROR");
                }
            }else{
                printf("write %d bytes data to sockfd",n);
                tooptr+=n;
                if(tooptr==toiptr){
                    tooptr=toiptr=to;
                    if(stdinEOF==1){
                        shutdown(sockfd,SHUT_WR);
                    }

                }
            }
        }
    }
}
