#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
	
void alarmHandler();	
int alarmFlag = 0;

int main(void)
{
	signal( SIGALRM, alarmHandler );
	char s[25];
	printf("Enter a string: ");
	alarm(10);
	scanf("%s",s);
	printf("Exiting with value 0\n");
	exit(0);
}

void alarmHandler() 
{
	printf("\nIt took too long to enter the string\nExiting with value -1\n");
	exit(-1);
}
