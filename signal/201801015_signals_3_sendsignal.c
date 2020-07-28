#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int main(void)
{
	pid_t pid;
	int toGroup;
	printf("Enter Process Id to which the signal needs to be sent: ");
	scanf("%d",&pid);
	do{
		printf("Enter 1 to send signal to process or 2 to for process group: ");
		scanf("%d",&toGroup);
	}while(toGroup!=1 && toGroup!=2);
	if(toGroup == 1) kill(pid, SIGINT);
	else kill(-pid, SIGINT);
	return 0;
}
