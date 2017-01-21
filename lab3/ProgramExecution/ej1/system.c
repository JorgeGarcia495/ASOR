#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(int argc, char **argv) {

	if(argc<2) {
		printf("Usage: %s <command> [args]\n", argv[0]);
		return 0;
	}

	int i,cmdLen = 1;

	for(i=1;i<argc;i++)
		cmdLen += strlen(argv[i])+1;

	char *command = malloc(sizeof(char)*cmdLen);

	for(i=1;i<argc;i++) {
		strcat(command, argv[i]);
		strcat(command, " ");
	}

	if(system(command) == -1) {
		printf("The system call has failed: %s\n", strerror(errno));
		return -1;
	}

	printf("The command finalized execution\n");
	return 0;
}
