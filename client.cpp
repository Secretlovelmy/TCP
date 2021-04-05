#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>

#define BUFFSIZ 1024
#define PORT 4763
using namespace std;

int main(int argc, char *argv[])
{
        int client_sockfd;//client socket
        int len;
        struct sockaddr_in remote_addr; //server network address structure
        char buf[BUFSIZ];  //data buffer
        memset(&remote_addr,0,sizeof(remote_addr)); //memeory set
        remote_addr.sin_family=AF_INET; //IPv4
        remote_addr.sin_addr.s_addr= INADDR_ANY;//IP address
        remote_addr.sin_port=htons(PORT); //PORt number

        //create a TCP socket using the SOCK_DGRAM for TCP packets
        if((client_sockfd=socket(PF_INET,SOCK_STREAM,0))<0)
        {
                perror("socket error");
                return 1;
        }

        //Bind the socket with the server address
        if(connect(client_sockfd,(struct sockaddr *)&remote_addr,sizeof(struct sockaddr))<0)
        {
                perror("connect error");
                return 1;
        }
        printf("connected to server\n");
        len=recv(client_sockfd,buf,BUFSIZ,0);//receive the message
        //buf[len]='/0';
        printf("%s",buf); //printf the message

        //Send receive message for loop and print receive message (can be sent on demand)
        //-recv returns the number of bytes received, send returns the number of bytes sent
        int i=0;
        while(i<1)
        {
            const char *hello = "Client X: Alice";
            len=send(client_sockfd,hello,strlen(hello),0);
            len=recv(client_sockfd,buf,BUFSIZ,0);
            //buf[len]='/0';
            printf("%s\n", hello);
            printf("received:%s\n",buf);
            i++;
        }

        //close the socket
        close(client_sockfd);

        return 0;
}
