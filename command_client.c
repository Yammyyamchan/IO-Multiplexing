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

int main(int argc,char**argv)
{
    int sock;
    struct sockaddr_in serv_adr;
    char msg[MAX_SIZE],cmd[64];
    int cnt, pos;
    int len, i;

    if(argc!=3)
    {
        printf("%s <IP> <PORT> \n",argv[0]);
        exit(2);
    }

    sock=socket(PF_INET,SOCK_STREAM,0);
    memset(&serv_adr,0,sizeof(serv_adr));
    serv_adr.sin_family=AF_INET;
    serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
    serv_adr.sin_port=htons(atoi(argv[2]));

    if(connect(sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr))==-1)
        error_handling("connect() error");

    printf("connected................ \n");

    while(1)
    {
        pos=0;
        printf("Command count(0 to quit): ");
        scanf("%d",&cnt);
        if(cnt==0)
            break;
    
        msg[pos++]=(char)cnt;
    
        fgetc(stdin);
        for(i=0;i<cnt;i++)
        {
            printf("command[%d]: ",i+1);
            fgets(cmd,sizeof(cmd),stdin);
            len=strlen(cmd);
            --len;
            msg[pos++]=(char)len;
            strncpy(&msg[pos],cmd,len);
            pos+=len;
        }
        write(sock,msg,strlen(msg));
    
    }
    close(sock);
    return 0;
}