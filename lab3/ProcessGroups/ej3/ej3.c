#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>

int printProcess(char *who) {

	pid_t pid = getpid();
	pid_t pgid = getpgid(pid);
	pid_t sid = getsid(pid);

	struct rlimit fileLimit;
	
	if(getrlimit(RLIMIT_NOFILE, &fileLimit) == -1) {
		printf("Can't get number of file descriptors that a process may allocate: %s\n", strerror(errno));
		return -1;
	}

	char *path = malloc(sizeof(char)*(4096+1));

	char *cwdpath = getcwd(path, 4096+1);

	if(cwdpath == NULL) {
		printf("Can't get the current working directry: %s\n", strerror(errno));
		return -1;
	}

	printf("*** %s ***\npid: %i, pgid: %i, sid:%i\nMax number of files that the process can allocate: %i\nCurrent working directory: %s\n", who,pid,pgid,sid, fileLimit.rlim_max,path);

	free(path);

	return 0;


}

int main() {

	pid_t pid = fork();

	switch(pid) {

		case -1:
			printf("Can't fork: %s\n", strerror(errno));
			return -1;
			break;

		case 0:
			if(setsid() == -1) {

				printf("Can't create new session: %s\n", strerror(errno));
				return -1;
			}

			if(chdir("/tmp") == -1) {
				printf("Can't change dir: %s\n", strerror(errno));
				return -1;
			}

			printProcess("Child");

			return 0;
			break;

		default:
			printProcess("Parent");
		
			int status;
			wait(&status);
			if(status == -1) {
				printf("Child process failed\n");
				return -1;
			}

	}

	return 0;
}
