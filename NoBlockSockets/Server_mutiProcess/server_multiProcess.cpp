#include"server_multiProcess.h"

void server_multiProcess(){

    int maxi;//存储最大数组下标，记录连接数组的最大值
    int connfd; //用于存储连接的描述符
    int listenfd; //用于存放监听的描述符
    int sockfd; //监听套接字
    int clilen;
    int maxfd;
    int nready;
    fd_set rset,allset;    //
    char recvbuf[MAXLINE];
    int client[FD_SETSIZE];
    struct sockaddr_in cliaddr,servaddr;
    listenfd=Socket(AF_INET,SOCK_STREAM,0);
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    servaddr.sin_port=htons(SERV_PORT);
    Bind(listenfd,(SA*)&servaddr,sizeof(servaddr));
    Listen(listenfd,LISTENQ);
    maxfd=listenfd;
    maxi=-1;
    for(int i=0;i<FD_SETSIZE; i++){
        client[i]=maxi;
    }
    FD_ZERO(&allset);
    FD_SET(listenfd,&allset);
    while(1){
        rset=allset;
        nready=select(maxfd+1,&rset,NULL,NULL,NULL);
        if(FD_ISSET(listenfd,&rset)){
            int i;
            socklen_t clilen=sizeof(cliaddr);
            connfd=Accept(listenfd,(SA*)&cliaddr,&clilen);
            for(i=0;i<FD_SETSIZE;i++){
                if(client[i]<0){
                    client[i]=connfd;
                    break;
                }
            }
            if(i==FD_SETSIZE){
                err_quit("the number of client greater then FD_SETSIZE");
            }
            FD_SET(connfd,&allset);
            if(connfd>maxfd){
                maxfd=connfd;
            }
            if(i>maxi){
                maxi=i;
            }
            if(--nready<=0)
                continue;

        }

        for(int i=0;i<=maxi; i++){
            if((sockfd=client[i])<0)
                continue;
            if(FD_ISSET(sockfd,&rset)){
                int n;
                memset(recvbuf,0,MAXLINE);
                if((n=read(sockfd,recvbuf,MAXLINE))==0){
                    Close(sockfd);
                    FD_CLR(sockfd,&rset);
                    client[i]=-1;
                }else{
                    printf("%s\n",recvbuf);
                    Writen(sockfd,recvbuf,n);
                }

                if(--nready<0)
                    break;
            }
        }
    }

}

