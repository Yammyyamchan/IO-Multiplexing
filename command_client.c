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
        printf("Command count(0 to quit): ");   //명령어 개수를 입력받는다.
        scanf("%d",&cnt);                       //명령어 개수를 scanf()을 이용해 저장
        if(cnt==0)
            break;                              //명령어 개수가 0이면 프로그램 종료
    
        msg[pos++]=(char)cnt;                   //입력받은 명령어 개수를 메시지의 첫번째 인덱스에 저장
    
        fgetc(stdin);
        for(i=0;i<cnt;i++)                      //명령어 개수만큼 반복
        {
            printf("command[%d]: ",i+1);        //명령어를 입력한다.
            fgets(cmd,sizeof(cmd),stdin);       //입력한 명령어를 cmd에 저장한다.
            len=strlen(cmd);                    
            --len;                              //fgets()함수로 입력해 저장해서 \n을 빼야됨.
            msg[pos++]=(char)len;               // \n을 뺀 명령어의 길이를 메시지 인덱스에 저장
            strncpy(&msg[pos],cmd,len);         // cmd의 명령어를 메시지에 저장함
            pos+=len;                           //메시지의 인덱스 값을 증가시킴
        }
        write(sock,msg,strlen(msg));
    
    }
    close(sock);
    return 0;
}
