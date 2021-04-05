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
int main(int argc, char *argv[])
{
        int server_sockfd;//server socket
        int client_sockfd[2];//client socket
        socklen_t len;
        struct sockaddr_in my_addr;   //server network address structure
        struct sockaddr_in remote_addr; //client network address structure
        struct timeval begin, end, differ;
        socklen_t sin_size;
        char buf[BUFSIZ];  //data transfer buffer
        char allBuf[2][BUFSIZ];
        memset(&my_addr,0,sizeof(my_addr)); //set memory
        my_addr.sin_family=AF_INET; //IPv4
        my_addr.sin_addr.s_addr=INADDR_ANY;//IP address
        my_addr.sin_port=htons(PORT); //port number

        //create a TCP socket using the SOCK_DGRAM for TCP packets
        if((server_sockfd=socket(PF_INET,SOCK_STREAM,0))<0)
        {
                perror("socket error");
                return 1;
        }


        //Bind the socket with the server address
        if(bind(server_sockfd,(struct sockaddr *)&my_addr,sizeof(struct sockaddr))<0)
        {
                perror("bind error");
                return 1;
        }


        //set the listen queue with 5.
        if(listen(server_sockfd,5)<0)
        {
                perror("listen error");
                return 1;
        };

        sin_size=sizeof(struct sockaddr_in);
        int i=0;
        while(i<2){
            gettimeofday(&begin,NULL);

            //server accept the connnect request of the client
            if((client_sockfd[i]=accept(server_sockfd,(struct sockaddr *)&remote_addr,&sin_size))<0)
            {
                    perror("accept error");
                    return 1;
            }
            printf("accept client %s\n",inet_ntoa(remote_addr.sin_addr));
            //send the hello message to client.
            len=send(client_sockfd[i],"Welcome to my server\n",21,0);


            //receive the message of the client
            len=recv(client_sockfd[i],buf,BUFSIZ,0);
            printf("%s\n",buf);
            strcpy(allBuf[i],buf);            
            i++;

        }
		
                //print the receive order of the client message
		char response[1024];
		memset(response, 0, sizeof(response));
		const char *before = " before ";
		strcat(response, allBuf[0]);
		strcat(response, before);
		strcat(response, allBuf[1]);
		for(int i = 0; i < 2; i++){
				if(send(client_sockfd[i],response, 20000,0)<0)
				{
					perror("write error");
					return 1;
				}
			
		}
		printf("Sent acknowledgment to both %s and %s \n", allBuf[0], allBuf[1]);

        //close the socket
        for(int i = 0; i < 2; i++){
			close(client_sockfd[i]);
			}
        close(server_sockfd);

        return 0;
}
