#indef STR_CLI_NO_BLOCK_H
#define STR_CLI_NO_BLOCK_H
#include "lib.h"
extern char to[MAXLINE];
extern char fr[MAXLINE]
void strCli_noBlock(int sockfd);
inline void noBlockFcntl(int fd);
#endif
