#include <stdio.h>
#include <unistd.h>

int main(){
	pid_t pid = getpid(); //gives the curr proc id
	pid_t ppid = getppid(); // gives parent proc id
	
	printf("proc id : %d\n", pid);
	printf("Parent proc id: %d\n",ppid);
	return 0;
}
