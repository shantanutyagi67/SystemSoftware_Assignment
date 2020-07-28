#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define PORT 8080
#define noClients 3

void * doRecieving(void * sockID){
	int clientSocket = *((int *) sockID);
	while(1){
		char output2[1024];
		int read2 = recv(clientSocket,output2,1024,0); // forwarded msg from server
		printf("\nrecieved from server: %s\n",output2);
		//printf("Enter Message: ");
		//scanf("%s",input);
		//if(send(clientSocket,input,1024,0)>=0){ //send message to client
			//printf("Sever got your message\n");
		//}
		//int read = recv(clientSocket,output1,1024,0); // message reached appreciation from server
		//printf("%s\n",output1);
	}
	return NULL;
}

int main(){

	int clientSocket = socket(PF_INET, SOCK_STREAM, 0); // make socket
	if(clientSocket<0){	//error
		printf("Socket creation failed...");
		exit(0);
	}
	printf("Socket created...");
	struct sockaddr_in serverAddr; // the usual

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(connect(clientSocket, (struct sockaddr*) &serverAddr, sizeof(serverAddr)) == -1) return 0; //connect
	printf("Connection established...\n");// appreciation from the server on being connected

	pthread_t thread;
	pthread_create(&thread, NULL, doRecieving, (void *) &clientSocket );	// call recieving function
	while(1){// sending function
		char input[1024];
		//printf("Enter Message: ");
		scanf("%s",input);
		if(send(clientSocket,input,1024,0)>=0){ //send message to client
			printf("Sever got your message\n");
		}
	}
		pthread_join(thread,NULL);
		return 0;
}
