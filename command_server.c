#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <string.h>

#define MAX_SIZE 1024
void error_handling(char* message)
{
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}
void cmdexe(int sock,int cmd_cnt,char* buf);

int main(int argc,char**argv)
{
    int serv_sock,clnt_sock;
    struct sockaddr_in serv_adr,clnt_adr;
    socklen_t adr_sz;
    char msg[MAX_SIZE];

    fd_set reads,cpy_reads;
    struct timeval timeout;
    int maxfd,fd_cnt,i;
    int len,cmd_cnt;
    
    if(argc !=2)
    {
        printf("usage: %s <PORT> \n",argv[0]);
        exit(2);
    }
    serv_sock=socket(PF_INET,SOCK_STREAM,0);
    memset(&serv_adr,0,sizeof(serv_adr));
    serv_adr.sin_family=AF_INET;
    serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_adr.sin_port=htons(atoi(argv[1]));

    if(bind(serv_sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr))==-1)
        error_handling("bind() error");

    if(listen(serv_sock,5)==-1)
        error_handling("listen() error");

    FD_ZERO(&reads);
    FD_SET(serv_sock,&reads);
    maxfd=serv_sock;

    while(1)
    {
        cpy_reads=reads;
        timeout.tv_sec=5;
        timeout.tv_usec=0;

        fd_cnt=select(maxfd+1,&cpy_reads,0,0,&timeout);
        if(fd_cnt==-1)
            error_handling("select() error");

        else if(fd_cnt ==0)
            continue;
        
        for(i=0;i<maxfd+1;i++)
        {
            if(FD_ISSET(i,&cpy_reads))
            {
                if(i==serv_sock)
                {
                    adr_sz=sizeof(clnt_adr);
                    clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_adr,&adr_sz);
                    if(clnt_sock==-1)
                        error_handling("accept() error");
                    printf("client connected: %d \n",clnt_sock);

                    FD_SET(clnt_sock,&reads);
                    if(maxfd<clnt_sock)
                        maxfd=clnt_sock;
                }
                else
                {
                    len=read(i,msg,MAX_SIZE);
                    if(len==0)
                    {
                        FD_CLR(i,&reads);
                        printf("closed client: %d \n",i);
                        close(i);
                    }
                    else
                    {
                       cmd_cnt=msg[0];
                       printf("===============================\n");
                       cmdexe(i,cmd_cnt,&msg[1]);
                       printf("Received from client: %d \n",i);
                       printf("===============================\n");
                    }

                }
            }
        }
        
    }
    close(serv_sock);
    return 0;
}

void cmdexe(int sock,int cmd_cnt,char* buf)
{
    int pos=0;
    int i,cmd_len;
    char cmd[64];
    int sock_num=sock;
    
    for(i=0;i<cmd_cnt;i++)
    {
        cmd_len=buf[pos++];
        strncpy(cmd,&buf[pos],cmd_len);
        pos+=cmd_len;
        cmd[cmd_len]='\0';
        printf("%s\n",cmd);
        system(cmd);
        printf("\n");
    }
   
}