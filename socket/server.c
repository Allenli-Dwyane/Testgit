#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<netdb.h>
#include<sys/time.h>
#include<netinet/in.h>
#define PORT 8888
/*
int main(char argc,char *argv[])
{
    int socket_desc,new_socket,c;
    struct sockaddr_in server,client;

    if(socket_desc==-1)
    {
        printf("Could not create socket");
        return 1;
    }

    server.sin_family=AF_INET;
    server.sin_addr.s_addr=INADDR_ANY;
    server.sin_port=htons(PORT);

    //Bind
    if(bind(socket_desc,(struct sockaddr *)&server,sizeof(server))<0)
    {
        puts("bind failed");
        return 1;
    }
    puts("bind done");

    //Listen
    listen(socket_desc,3);
    
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c=sizeof(struct sockaddr_in);
    new_socket=accept(socket_desc,(struct sockaddr *)&client,(socklen_t*)&c);
    if(new_socket<0)
    {
        perror("accept failed");
        return 1;
    }
    puts("Connection accepted");

    char *message="Hello Client, I've received your connection.But I have to go now,bye\n";
    write(new_socket,message,strlen(message));
    return 0;
}
*/
int main(int argc,char *argv[])
{
    int listensock,connsock;
    struct addrinfo hints,*si;
    char buff[100];
    char recvbuf[100];
    int rv;

    memset(&hints,0,sizeof(hints));
    hints.ai_family=AF_INET;
    hints.ai_socktype=SOCK_STREAM;
    hints.ai_flags=AI_PASSIVE;

    if((rv=getaddrinfo(NULL,"5000",&hints,&si))!=0)
    {
        fprintf(stderr,"getaddrinfo: %s\n",gai_strerror(rv));
        exit(1);
    }
    
    if((listensock=socket(si->ai_family,si->ai_socktype,si->ai_protocol))==-1)
    {
        perror("socket");
        exit(1);
    }

    rv=1;
    if(setsockopt(listensock,SOL_SOCKET,SO_REUSEADDR,&rv,sizeof(int))==-1)
    {
        perror("setsockopt");
        exit(1);
    }

    if(bind(listensock,si->ai_addr,si->ai_addrlen)==-1){
        close(listensock);
        perror("bind");
        exit(1);
    }
    freeaddrinfo(si);

    if(listen(listensock,10)==-1){
        perror("listen");
        exit(1);
    }

    connsock=accept(listensock,NULL,NULL);
    close(listensock);
    /*
    if(send(connsock,buff,sizeof(buff),0)==-1){
        perror("send");
    }
    */
    while(1){
        memset(buff,'\0',sizeof(buff));
        memset(recvbuf,'\0',sizeof(recvbuf));
        //puts(buff);
        puts("----Input 'exit' to quit");
        puts("----Input your message to the client:");
        scanf("%[^\n]",buff);
        getchar();
        //puts(buff);
        if(strcmp(buff,"exit")==0){
            const char quitbuff[]="Server left the chatroom";
            if(send(connsock,quitbuff,sizeof(quitbuff),0)==-1){
                perror("send");
            }
            break;
        }
        else{
            if(send(connsock,buff,sizeof(buff),0)==-1){
                perror("send");
            }
            
            if(recv(connsock,recvbuf,sizeof(recvbuf),0)==-1){
                perror("recv");
            }
            printf("Client says to you:\"%s\"\n",recvbuf);
            if(strcmp(recvbuf,"Client left the chatroom")==0){
                break;
            }
        }
    }
    close(connsock);
    return 0;
}
