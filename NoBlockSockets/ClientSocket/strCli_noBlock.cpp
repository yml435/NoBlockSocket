
inline void nBlockFcntl(int fd){
    if((fcntlVal=fcntl(fd,F_GETFL,0))==-1){
        sys_quit("fcntl error for read sockfd");
    }else{
        if(fcntl(fd,F_SETFL,fcntlVal|O_NONBLOCK)<0){//set the NO_BLOCK for sockfd
            sys_quit("fcntl error for set sockfd");
        }
    }
}

void strCli_noBlock(int sockfd){
    int fcntlVal;
    char to[MAXLINE];
    char fr[MAXLINE];
    char *tooptr,*toiptr;
    char *froptr,*friptr;
    fd_set frset,wset;//用于SELECT函数的描述符
    /*set noblock for STDOUT_FILENO STDIN_FILENO sockfd*/
    noBlockFcntl(STDOUT_FILENO);
    noBlockFcntl(STDIN_FILENO);
    noBlockFcntl(sockfd);
    toopr=toiptr=to;//init the pointer
    froptr=friptr=fr;





















}
