#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>

int main(int argc, char **argv) {

	if(argc<2) {
		printf("Usage: %s <command> [args]\n", argv[0]);
		return 0;
	}

	pid_t pid = fork();
	int status;
	
	switch(pid) {
		case -1:
			printf("Can't fork: %s\n", strerror(errno));
			return -1;
			break;
		case 0:
			/* Child executes the process */

			if(execvp(argv[1],argv+1) == -1) {
				printf("The exec call has failed: %s\n", strerror(errno));
				return -1;
			}
			return 0;
			break;
		default:
			/* Parent waits for it to terminate and then prints its stuff */
			wait(&status);
			if(status == -1){
				printf("command failed: %s\n", strerror(errno));
				return -1;
			}
			printf("The command finalized execution\n");
			return 0;
			break;
	}
}
