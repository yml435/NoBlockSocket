#include"Client.h"
void str_cli(FILE *fp,int sockfd){
    char recvline[MAXLINE];
    char sendline[MAXLINE];
    while(Fgets(sendline,MAXLINE,fp)!=NULL){
        Writen(sockfd,sendline,strlen(sendline));
        if(Readline(sockfd,recvline,MAXLINE)==0){
            err_sys("strcli:server termained permantely");
        }
        printf("%s",recvline);
    }

}
void str_cli_stdineof(FILE *fp,int sockfd){
    int maxfdp;
    int stdineof;
    fd_set rset;
    char sendline[MAXLINE];
    char recvline[MAXLINE];
    FD_ZERO(&rset);
    stdineof=0;
    while(1){
        if(stdineof==0){
            FD_SET(fileno(fp),&rset);
        }
        //FD_SET(fileno(fp),&rset);
        FD_SET(sockfd,&rset);
        maxfdp=max(fileno(fp),sockfd)+1;
        if(select(maxfdp,&rset,NULL,NULL,NULL)<0){
            err_sys("select error");
            return;
        }
        if(FD_ISSET(fileno(fp),&rset)){
            if(Fgets(sendline,MAXLINE,fp)==NULL){
                stdineof=1;
                shutdown(sockfd,SHUT_WR);
                FD_CLR(fileno(fp),&rset);
                continue;
            }
            Writen(sockfd,sendline,strlen(sendline));
        }
        if(FD_ISSET(sockfd,&rset)){
            if(Readline(sockfd,recvline,MAXLINE)==0){
                if(stdineof==1){
                   sys_quit("server normal  termained permantely");
                }else{
                   sys_quit("strcli:server un-normal termained permantely");
                }
            }
            printf("%s\n",recvline);
        }
    }
}
void client5_2(){
    read_cnt=0;
    const char addr[20]="127.0.0.1";
    //std::cout<<addr<<std::endl;
    int sockfd;
    struct sockaddr_in servaddr;
    sockfd=Socket(AF_INET,SOCK_STREAM,0);
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(9877);
    if(inet_pton(AF_INET,addr,&servaddr.sin_addr)==NULL){
        err_sys("inet_pton,error");
    }
    if(signal(SIGPIPE,cache_sigpipe)==SIG_ERR){
        err_sys("sigpipe signal caught error!");
    }
    Connect(sockfd,(SA*)&servaddr,sizeof(servaddr));
    str_cli_select(stdin,sockfd);
    exit(0);
}
void str_cli_select(FILE *fp,int sockfd){
    int maxfdp;
    fd_set rset;
    char sendline[MAXLINE];
    char recvline[MAXLINE];
    FD_ZERO(&rset);
    while(1){
        FD_SET(fileno(fp),&rset);
        FD_SET(sockfd,&rset);
        maxfdp=max(fileno(fp),sockfd)+1;
        if(select(maxfdp,&rset,NULL,NULL,NULL)<0){
            err_sys("select error");
            return;
        }
        if(FD_ISSET(fileno(fp),&rset)){
            if(Fgets(sendline,MAXLINE,fp)==NULL)
                return;
            Writen(sockfd,sendline,strlen(sendline));
        }
        if(FD_ISSET(sockfd,&rset)){
            if(Readline(sockfd,recvline,MAXLINE)==0){
                err_sys("strcli:server termained permantely");
                return ;
            }
            printf("%s\n",recvline);
        }

    }
}
void cache_sigpipe(int signal){
    printf("%s\n","we caught sigpid signal");
    return ;
}
