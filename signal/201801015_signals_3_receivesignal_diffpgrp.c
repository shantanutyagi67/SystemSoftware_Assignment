#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void signalHandler();

int main(void)
{
	signal( SIGINT, signalHandler );
	pid_t pid1, pid2;
	printf("Parent PID = %d, PGID = %d\n", getpid(), getpgid(0));
	int p = fork();
	if(p<0) {
		printf("Fork failed");
		return 0;
	}
	else if(p==0){  //child
		setpgid(getpid(), getpid());
		printf("Child PID = %d, PGID = %d\n", getpid(), getpgid(0));
	}
	else{   //parent
	}
	pause();
}

void signalHandler()
{
	printf("Process id %d received SIGINT signal\n", getpid());

}
