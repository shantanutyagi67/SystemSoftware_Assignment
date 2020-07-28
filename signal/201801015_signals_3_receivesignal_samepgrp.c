#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void signalHandler();

int main(void)
{
	signal( SIGINT, signalHandler );
	pid_t pid1, pid2;

	int p = fork();
	if(p<0) {
		printf("Fork failed\n");
		return 0;
	}
	else if(p==0){  //child
		pid2 = getpid();
		printf("Child PID = %d, PGID = %d\n", pid2, getpgid(0));
	}
	else{   //parent
		//sleep(1); // child will now execute before parent
		pid1 = getpid();
		printf("Parent PID = %d, PGID = %d\n", pid1, getpgid(0));
	}
	pause();
}

void signalHandler()
{
	printf("Process id %d received SIGINT signal\n", getpid());

}
