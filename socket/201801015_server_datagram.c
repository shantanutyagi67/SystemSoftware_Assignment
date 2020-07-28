#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include<pthread.h>
#define MAXLINE 1024 
#define PORT 8888

int K=3;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int main(int argc,char *argv[]) { 
	int sockfd,mode; 
	mode=atoi(argv[1]);

	struct sockaddr_in servaddr, cliaddr;
	if(mode==1)
		puts("ONE to ONE MODE");
	else
		puts("BROADCAST MODE");
	puts("Waiting for connections...\n");

	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
		perror("Socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	memset(&servaddr, 0, sizeof(servaddr)); 
	memset(&cliaddr, 0, sizeof(cliaddr));
	
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = INADDR_ANY; 
	servaddr.sin_port = htons(PORT); 
	int arr[K];
	for(int i=0;i<K;i++)arr[i]=-999999;
	int t=0;

	if ( bind(sockfd, (const struct sockaddr *)&servaddr,
		sizeof(servaddr)) < 0 )
	{
		perror("Bind failed");
		exit(EXIT_FAILURE);
	}
	int index=0;
	while(1)
	{
		char buffer[MAXLINE]; 
		bzero(buffer,MAXLINE);
		
		int len,n,flag=0;  
		
		len = sizeof(cliaddr); //len is value/result 

		n = recvfrom(sockfd,buffer,MAXLINE,0, 
		    (struct sockaddr *)&cliaddr,&len); 

		buffer[n] = '\0'; 
		
		
		int no=ntohs(cliaddr.sin_port);
		if(strcmp(buffer,"client")==0)
		{
			arr[t++]=no; 
			flag=1;
		}		
		
		else
		{
			for(int j=0;j<t;j++)
			{	
				
				if(arr[j]==no)
				{
					index=j;
					break;
				}
			}
		}

		if(!flag)
		printf("Message from Client : %s\n",buffer); 
		
		if(mode==2&&!flag)
		{
			for(int j=(index+1)%K;;j=(j+1)%K)
			{	
				if(j==index)break;
				cliaddr.sin_port = htons(arr[j]);
				if(j!=index)
				{
				   sendto(sockfd,(const char *)buffer,strlen(buffer),0,(const struct sockaddr*) &cliaddr,len);
				}
			}
		}
		else if(mode==1&&!flag)
		{
			cliaddr.sin_port = htons(arr[(index+1)%K]);
			sendto(sockfd,(const char *)buffer,strlen(buffer),0,(const struct sockaddr*) &cliaddr,len);		
		}
		bzero(buffer,MAXLINE);
	}
	free(&sockfd);
	 return 0; 
}
