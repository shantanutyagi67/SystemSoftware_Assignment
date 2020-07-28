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
#define TOTAL_CLIENTS 3

int clientCount = 0;
int mode;

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

struct client{ // client class to that we can index multiple clients and their socket Ids
	int again;// used for controlling in cyclic logic of message passing
	int index;
	int sockID;
	struct sockaddr_in clientAddr; // the usual structure that we always use for clients
	int len;
};

struct client Client[TOTAL_CLIENTS];	//make array on clients
pthread_t thread[TOTAL_CLIENTS];	// same number of threads to handle each clients networking process

void * doNetworking(void * ClientDetail){	// the main networking function
	struct client* clientDetail = (struct client*) ClientDetail;
	int index = clientDetail -> index;
	//int again = clientDetail -> again;
	int clientSocket = clientDetail -> sockID;
	printf("Client %d connected.\n",index + 1);
	if(mode==1){ // OO mode
		while(1){
			char data[1024], ackn[1024]={'r','e','c','i','e','v','e','d'},temp[1024]={'s','t','a','r','t'};
			//if(index==0&&Client[0].again == 1){
				//send(Client[index].sockID,temp,1024,0);
				//Client[0].again = 0;
			//}
			int read = recv(clientSocket,data,1024,0);
			printf("recieved from client %d: %s\n",index + 1,data);
			//send(Client[index].sockID,ackn,1024,0);// acknowledge
			send(Client[(index+1)%clientCount].sockID,data,1024,0); // forward to next client			
			}
			return NULL;
	}
	else{ // BC mode
		while(1){
			char data[1024], ackn[1024]={'r','e','c','i','e','v','e','d'},temp[1024]={'s','t','a','r','t'};
			//if(index==0&&Client[0].again == 1){
				//send(Client[index].sockID,temp,1024,0);
				//Client[0].again = 0;
			//}
			int read = recv(clientSocket,data,1024,0);
			printf("recieved from client %d: %s\n",index + 1,data);
			//send(Client[index].sockID,ackn,1024,0);// acknowledge
			for(int i=1;i<TOTAL_CLIENTS;i++)
				send(Client[(index+i)%clientCount].sockID,data,1024,0); // forward to next client			
			}
			return NULL;
	}
	}

int main(){ //driver
	do {
		printf("ENTER 1 for OO and 2 for BC MODE: ");
		scanf("%d",&mode);
	} while( mode !=1  && mode != 2 );
	int serverSocket = socket(PF_INET, SOCK_STREAM, 0); // create a socket
	if(serverSocket<0){	//error
		printf("Socket creation failed...\n");
		exit(0);
	}
	printf("Socket created...\n");
	
	struct sockaddr_in serverAddr;

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = htons(INADDR_ANY);

	if(bind(serverSocket,(struct sockaddr *) &serverAddr , sizeof(serverAddr)) == -1) {
	printf("Binding error...\n");
	return 0; //bind
	}
	printf("Binding complete...\n");
	if(listen(serverSocket,1024) == -1) return 0;	//listen
	printf("Server started listenting on port %d.\n",PORT);

	while(1){	// keep accepting clients as they send connection request
		Client[clientCount].sockID = accept(serverSocket, (struct sockaddr*) &Client[clientCount].clientAddr, &Client[clientCount].len);
		if(Client[clientCount].sockID < 0) {
			printf("Connection failed...\n");
			exit(1);
		}
		Client[clientCount].again = 1;
		Client[clientCount].index = clientCount; //hashing the clients in first come first serve basis
		pthread_create(&thread[clientCount], NULL, doNetworking, (void *) &Client[clientCount]); //thread calls the the do networking function along with making a new thread to handle each client
		clientCount ++; //counter
	}

	for(int i = 0 ; i < clientCount ; i ++) // thread syncronisation
		pthread_join(thread[i],NULL);
	return 0;
}
