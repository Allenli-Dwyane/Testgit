#include<stdio.h>
#include<sys/socket.h>
int main(int argc,char *argv[])
{
    int socket_desc;
    socket_desc=socket(AF_INET,SOCK_STREAM,0);
    //Address Family-AF_INET  Type-SOCK_STREAM   Protocol-0
    
    if(socket_desc==-1)
    {
        printf("Could not create socket");
    }

}