#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define STD_IN   0
#define STD_OUT  1
#define STD_ERR  2


int main(int argc, char **argv) {

	pid_t pid = fork();
	int status;

	switch(pid) {

		case -1:
			printf("Can't fork: %s\n", strerror(errno));
			return -1;
			break;

		case 0:

            /* Child */

			if(setsid() == -1) {

				printf("Can't create new session: %s\n", strerror(errno));
				return -1;
			}

			if(chdir("/tmp") == -1) {
				printf("Can't change dir: %s\n", strerror(errno));
				return -1;
			}

			int daemon_stdin = open("/dev/null", O_RDONLY);
			int daemon_stdout = open("daemon.out", O_CREAT | O_WRONLY);
			int daemon_stderr = open("daemon.err", O_CREAT | O_WRONLY);

			if(daemon_stdin == -1 || daemon_stdout == -1 || daemon_stderr == -1) {
				printf("Error opening files: %s\n", strerror(errno));
                close(daemon_stdin);
                close(daemon_stdout);
                close(daemon_stderr);
                exit(-1);
			}

			if(dup2(daemon_stdin,STD_IN) == -1) {
				printf("Error redirecting from /dev/null: %s\n", strerror(errno));
                close(daemon_stdin);
                close(daemon_stdout);
                close(daemon_stderr);
                exit(-1);
			}

			if(dup2(daemon_stdout,STD_OUT) == -1) {
				printf("Error redirecting to daemon.out: %s\n", strerror(errno));
                close(daemon_stdin);
                close(daemon_stdout);
                close(daemon_stderr);
                exit(-1);
			}

			if(dup2(daemon_stderr,STD_ERR) == -1) {
				printf("Error redirecting to daemon.err: %s\n", strerror(errno));
                close(daemon_stdin);
                close(daemon_stdout);
                close(daemon_stderr);
                exit(-1);
			}

			if(execvp(argv[1],argv+1) == -1) {
				printf("The exec call has failed: %s\n", strerror(errno));
                close(daemon_stdin);
                close(daemon_stdout);
                close(daemon_stderr);
                exit(-1);
			}
			
            close(daemon_stdin);
            close(daemon_stdout);
            close(daemon_stderr);

			return 0;
			break;

		default:
		
			/* Parent */

			wait(&status);
			if(status == -1) {
				printf("Child process failed\n");
				return -1;
			}

            printf("The command finalized execution\n");

	}

	return 0;
}
