#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <pthread.h>
#define MAXLINE 1024
#define PORT 8888

int sockfd, len;
struct sockaddr_in servaddr; 

void * doRecieving(void * sockId){
	
	while(1)
	{
		char buffer1[MAXLINE];
		bzero(buffer1,MAXLINE);

	    int n=recvfrom(sockfd,buffer1,MAXLINE, 0, (struct sockaddr *)&servaddr,&len);
	    
	    if (n < 0)
			perror("ERROR in reading");
		printf("Recieved from server: %s\n",buffer1);
		bzero(buffer1,MAXLINE);
	}
	return NULL;
}

int main(int argc,char *argv[]) { 
	//int sockfd;
	char buffer[MAXLINE]; 
	//struct sockaddr_in servaddr; 
	char *message="client";
	
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
		perror("Socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 

	memset(&servaddr, 0, sizeof(servaddr)); 

	servaddr.sin_family = AF_INET; 
	servaddr.sin_port = htons(PORT); 
	servaddr.sin_addr.s_addr = INADDR_ANY; 

	int n=10;
	len=sizeof(servaddr); 

	sendto(sockfd,(const char *)message,strlen(message), 
		0,(const struct sockaddr *)&servaddr,len);
	
	pthread_t thread;
	pthread_create(&thread, NULL, doRecieving, (void *) &sockfd );
		
	while(1)
	{
		//printf("Enter the message: ");
		bzero(buffer,MAXLINE);
		fgets(buffer,MAXLINE,stdin);
	    	
	    n=sendto(sockfd,(const char *)buffer,strlen(buffer), 0, (const struct sockaddr *)&servaddr,len);
		if (n < 0) 
			perror("ERROR in writing");
		else if(n>=0)
			puts("Acknowledged\n");
	    bzero(buffer,MAXLINE);
	 }
	close(sockfd); 
	//pthread_join(thread,NULL);
	return 0; 
}
