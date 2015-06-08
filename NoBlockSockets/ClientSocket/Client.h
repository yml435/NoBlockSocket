#ifndef CLINET_H
#define CLIENT_H
#include "lib.h"
void client5_2();
void str_cli(FILE *fp,int sockfd);
void str_cli_select(FILE *fp,int sockfd);
void cache_sigpipe(int signal);
void str_cli_stdineof(FILE *fp,int sockfd);
#endif
