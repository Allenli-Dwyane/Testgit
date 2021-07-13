#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<netdb.h>
#include<sys/time.h>
#include<netinet/in.h>
/*
int main(int argc,char *argv[])
{
    int socket_desc;
    struct sockaddr_in server;
    socket_desc=socket(AF_INET,SOCK_STREAM,0);
    //Address Family-AF_INET  Type-SOCK_STREAM   Protocol-0
    
    if(socket_desc==-1)
    {
        printf("Could not create socket");
    }
    server.sin_addr.s_addr=inet_addr("172.17.170.179");
    server.sin_family=AF_INET;
    server.sin_port=htons(PORT);

    if(connect(socket_desc,(struct sockaddr *)&server,sizeof(server))<0)
    {
        puts("connect error");
        return 1;
    }
    puts("connected");

    char *message="My First http\n";
    if(send(socket_desc,message,strlen(message),0)<0)
    {
        puts("send failed");
        return 1;
    }
    puts("Data Send\n");

    char server_reply[2000];
    if(recv(socket_desc,server_reply,2000,0)<0)
    {
        puts("recv failed");
    }
    puts("Reply received\n");
    puts(server_reply);

    close(socket_desc);

    return 0;
}
*/
int main(int argc,char *argv[])
{
    char recvbuff[100];
    int sockfd;
    struct addrinfo hints,*si;
    int rv;
    /*
    if(argc!=2){
        printf("Usage: %s ip\n",argv[0]);
        exit(0);
    }
    */
    memset(&hints,0,sizeof(hints));
    hints.ai_family=AF_INET;
    hints.ai_socktype=SOCK_STREAM;

    if((rv=getaddrinfo(argv[1],"5000",&hints,&si))!=0){
        fprintf(stderr,"getaddrinfo: %s\n",gai_strerror(rv));
        exit(1);
    }

    if((sockfd=socket(si->ai_family,si->ai_socktype,si->ai_protocol))==-1){
        perror("socket");
        exit(1);
    }

    if(connect(sockfd,si->ai_addr,si->ai_addrlen)==-1){
        perror("connect");
        exit(1);
    }
    freeaddrinfo(si);

    if(recv(sockfd,recvbuff,sizeof(recvbuff),0)==-1){
        perror("recv");
    }
    printf("%s\n",recvbuff);
    close(sockfd);
    return 0;
}