#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <limits.h>

#define	READ	0     
#define	WRITE	1    

void main(void) 
{
	// variables
	char* questions[] = {"Quit", "In which university do you study?", "Which course are you studying?", "What is your area of interest?"};
	char* answers[] = {"Quit", "DAIICT", "Systems Software", "Kernel Programming"};
	int fd1[2], fd2[2], bytesRead, que=-1; 
	char message1[200], message2[200]; //for reading by child and parent respectively 
	
	// create pipes
	if( pipe(fd1) == -1 ) { // message from parent to child
		printf("pipe1 failed!");
		return;
	}
	if( pipe(fd2) == -1 ){ // message from child to parent
		printf("pipe1 failed!");
		return;
	}
	
	// create parent-child processes
	pid_t p = fork();
	
	if(p<0){
		printf("fork failed!");
		return;
	}
	else if ( p == 0 )  //child
	{
		int len=4,ans=-1;
		close(fd1[WRITE]); // close writind end of the pipe to currently read from
		close(fd2[READ]);  //  close reading end of the pipe to later reply from
		do{
		bytesRead = read(fd1[READ], message1, 200 );
		printf("Child read %d bytes from the question: %s \n", bytesRead, message1 );
		//close(fd1[READ]); 
		for(int i=0;i<len;++i){ // find index of question
			if(strcmp(questions[i],message1)==0){
				ans=i;// store index in ans
				break;
			}
		}
		write(fd2[WRITE], answers[ans], strlen(answers[ans])+1); //send the answer
		printf("Sent answer: %s, to the parent\n", answers[ans]);
		//close(fd2[WRITE]);
		}while(ans!=0);// quit condition
		if(ans==0){
		close (fd1[READ]); // close the open ends
	  	close (fd2[WRITE]);
	  	printf("Child exits\n");
		exit(0);
		}
	} 
	else
	{
		close(fd1[READ]);  // close the reading end of the pipe to crrently ask from
		close(fd2[WRITE]); // close the writng end of the pipe to later read reply from
		do{//ask questions untill quit
			do{// only valid question indexes
				printf("\nEnter question number (0-3): ");
				scanf("%d",&que);
			}while(que<0||que>3);
			write(fd1[WRITE], questions[que], strlen(questions[que])+1);//send Q
			printf("Sent question: %s, to the child\n", questions[que]);
			//close(fd1[WRITE]);  
			bytesRead = read(fd2[READ], message2, 200);//read answer
			printf("Parent read %d bytes from the answer: %s \n", bytesRead, 				message2 );
			//close(fd2[READ]);
		}while(que!=0);//quit
		wait(NULL);// wait for child to exit
		close(fd2[READ]);// close the open ends
		close(fd1[WRITE]);  
		printf("Parent exits\n");
		exit(0);//exit with no zombie process
	}
}
