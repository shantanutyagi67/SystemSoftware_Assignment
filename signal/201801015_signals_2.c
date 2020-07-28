#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define	READ	0      
#define	WRITE	1 

char* questions[] = {"Quit", "In which university do you study?", "Which course are you studying?","What is your area of interest?"};
char* answers[] = {"Quit", "DAIICT", "Systems Software", "Kernel Programming"};
int i, pid1, pid2, status;
int fd1[2],fd2[2], bytesRead1, bytesRead2, que = -1, ans = -1; 
char message1[100], message2[100];
static void signal_handler1(int);
static void signal_handler2(int);

int main(void) 
{
	if( signal( SIGUSR1, signal_handler1) == SIG_ERR  )
     	printf("Parent: Unable to create handler for SIGUSR1\n");
    if( signal( SIGUSR2, signal_handler2) == SIG_ERR  )
    	printf("Parent: Unable to create handler for SIGUSR2\n");
    
    pid1 = getpid();
	pipe(fd1); 
	pipe(fd2);

	if ( fork() == 0 )  //Child
	{	
		close(fd2[READ]);
		close(fd1[WRITE]);
		pid2 = getpid();
		//pause();
		while(1);
	} 
	else	//Parent
	{
		//sleep(1);
		close(fd1[READ]);
		close(fd2[WRITE]);
		do {
			printf("Enter question number (0-3): ");
			scanf("%d",&que);
		} while(que<0&&que>3);
		write(fd1[WRITE], questions[que], strlen(questions[que])+1);
		printf("Sent question: %s, to the child\n", questions[que]);
		printf("Sending SIGUSR1 to the child\n");
		kill( pid2, SIGUSR1 );
		wait(NULL);
		//pause();
	}
	exit(0);
}

static void signal_handler1(int signo)
{
	if(getpid()==pid2){ // SIGUSR1 handler implemented by the child
		printf("Child recieved SIGUSR1\n");
		//close(fd1[WRITE]);
		//close(fd2[READ]);
		bytesRead1 = read(fd1[READ], message1, 100 );
		printf("Child read %d bytes from the question: %s \n", bytesRead1, message1 );
		int len=4;
		for(int i=0;i<len;++i){
			if(strcmp(questions[i],message1)==0){
				ans=i;
				break;
			}
		}
		write(fd2[WRITE], answers[ans], strlen(answers[ans])+1);
		printf("Sent answer: %s, to the parent\n", answers[ans]);
		printf("Sending SIGUSR2 to the parent\n");
		kill( pid1, SIGUSR2 );
		if(ans==0){
			close(fd1[WRITE]);
			close(fd2[WRITE]);
			close(fd1[READ]);
			close(fd2[READ]);
			printf("Child exits\n");
			kill(pid2,SIGINT);
		}
				//pause();
	}
}
static void signal_handler2(int signo)
{
	if ( getpid() == pid1 ){ // SIGUSR1 handler implemented by the parent
		printf("Parent recieved SIGUSR2\n");
		bytesRead2= read(fd2[READ], message2, 100);
		printf("Parent read %d bytes from the answer: %s \n", bytesRead2, message2 );
		//close(fd2[WRITE]);
		//close(fd1[READ]);
		if ( que == 0 ){
			waitpid(pid2,NULL,0);
			close(fd1[WRITE]);
			close(fd2[WRITE]);
			close(fd1[READ]);
			close(fd2[READ]);
			printf("Parent exits\n");
			return;
		}
		do {
			printf("Enter question number (0-3): ");
			scanf("%d",&que);
		} while(que<0&&que>3);
		write(fd1[WRITE], questions[que], strlen(questions[que])+1);
		printf("Sent question: %s, to the child\n", questions[que]);
		printf("Sending SIGUSR1 to the child\n");
		kill( pid2, SIGUSR1 );
		//pause();
	}
}
