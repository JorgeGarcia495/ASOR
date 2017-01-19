#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/resource.h>
#include <stdlib.h>
#include <string.h>


int main() {

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

	printf("pid: %i, pgid: %i, sid:%i\nMax number of files that the process can allocate: %i\nCurrent working directory: %s\n", pid,pgid,sid, fileLimit.rlim_max,path);

	free(path);

	return 0;
}
